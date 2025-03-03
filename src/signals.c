#include "signals.h"

void add_vte(AppLayout *app_layout, AppData *app_data, Server *server);

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data)
{
    g_signal_connect(instance, signal, callback, user_data);
}

Node get_node_by_id(Config *config, const char *id)
{
    Node node = {NULL, USER_NODE};

    for (int i = 0; i < config->user_count; i++)
    {
        User *user = &config->users[i];
        if (strcmp(user->id, id) == 0)
        {
            node.node = user;
            node.type = USER_NODE;
            return node;
        }

        for (int j = 0; j < user->folder_count; j++)
        {
            Folder *folder = &user->folders[j];
            if (strcmp(folder->id, id) == 0)
            {
                node.node = folder;
                node.type = FOLDER_NODE;
                return node;
            }

            for (int k = 0; k < folder->server_count; k++)
            {
                if (strcmp(folder->servers[k].id, id) == 0)
                {
                    node.node = &folder->servers[k];
                    node.type = SERVER_NODE;
                    return node;
                }
            }

            for (int m = 0; m < folder->folder_count; m++)
            {
                Folder *sub_folder = &folder->folders[m];
                if (strcmp(sub_folder->id, id) == 0)
                {
                    node.node = sub_folder;
                    node.type = FOLDER_NODE;
                    return node;
                }
                for (int n = 0; n < sub_folder->server_count; n++)
                {
                    if (strcmp(sub_folder->servers[n].id, id) == 0)
                    {
                        node.node = &sub_folder->servers[n];
                        node.type = SERVER_NODE;
                        return node;
                    }
                }
            }
        }
    }
    return node;
}

void on_row_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
    AppState *state = (AppState *)user_data;
    if (!state)
    {
        g_error(">> state is missing!");
        return;
    }

    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreeIter iter;
    if (!gtk_tree_model_get_iter(model, &iter, path))
    {
        return;
    }

    gchar *id;
    gtk_tree_model_get(model, &iter, COLUMN_ID, &id, -1);

    Node node = get_node_by_id(state->app_data->config, id);
    if (node.node && node.type == SERVER_NODE)
    {
        Server *server = (Server *)node.node;
        add_vte(state->app_layout, state->app_data, server);
    }
    else
    {
        printf("Clicked non-server node (ID: %s)\n", id);
    }
}

void add_vte(AppLayout *app_layout, AppData *app_data, Server *server)
{
    GtkWidget *scroll = create_vte_box(app_data, server);

    int columns = 1;
    if (app_data->vte_count >= COLUMN_THRESHOLD_3)
        columns = 3;
    else if (app_data->vte_count >= COLUMN_THRESHOLD_2)
        columns = 2;

    GList *removed_scrolls = clear_vte_grid(app_layout);
    removed_scrolls = g_list_append(removed_scrolls, scroll);
    g_object_ref(scroll);

    fill_vte_grid(app_layout, removed_scrolls, columns);
    gtk_widget_show_all(scroll);
}

void activate_app(GtkApplication *app, gpointer user_data)
{
    AppData *app_data = get_app_data(app);

    GtkWidget *window = create_main_window(app, "apXterm", 1200, 700);

    AppLayout *app_layout = create_main_layout(GTK_CONTAINER(window));

    GtkWidget *tree_view = create_tree_view(app_data->config);
    GtkWidget *scrolled_tree = create_scrolled_tree_view(tree_view);
    AppState *state = g_new0(AppState, 1);
    state->app_data = app_data;
    state->app_layout = app_layout;

    g_signal_connect(tree_view, "row-activated", G_CALLBACK(on_row_activated), state);
    gtk_box_pack_start(GTK_BOX(app_layout->sidebar_box), scrolled_tree, TRUE, TRUE, 0);

    app_data->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->entry), "Enter command for all terminals");
    g_signal_connect(app_data->entry, "activate", G_CALLBACK(send_to_all_terminals), app_data);
    gtk_box_pack_start(GTK_BOX(app_layout->terminals_box), app_data->entry, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

void send_to_all_terminals(GtkButton *button, gpointer user_data)
{
    AppData *app_data = (AppData *)user_data;
    const char *command = gtk_entry_get_text(GTK_ENTRY(app_data->entry));

    // Append a newline to simulate pressing Enter
    char command_with_newline[strlen(command) + 2];
    snprintf(command_with_newline, sizeof(command_with_newline), "%s\n", command);

    // Broadcast to all terminals
    GList *iter;
    for (iter = app_data->terminals; iter != NULL; iter = iter->next)
    {
        TerminalEntry *entry = (TerminalEntry *)iter->data;
        VteTerminal *terminal = VTE_TERMINAL(entry->vte);
        if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(entry->checkbox)))
        {
            vte_terminal_feed_child(terminal, command_with_newline, -1);
        }
    }

    // Clear the entry field
    gtk_entry_set_text(GTK_ENTRY(app_data->entry), "");
}
