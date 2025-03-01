#include "ui.h"

void set_widget_size(GtkWidget *widget, gint width, gint height);
GtkTreeStore *create_tree_store(Config *config);
void add_folder_to_parent(GtkTreeStore *store, GtkTreeIter *parent, Folder *folder);
void add_server_to_folder(GtkTreeStore *store, Folder *folder, GtkTreeIter *folder_iter);

void append_to_container(GtkContainer *container, GtkWidget *widget)
{
    gtk_container_add(container, widget);
}

void append_to_paned_layout(GtkPaned *paned, GtkWidget *widget1, gboolean resize1, gboolean shrink1,
                            GtkWidget *widget2, gboolean resize2, gboolean shrink2)
{
    if (!paned || !widget1 || !widget2)
    {
        g_error(" >>Invalid arguments to append_to_paned_layout()");
    }

    gtk_paned_pack1(GTK_PANED(paned), widget1, resize1, shrink1);
    gtk_paned_pack2(GTK_PANED(paned), widget2, resize2, shrink2);
}

AppLayout *create_main_layout(GtkContainer *window)
{
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    if (!paned)
        g_error("Failed to create GtkPaned!");
    append_to_container(window, paned);

    GtkWidget *sidebar_box = create_box(GTK_ORIENTATION_VERTICAL, 5, 350, -1);
    GtkWidget *terminals_box = create_box(GTK_ORIENTATION_VERTICAL, 5, -1, -1);

    append_to_paned_layout(GTK_PANED(paned), sidebar_box, FALSE, FALSE, terminals_box, TRUE, TRUE);

    AppLayout *layout = g_new0(AppLayout, 1);
    if (!layout)
        g_error("Failed to allocate AppLayout!");
    layout->sidebar_box = sidebar_box;
    layout->terminals_box = terminals_box;

    return layout;
}

GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), name);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    return window;
}

GtkWidget *create_paned_layout(GtkOrientation orientation)
{
    GtkWidget *paned = gtk_paned_new(orientation);
    return paned;
}

GtkWidget *create_tree_view(Config *config)
{
    // Create the tree store
    GtkTreeStore *store = create_tree_store(config);

    // Create the tree view
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store); // Tree view takes ownership

    // Enable expand/collapse
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), FALSE);
    gtk_tree_view_expand_all(GTK_TREE_VIEW(tree_view)); // Start expanded (optional)

    // Add column with icon and text
    GtkCellRenderer *pixbuf_renderer = gtk_cell_renderer_pixbuf_new();
    GtkCellRenderer *text_renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(column, "Name");
    gtk_tree_view_column_pack_start(column, pixbuf_renderer, FALSE);
    gtk_tree_view_column_add_attribute(column, pixbuf_renderer, "pixbuf", COLUMN_ICON);
    gtk_tree_view_column_pack_start(column, text_renderer, TRUE);
    gtk_tree_view_column_add_attribute(column, text_renderer, "text", COLUMN_NAME);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // Wrap in a scrolled window
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    append_to_container(GTK_CONTAINER(scrolled), tree_view);

    return scrolled;
}

GtkTreeStore *create_tree_store(Config *config)
{
    GtkTreeStore *store = gtk_tree_store_new(NUM_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING);
    GtkTreeIter user_iter;

    for (int i = 0; i < config->user_count; i++)
    {
        User *user = &config->users[i];
        gtk_tree_store_append(store, &user_iter, NULL);
        gtk_tree_store_set(store, &user_iter,
                           COLUMN_ICON, gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), "avatar-default", 16, 0, NULL),
                           COLUMN_NAME, user->username,
                           -1);

        for (int j = 0; j < user->folder_count; j++)
        {
            add_folder_to_parent(store, &user_iter, &user->folders[j]);
        }
    }

    return store;
}

void add_folder_to_parent(GtkTreeStore *store, GtkTreeIter *parent, Folder *folder)
{
    GtkTreeIter folder_iter;

    // Add this folder as a node
    gtk_tree_store_append(store, &folder_iter, parent);
    gtk_tree_store_set(store, &folder_iter,
                       COLUMN_ICON, gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), "folder", 16, 0, NULL),
                       COLUMN_NAME, folder->name,
                       -1);

    // Recursively add sub-folders
    for (int i = 0; i < folder->folder_count; i++)
    {
        add_folder_to_parent(store, &folder_iter, &folder->folders[i]);
    }

    add_server_to_folder(store, folder, &folder_iter);
}

void add_server_to_folder(GtkTreeStore *store, Folder *folder, GtkTreeIter *folder_iter)
{
    GtkTreeIter server_iter;
    for (int i = 0; i < folder->server_count; i++)
    {
        gtk_tree_store_append(store, &server_iter, folder_iter);
        gtk_tree_store_set(store, &server_iter,
                           COLUMN_ICON, gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), "network-server", 16, 0, NULL),
                           COLUMN_NAME, folder->servers[i].name,
                           -1);
    }
}


GtkWidget *create_box(GtkOrientation orientation, gint spacing, gint width, gint height)
{
    GtkWidget *box = gtk_box_new(orientation, spacing);

    set_widget_size(box, width, height);

    return box;
}

GtkWidget *create_vte()
{
    GtkWidget *vte = vte_terminal_new();

    VtePtyFlags pty_flags = VTE_PTY_DEFAULT;       // Default PTY settings
    const char *working_directory = NULL;          // Use current directory
    char *argv_child[] = {"/bin/fish", NULL};      // Run fish
    char **envv = NULL;                            // Inherit parent’s environment
    GSpawnFlags spawn_flags = G_SPAWN_SEARCH_PATH; // Search PATH for executable
    GSpawnChildSetupFunc child_setup = NULL;       // No custom setup
    gpointer child_setup_data = NULL;              // No setup data
    GPid child_pid = -1;                           // Placeholder for PID
    GCancellable *cancellable = NULL;              // No cancellation
    GError *error = NULL;                          // Error handling

    gboolean success = vte_terminal_spawn_sync(
        VTE_TERMINAL(vte),
        pty_flags,
        working_directory,
        argv_child,
        envv,
        spawn_flags,
        child_setup,
        child_setup_data,
        &child_pid,
        cancellable,
        &error);

    if (!success)
    {
        g_printerr("Failed to spawn terminal: %s\n", error ? error->message : "Unknown error");
        if (error)
            g_error_free(error);
        return NULL;
    }

    return vte;
}

void set_widget_size(GtkWidget *widget, gint width, gint height)
{
    gtk_widget_set_size_request(widget, width, height);
}