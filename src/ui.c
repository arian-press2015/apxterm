/*
 * apXterm - A terminal emulator with SSH support
 * Copyright (C) 2025 arian-press2015 <arian.press2015@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
    GtkWidget *vte_grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(vte_grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(vte_grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(vte_grid), GRID_SPACING);
    gtk_grid_set_column_spacing(GTK_GRID(vte_grid), GRID_SPACING);

    gtk_box_pack_start(GTK_BOX(terminals_box), vte_grid, TRUE, TRUE, 0);

    append_to_paned_layout(GTK_PANED(paned), sidebar_box, FALSE, FALSE, terminals_box, TRUE, TRUE);

    AppLayout *layout = g_new0(AppLayout, 1);
    if (!layout)
    {
        g_error("Failed to allocate AppLayout!");
    }
    layout->sidebar_box = sidebar_box;
    layout->terminals_box = terminals_box;
    layout->vte_grid = vte_grid;

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

GtkWidget *create_scrolled_tree_view(GtkWidget *tree_view)
{
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    append_to_container(GTK_CONTAINER(scrolled), tree_view);

    return scrolled;
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

    return tree_view;
}

GtkTreeStore *create_tree_store(Config *config)
{
    GtkTreeStore *store = gtk_tree_store_new(NUM_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
    GtkTreeIter user_iter;

    for (int i = 0; i < config->user_count; i++)
    {
        User *user = &config->users[i];
        gtk_tree_store_append(store, &user_iter, NULL);
        gtk_tree_store_set(store, &user_iter,
                           COLUMN_ICON, gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), "avatar-default", 16, 0, NULL),
                           COLUMN_NAME, user->username,
                           COLUMN_TYPE, USER_NODE,
                           COLUMN_ID, user->id,
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
                       COLUMN_TYPE, FOLDER_NODE,
                       COLUMN_ID, folder->id,
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
                           COLUMN_TYPE, SERVER_NODE,
                           COLUMN_ID, folder->servers[i].id,
                           -1);
    }
}

GtkWidget *create_box(GtkOrientation orientation, gint spacing, gint width, gint height)
{
    GtkWidget *box = gtk_box_new(orientation, spacing);

    set_widget_size(box, width, height);

    return box;
}

GtkWidget *create_vte_grid()
{
    return NULL;
}

GList *clear_vte_grid(AppLayout *app_layout)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(app_layout->vte_grid));
    GList *removed_scrolls = NULL; // Store existing scrolled windows
    for (GList *iter = children; iter; iter = iter->next)
    {
        GtkWidget *child = GTK_WIDGET(iter->data);
        g_object_ref(child); // Keep it alive
        gtk_container_remove(GTK_CONTAINER(app_layout->vte_grid), child);
        removed_scrolls = g_list_prepend(removed_scrolls, child); // Save for reuse
    }
    g_list_free(children);

    return removed_scrolls;
}

void fill_vte_grid(AppLayout *app_layout, GList *scrolls, int columns)
{
    int row = 0, col = 0;
    for (GList *iter = scrolls; iter; iter = iter->next)
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
    g_list_free(scrolls); // Free the list, not the widgets (already unref’d)
}

BoxedVte *create_vte_box(AppData *app_data, Server *server)
{
    GtkWidget *vte_box = create_box(GTK_ORIENTATION_VERTICAL, 5, -1, -1);

    char label[MAX_NAME_LEN + MAX_IP_LEN + 4];
    snprintf(label, sizeof(label), "%s [%s]", server->name, server->ip);

    GtkWidget *checkbox = gtk_check_button_new_with_label(label);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox), FALSE);
    gtk_box_pack_start(GTK_BOX(vte_box), checkbox, FALSE, FALSE, 0);

    GtkWidget *vte = create_vte();
    GtkWidget *scroll = create_scrolled_vte(vte);
    gtk_box_pack_start(GTK_BOX(vte_box), scroll, TRUE, TRUE, 0);

    TerminalEntry *entry = g_new0(TerminalEntry, 1);
    entry->vte = vte;
    entry->checkbox = checkbox;
    entry->server = server;
    app_data->terminals = g_list_append(app_data->terminals, entry);
    app_data->vte_count++;

    BoxedVte *boxed_vte = g_new0(BoxedVte, 1);
    boxed_vte->vte_box = vte_box;
    boxed_vte->vte = vte;
    return boxed_vte;
}

GtkWidget *create_scrolled_vte(GtkWidget *vte)
{
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    append_to_container(GTK_CONTAINER(scroll), vte);

    return scroll;
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
        g_error("Failed to spawn terminal: %s\n", error ? error->message : "Unknown error");
        return NULL;
    }

    return vte;
}

void set_widget_size(GtkWidget *widget, gint width, gint height)
{
    gtk_widget_set_size_request(widget, width, height);
}