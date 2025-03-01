#include "ui.h"

void append_to_container(GtkContainer *container, GtkWidget *widget)
{
    gtk_container_add(container, widget);
}

GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), name);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    return window;
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
