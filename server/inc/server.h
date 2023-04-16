#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <json-c/json.h>
#include <signal.h>
#include <netinet/in.h>
#include <sqlite3.h>

//structs

struct client {
    int socket_fd;
    struct client *next;
};

struct chat {
    int id;
    struct client *clients;
    struct chat *next;
};

extern struct chat *chat_list;

//
void sigint_handler(int sig);
void *client_handler(void *arg);
void broadcast_connection(struct client *client_ptr, int chat_id, char* message_text);
struct chat *find_chat_by_id(int chat_id);
void add_chat(int chat_id);
void delete_chat(int chat_id);
// errors
void login_error(int client_socket);
void password_error(int client_socket);
void server_error(int client_socket);
