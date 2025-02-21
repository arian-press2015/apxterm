#include "app.h"
#include "signals.h"

GtkApplication *init_app()
{
    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    connect_signal(app, "activate", G_CALLBACK(activate_app), NULL);
    return app;
}

int run_app(GtkApplication *app, int argc, char **argv)
{
    return g_application_run(G_APPLICATION(app), argc, argv);
}

void clear_app(GtkApplication *app)
{
    g_object_unref(app);
}