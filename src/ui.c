#include "ui.h"

void set_widget_size(GtkWidget *widget, gint width, gint height);

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

    GtkWidget *sidebar_box = create_box(GTK_ORIENTATION_VERTICAL, 5, 200, -1);
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