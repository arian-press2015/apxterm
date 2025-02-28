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
    GtkWidget *vbox = gtk_box_new(orientation, spacing);
    return vbox;
}

GtkWidget *create_scrolled_window()
{
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_hexpand(scrolled_window, TRUE);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    return scrolled_window;
}

GtkWidget *create_text_view(gboolean editable, GtkWrapMode wrap_mode)
{
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), editable);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), wrap_mode);
    return text_view;
}

GtkWidget *create_entry()
{
    GtkWidget *input_entry = gtk_entry_new();
    return input_entry;
}
