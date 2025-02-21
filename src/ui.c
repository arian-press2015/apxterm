#include "ui.h"

void append_to_container(GtkContainer *container, GtkWidget *widget)
{
    // use GTK_CONTAINER(some_widget) if container is a widget
    gtk_container_add(container, widget);
}

void append_to_box(GtkBox *box, GtkWidget *widget, gboolean expand, gboolean fill, guint padding)
{
    // use GTK_BOX(box_widget) if the box is of type GtkWidget
    gtk_box_pack_start(box, widget, expand, fill, padding);
}

GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), name);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    return window;
}

GtkWidget *create_box(GtkOrientation orientation, gint spacing)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    return vbox;
}

GtkWidget *create_text_view(GtkBox *box, gboolean expand, gboolean fill, guint padding)
{
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    return text_view;
}

GtkWidget *create_entry(GtkBox *box, gboolean expand, gboolean fill, guint padding)
{
    GtkWidget *input_entry = gtk_entry_new();
    return input_entry;
}
