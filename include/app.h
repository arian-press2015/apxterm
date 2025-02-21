#ifndef APP_H
#define APP_H

#include <gtk/gtk.h>

GtkApplication *init_app();
int run_app(GtkApplication *app, int argc, char **argv);
void clear_app(GtkApplication *app);

#endif