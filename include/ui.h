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

#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <vte/vte.h>
#include "parser.h"
#include "app.h"

typedef struct
{
    GtkWidget *sidebar_box;
    GtkWidget *terminals_box;
    GtkWidget *vte_grid;
} AppLayout;

enum
{
    COLUMN_ICON,
    COLUMN_NAME,
    COLUMN_TYPE,
    COLUMN_ID,
    NUM_COLUMNS
};

typedef enum
{
    USER_NODE,
    FOLDER_NODE,
    SERVER_NODE
} NodeType;

typedef struct
{
    void *node;
    NodeType type;
} Node;

typedef struct
{
    GtkWidget *vte_box;
    GtkWidget *vte;
} BoxedVte;

#define COLUMN_THRESHOLD_2 5
#define COLUMN_THRESHOLD_3 13

#define GRID_SPACING 2

void append_to_container(GtkContainer *container, GtkWidget *widget);
void append_to_paned_layout(GtkPaned *paned, GtkWidget *widget1, gboolean resize1, gboolean shrink1,
                            GtkWidget *widget2, gboolean resize2, gboolean shrink2);
AppLayout *create_main_layout(GtkContainer *window);
GtkWidget *create_scrolled_tree_view(GtkWidget *tree_view);
GtkWidget *create_tree_view(Config *config);
GtkWidget *create_main_window(GtkApplication *app, char *name, int width, int height);
GtkWidget *create_paned_layout(GtkOrientation orientation);
GtkWidget *create_box(GtkOrientation orientation, gint spacing, gint width, gint height);
GtkWidget *create_vte_grid();
GList *clear_vte_grid(AppLayout *app_layout);
void fill_vte_grid(AppLayout *app_layout, GList *scrolls, int columns);
BoxedVte *create_vte_box(AppData *app_data, Server *server);
GtkWidget *create_scrolled_vte(GtkWidget *vte);
GtkWidget *create_vte();

#endif