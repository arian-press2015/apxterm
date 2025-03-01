#include "signals.h"
#include "ui.h"

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data)
{
    g_signal_connect(instance, signal, callback, user_data);
}

void activate_app(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = create_main_window(app, "apXterm", 700, 400);

    GtkWidget *vte = create_vte();

    append_to_container(GTK_CONTAINER(window), vte);

    gtk_widget_show_all(window);
}
