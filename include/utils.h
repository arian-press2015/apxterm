#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include <vte/vte.h>
#include "parser.h"

void ssh_vte(GtkWidget *vte, Server *server);

#endif