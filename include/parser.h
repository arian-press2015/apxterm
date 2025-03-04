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

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <cjson/cJSON.h>

#define MAX_CONFIG_PATH_LEN 128
#define MAX_NAME_LEN 128
#define MAX_IP_LEN 16
#define MAX_SSH_KEY_LEN 256
#define MAX_ID_LEN 32
#define MAX_USERNAME_LEN 32

typedef struct Server
{
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char ip[MAX_IP_LEN];
    char username[MAX_USERNAME_LEN];
    char ssh_key[MAX_SSH_KEY_LEN];
} Server;

typedef struct Folder
{
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    struct Folder *folders;
    int folder_count;
    Server *servers;
    int server_count;
} Folder;

typedef struct User
{
    char id[MAX_ID_LEN];
    char username[MAX_NAME_LEN];
    Folder *folders;
    int folder_count;
} User;

typedef struct Config
{
    User *users;
    int user_count;
} Config;

char *get_config_path();
Config *get_config(const char *config_path);
void free_config(Config *config);

#endif