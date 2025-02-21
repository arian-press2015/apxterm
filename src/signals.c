#include "signals.h"
#include "ui.h"

void activate_app(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = create_main_window(app, "apXterm", 700, 400);
    gtk_widget_show_all(window);
}

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data)
{
    g_signal_connect(instance, signal, callback, user_data);
}