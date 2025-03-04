/*
 * apXterm - A terminal emulator with SSH support
 * Copyright (C) 2025 AP2015 <arian.press@gmail.com>
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

#ifndef APP_H
#define APP_H

#include <gtk/gtk.h>
#include "parser.h"

typedef struct TerminalEntry
{
    GtkWidget *vte;
    GtkWidget *checkbox;
    Server *server;
} TerminalEntry;

typedef struct
{
    GList *terminals;
    GtkWidget *entry;
    int vte_count;
    Config *config;
} AppData;

#define APP_DATA_KEY "app-data"

GtkApplication *init_app();
int run_app(GtkApplication *app, int argc, char **argv);
void clear_app(GtkApplication *app);
AppData *get_app_data(GtkApplication *app);

#endif