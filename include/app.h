#ifndef APP_H
#define APP_H

#include <gtk/gtk.h>
#include "parser.h"

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