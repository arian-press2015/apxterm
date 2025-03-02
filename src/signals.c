#include "signals.h"
#include "ui.h"
#include "app.h"

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data)
{
    g_signal_connect(instance, signal, callback, user_data);
}

void add_vte(AppLayout *app_layout, AppData *app_data)
{
    GtkWidget *scroll = create_vte_box(app_data);

    int columns = 1;
    if (app_data->vte_count >= COLUMN_THRESHOLD_3)
        columns = 3;
    else if (app_data->vte_count >= COLUMN_THRESHOLD_2)
        columns = 2;

    // Clear existing VTEs from the grid
    GList *children = gtk_container_get_children(GTK_CONTAINER(app_layout->vte_grid));
    GList *existing_scrolls = NULL; // Store existing scrolled windows
    for (GList *iter = children; iter; iter = iter->next)
    {
        GtkWidget *child = GTK_WIDGET(iter->data);
        g_object_ref(child); // Keep it alive
        gtk_container_remove(GTK_CONTAINER(app_layout->vte_grid), child);
        existing_scrolls = g_list_append(existing_scrolls, child); // Save for reuse
    }
    g_list_free(children);

    // Redistribute existing VTEs and add the new one
    int row = 0, col = 0;
    for (GList *iter = existing_scrolls; iter; iter = iter->next)
    {
        GtkWidget *vte_scroll = GTK_WIDGET(iter->data);
        gtk_grid_attach(GTK_GRID(app_layout->vte_grid), vte_scroll, col, row, 1, 1);
        col++;
        if (col >= columns)
        {
            col = 0;
            row++;
        }
        g_object_unref(vte_scroll); // Release ref after attaching
    }

    // Add the new VTE
    gtk_grid_attach(GTK_GRID(app_layout->vte_grid), scroll, col, row, 1, 1);

    g_list_free(existing_scrolls); // Free the list, not the widgets (already unref’d)
}

void activate_app(GtkApplication *app, gpointer user_data)
{
    AppData *app_data = get_app_data(app);

    GtkWidget *window = create_main_window(app, "apXterm", 1200, 700);

    AppLayout *app_layout = create_main_layout(GTK_CONTAINER(window));

    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);
    add_vte(app_layout, app_data);

    GtkWidget *tree_view = create_tree_view(app_data->config);
    gtk_box_pack_start(GTK_BOX(app_layout->sidebar_box), tree_view, TRUE, TRUE, 0);

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
