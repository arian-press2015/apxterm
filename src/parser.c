#include "parser.h"

char *read_config(const char *config_path);
Config *parse_config(const char *json_data);
void parse_user(cJSON *user_json, User *user);
void parse_folder(cJSON *folder_json, Folder *folder);
void parse_server(cJSON *server_json, Server *server);
void free_config(Config *config);
void free_user(User *user);
void free_folder(Folder *folder);

char *get_config_path()
{
    const char *home_dir = getenv("HOME");
    if (!home_dir)
    {
        g_error("Could not determine home directory");
    }
    char *config_path = (char *)malloc(sizeof(char) * MAX_CONFIG_PATH_LEN);
    snprintf(config_path, MAX_CONFIG_PATH_LEN, "%s/.config/apxterm/config.json", home_dir);

    return config_path;
}

Config *get_config(const char *config_path)
{
    char *config_str = read_config(config_path);
    Config *config = parse_config(config_str);

    free(config_str);

    return config;
}

char *read_config(const char *config_path)
{
    FILE *file = fopen(config_path, "r");
    if (!file)
    {
        g_error(">> Error opening file: %s\n", config_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_data = (char *)malloc(length + 1);
    fread(json_data, 1, length, file);
    json_data[length] = '\0';
    fclose(file);

    return json_data;
}

Config *parse_config(const char *json_data)
{
    cJSON *root = cJSON_Parse(json_data);
    if (!root)
    {
        g_error(">> Error parsing JSON\n");
        return NULL;
    }

    Config *config = malloc(sizeof(Config));

    if (!config)
    {
        g_error(">> Failed to allocate Config");
        cJSON_Delete(root);
        return NULL;
    }

    config->users = NULL;
    config->user_count = 0;

    cJSON *users_json = cJSON_GetObjectItemCaseSensitive(root, "users");
    if (users_json && cJSON_IsArray(users_json))
    {
        config->user_count = cJSON_GetArraySize(users_json);
        config->users = malloc(config->user_count * sizeof(User));

        if (!config->users)
        {
            g_error(">> Failed to allocate users array\n");
            free(config);
            cJSON_Delete(root);
            return NULL;
        }

        for (int i = 0; i < config->user_count; ++i)
        {
            parse_user(cJSON_GetArrayItem(users_json, i), &config->users[i]);
        }
    }

    cJSON_Delete(root);
    return config;
}

void parse_user(cJSON *user_json, User *user)
{
    user->folders = NULL;
    user->folder_count = 0;
    user->username[0] = '\0';
    user->id[0] = '\0';

    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(user_json, "id");
    if (id_json && cJSON_IsString(id_json))
    {
        strncpy(user->id, id_json->valuestring, MAX_ID_LEN - 1);
        user->id[MAX_ID_LEN - 1] = '\0';
    }

    cJSON *username_json = cJSON_GetObjectItemCaseSensitive(user_json, "username");
    if (username_json && cJSON_IsString(username_json))
    {
        strncpy(user->username, username_json->valuestring, MAX_NAME_LEN - 1);
        user->username[MAX_NAME_LEN - 1] = '\0';
    }

    cJSON *folders_json = cJSON_GetObjectItemCaseSensitive(user_json, "folders");
    if (folders_json && cJSON_IsArray(folders_json))
    {
        user->folder_count = cJSON_GetArraySize(folders_json);
        user->folders = malloc(user->folder_count * sizeof(Folder));
        if (!user->folders)
        {
            g_error(">> Failed to allocate folders for user\n");
            return;
        }
        for (int i = 0; i < user->folder_count; ++i)
        {
            parse_folder(cJSON_GetArrayItem(folders_json, i), &user->folders[i]);
        }
    }
}

void parse_folder(cJSON *folder_json, Folder *folder)
{
    folder->folders = NULL;
    folder->folder_count = 0;
    folder->servers = NULL;
    folder->server_count = 0;
    folder->name[0] = '\0';
    folder->id[0] = '\0';

    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(folder_json, "id");
    if (id_json && cJSON_IsString(id_json))
    {
        strncpy(folder->id, id_json->valuestring, MAX_ID_LEN - 1);
        folder->id[MAX_ID_LEN - 1] = '\0';
    }

    cJSON *name_json = cJSON_GetObjectItemCaseSensitive(folder_json, "name");
    if (name_json && cJSON_IsString(name_json))
    {
        strncpy(folder->name, name_json->valuestring, MAX_NAME_LEN - 1);
        folder->name[MAX_NAME_LEN - 1] = '\0';
    }

    cJSON *nested_folders_json = cJSON_GetObjectItemCaseSensitive(folder_json, "folders");
    if (nested_folders_json && cJSON_IsArray(nested_folders_json))
    {
        folder->folder_count = cJSON_GetArraySize(nested_folders_json);
        folder->folders = malloc(folder->folder_count * sizeof(Folder));
        if (!folder->folders)
        {
            g_error(">> Failed to allocate nested folders\n");
            return;
        }
        for (int i = 0; i < folder->folder_count; ++i)
        {
            parse_folder(cJSON_GetArrayItem(nested_folders_json, i), &folder->folders[i]);
        }
    }

    cJSON *servers_json = cJSON_GetObjectItemCaseSensitive(folder_json, "servers");
    if (servers_json && cJSON_IsArray(servers_json))
    {
        folder->server_count = cJSON_GetArraySize(servers_json);
        folder->servers = malloc(folder->server_count * sizeof(Server));
        if (!folder->servers)
        {
            g_error(">> Failed to allocate servers\n");
            return;
        }
        for (int i = 0; i < folder->server_count; ++i)
        {
            parse_server(cJSON_GetArrayItem(servers_json, i), &folder->servers[i]);
        }
    }
}

void parse_server(cJSON *server_json, Server *server)
{
    server->name[0] = '\0';
    server->ip[0] = '\0';
    server->ssh_key[0] = '\0';
    server->id[0] = '\0';
    server->username[0] = '\0';

    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(server_json, "id");
    if (id_json && cJSON_IsString(id_json))
    {
        strncpy(server->id, id_json->valuestring, MAX_ID_LEN - 1);
        server->id[MAX_ID_LEN - 1] = '\0';
    }

    cJSON *name_json = cJSON_GetObjectItemCaseSensitive(server_json, "name");
    if (name_json && cJSON_IsString(name_json))
    {
        strncpy(server->name, name_json->valuestring, MAX_NAME_LEN - 1);
        server->name[MAX_NAME_LEN - 1] = '\0';
    }

    cJSON *ip_json = cJSON_GetObjectItemCaseSensitive(server_json, "ip");
    if (ip_json && cJSON_IsString(ip_json))
    {
        strncpy(server->ip, ip_json->valuestring, MAX_IP_LEN - 1);
        server->ip[MAX_IP_LEN - 1] = '\0';
    }

    cJSON *username_json = cJSON_GetObjectItemCaseSensitive(server_json, "username");
    if (username_json && cJSON_IsString(username_json))
    {
        strncpy(server->username, username_json->valuestring, MAX_USERNAME_LEN - 1);
        server->id[MAX_USERNAME_LEN - 1] = '\0';
    }

    cJSON *ssh_key_json = cJSON_GetObjectItemCaseSensitive(server_json, "ssh-key");
    if (ssh_key_json && cJSON_IsString(ssh_key_json))
    {
        strncpy(server->ssh_key, ssh_key_json->valuestring, MAX_SSH_KEY_LEN - 1);
        server->ssh_key[MAX_SSH_KEY_LEN - 1] = '\0';
    }
}

void free_folder(Folder *folder)
{
    if (!folder)
    {
        return;
    }

    for (int i = 0; i < folder->folder_count; ++i)
    {
        free_folder(&folder->folders[i]);
    }
    free(folder->folders);

    free(folder->servers);
}

void free_user(User *user)
{
    if (!user)
    {
        return;
    }

    for (int i = 0; i < user->folder_count; ++i)
    {
        free_folder(&user->folders[i]);
    }
    free(user->folders);
}

void free_config(Config *config)
{
    if (!config)
    {
        return;
    }

    for (int i = 0; i < config->user_count; ++i)
    {
        free_user(&config->users[i]);
    }
    free(config->users);
    free(config);
}