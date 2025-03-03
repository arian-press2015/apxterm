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