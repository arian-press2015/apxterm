/*
 * apXterm - A terminal emulator with SSH support
 * Copyright (C) 2025 Your Name <your.email@example.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "app.h"
#include "signals.h"

AppData *create_app_data();

GtkApplication *init_app()
{
    GtkApplication *app = gtk_application_new("org.gtk.apxterm", G_APPLICATION_FLAGS_NONE);

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

    char *config_path = get_config_path();
    app_data->config = get_config(config_path);
    if (!app_data->config)
    {
        g_error(">> Failed to load config.json\n");
    }

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