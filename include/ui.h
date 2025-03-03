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
    NUM_COLUMNS
};

enum
{
    USER_NODE,
    FOLDER_NODE,
    SERVER_NODE
};

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
GtkWidget *create_vte_box(AppData *app_data, Server *server);
GtkWidget *create_scrolled_vte(GtkWidget *vte);
GtkWidget *create_vte();

#endif