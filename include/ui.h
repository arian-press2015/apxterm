#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

void append_widget(GtkContainer *container, GtkWidget *widget);

GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height);

GtkWidget *create_box(GtkApplication *app, GtkOrientation orientation, gint spacing);

void append_text_view(GtkBox *box, gboolean expand, gboolean fill, guint padding);

void append_entry(GtkBox *box, gboolean expand, gboolean fill, guint padding);

#endif