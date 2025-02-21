#ifndef SIGNALS_H
#define SIGNALS_H

#include <gtk/gtk.h>

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data);

void activate_app(GtkApplication *app, gpointer user_data);

void send_input(GtkWidget *widget, gpointer data);

void on_window_destroy(GtkWidget *widget, gpointer data);

#endif