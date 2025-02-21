#include "app.h"

GtkApplication *init_app()
{
    return gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
}

int run_app(GtkApplication *app, int argc, char **argv)
{
    return g_application_run(G_APPLICATION(app), argc, argv);
}

void clear_app(GtkApplication *app)
{
    g_object_unref(app);
}