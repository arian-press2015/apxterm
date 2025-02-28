#include "signals.h"
#include "ui.h"

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data)
{
    g_signal_connect(instance, signal, callback, user_data);
}

void activate_app(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = create_main_window(app, "apXterm", 700, 400);

    GtkWidget *vbox = create_box(GTK_ORIENTATION_VERTICAL, 5);
    append_to_container(GTK_CONTAINER(window), vbox);

    GtkWidget *scrolled_window = create_scrolled_window();
    append_to_container(GTK_CONTAINER(vbox), scrolled_window);

    GtkWidget *text_view = create_text_view(FALSE, GTK_WRAP_WORD_CHAR);
    append_to_container(GTK_CONTAINER(scrolled_window), text_view);

    GtkWidget *entry = create_entry();
    append_to_box(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    connect_signal(entry, "activate", G_CALLBACK(send_input), text_view);

    gtk_widget_show_all(window);
}

void send_input(GtkWidget *widget, gpointer data)
{
    const char *input = gtk_entry_get_text(GTK_ENTRY(widget));
    GtkWidget *text_view = GTK_WIDGET(data);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    gtk_text_buffer_insert_at_cursor(buffer, input, -1);
    gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);

    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(text_view), &end_iter, 0.0, TRUE, 0.0, 1.0);

    gtk_entry_set_text(GTK_ENTRY(widget), "");
}
