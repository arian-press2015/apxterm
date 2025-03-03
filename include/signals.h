#ifndef SIGNALS_H
#define SIGNALS_H

#include <gtk/gtk.h>
#include "ui.h"
#include "app.h"
#include "utils.h"

typedef struct
{
    AppData *app_data;
    AppLayout *app_layout;
} AppState;

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data);
void activate_app(GtkApplication *app, gpointer user_data);
void send_to_all_terminals(GtkButton *button, gpointer user_data);

#endif