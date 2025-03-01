#include "signals.h"
#include "ui.h"
#include "app.h"

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data)
{
    g_signal_connect(instance, signal, callback, user_data);
}

void activate_app(GtkApplication *app, gpointer user_data)
{
    AppData *app_data = get_app_data(app);

    GtkWidget *window = create_main_window(app, "apXterm", 700, 400);

    GtkWidget *paned = create_paned_layout(GTK_ORIENTATION_HORIZONTAL);
    append_to_container(GTK_CONTAINER(window), paned);

    // Left sidebar (placeholder for now)
    GtkWidget *sidebar = gtk_label_new("Sidebar Placeholder\n(To be defined later)");
    gtk_widget_set_size_request(sidebar, 200, -1);            // Set a minimum width
    gtk_paned_pack1(GTK_PANED(paned), sidebar, FALSE, FALSE); // Fixed size sidebar

    // Right side: terminal area
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_paned_pack2(GTK_PANED(paned), vbox, TRUE, TRUE); // Expandable terminal area

    // Create two VTE terminals
    GtkWidget *vte1 = create_vte();
    if (!vte1)
    {
        g_printerr("Failed to create first terminal\n");
        return;
    }
    app_data->terminals = g_list_append(app_data->terminals, vte1);

    GtkWidget *vte2 = create_vte();
    if (!vte2)
    {
        g_printerr("Failed to create second terminal\n");
        return;
    }
    app_data->terminals = g_list_append(app_data->terminals, vte2);

    // Add terminals to scrolled windows
    GtkWidget *scroll1 = gtk_scrolled_window_new(NULL, NULL);
    append_to_container(GTK_CONTAINER(scroll1), vte1);
    GtkWidget *scroll2 = gtk_scrolled_window_new(NULL, NULL);
    append_to_container(GTK_CONTAINER(scroll2), vte2);

    gtk_box_pack_start(GTK_BOX(vbox), scroll1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scroll2, TRUE, TRUE, 0);

    // Add text entry and button for broadcasting
    app_data->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->entry), "Enter command for all terminals");
    gtk_box_pack_start(GTK_BOX(vbox), app_data->entry, FALSE, FALSE, 0);

    GtkWidget *send_button = gtk_button_new_with_label("Send to All");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_to_all_terminals), app_data);
    gtk_box_pack_start(GTK_BOX(vbox), send_button, FALSE, FALSE, 0);

    // Show everything
    gtk_widget_show_all(window);

    // Note: app_data should be freed on window close in a real app
}

// Callback to send input to all terminals
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
        VteTerminal *terminal = VTE_TERMINAL(iter->data);
        vte_terminal_feed_child(terminal, command_with_newline, -1);
    }

    // Clear the entry field
    gtk_entry_set_text(GTK_ENTRY(app_data->entry), "");
}
