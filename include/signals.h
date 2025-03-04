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

#ifndef SIGNALS_H
#define SIGNALS_H

#include <gtk/gtk.h>
#include "ui.h"
#include "app.h"
#include "utils.h"

typedef struct
{
    AppData *app_data;
    AppLayout *app_layout;
} AppState;

void connect_signal(gpointer instance, const gchar *signal, GCallback callback, gpointer user_data);
void activate_app(GtkApplication *app, gpointer user_data);
void send_to_all_terminals(GtkButton *button, gpointer user_data);

#endif