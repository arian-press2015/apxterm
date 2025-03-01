#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <cjson/cJSON.h>

#define MAX_NAME_LEN 128
#define MAX_IP_LEN 16
#define MAX_SSH_KEY_LEN 256

typedef struct
{
    char name[MAX_NAME_LEN];
    char ip[MAX_IP_LEN];
    char ssh_key[MAX_SSH_KEY_LEN];
} Server;

typedef struct Folder
{
    char name[MAX_NAME_LEN];
    struct Folder *folders;
    int folder_count;
    Server *servers;
    int server_count;
} Folder;

typedef struct User
{
    char username[MAX_NAME_LEN];
    Folder *folders;
    int folder_count;
} User;

typedef struct Config
{
    User **users;
    int user_count;
} Config;

Config *get_config(const char *config_path);
void free_config(Config *config);

#endif