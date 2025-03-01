#include "app.h"
#include "signals.h"

AppData *create_app_data();

GtkApplication *init_app()
{
    GtkApplication *app = gtk_application_new("org.gtk.apXterm", G_APPLICATION_FLAGS_NONE);

    AppData *app_data = create_app_data();
    g_object_set_data_full(G_OBJECT(app), APP_DATA_KEY, app_data, g_free);

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

AppData *create_app_data() 
{
    AppData *app_data = g_new0(AppData, 1);
    app_data->terminals = NULL;

    return app_data;
}

AppData *get_app_data(GtkApplication *app)
{
    AppData *app_data = g_object_get_data(G_OBJECT(app), APP_DATA_KEY);
    if (!app_data)
    {
        g_error(">> AppData not initialized!\n");
        return NULL;
    }

    return app_data;
}