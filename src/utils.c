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

#include "utils.h"

void ssh_vte(GtkWidget *vte, Server *server)
{
    VteTerminal *terminal = VTE_TERMINAL(vte);
    char ssh_command[512];

    if (server->ssh_key[0] == '\0')
    {
        snprintf(ssh_command, sizeof(ssh_command), "ssh %s@%s\n",
                 server->username, server->ip);
    }
    else
    {
        snprintf(ssh_command, sizeof(ssh_command), "ssh -i %s %s@%s\n",
                 server->ssh_key, server->username, server->ip);
    }

    vte_terminal_feed_child(terminal, ssh_command, -1);
}