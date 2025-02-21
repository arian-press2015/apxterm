#include "ui.h"

void append_widget(GtkContainer *container, GtkWidget *widget)
{
    // use GTK_CONTAINER(some_widget) if container is a widget
    gtk_container_add(container, widget);
}

GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), name);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    return window;
}

GtkWidget *create_box(GtkApplication *app, GtkOrientation orientation, gint spacing)
{
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    return vbox;
}

void append_text_view(GtkBox *box, gboolean expand, gboolean fill, guint padding)
{
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    // use GTK_BOX(box_widget) if the box is of type GtkWidget
    gtk_box_pack_start(box, text_view, expand, fill, padding);
}

void append_entry(GtkBox *box, gboolean expand, gboolean fill, guint padding)
{
    GtkWidget *input_entry = gtk_entry_new();
    // use GTK_BOX(box_widget) if the box is of type GtkWidget
    gtk_box_pack_start(box, input_entry, expand, fill, padding);
}
