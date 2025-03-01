#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <vte/vte.h>

void append_to_container(GtkContainer *container, GtkWidget *widget);
GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height);
GtkWidget *create_vte();

#endif