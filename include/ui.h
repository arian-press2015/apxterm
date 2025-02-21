#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

void append_to_container(GtkContainer *container, GtkWidget *widget);

void append_to_box(GtkBox *box, GtkWidget *widget, gboolean expand, gboolean fill, guint padding);

GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height);

GtkWidget *create_box(GtkOrientation orientation, gint spacing);

GtkWidget *create_text_view(gboolean editable, GtkWrapMode wrap_mode);

GtkWidget *create_entry();

#endif