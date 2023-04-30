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
#include <spawn.h>
#include <sys/stat.h>


#define LIST_SIZE 100
//structs

struct list_node {
    int user_id;
    int socket_fd;
    struct list_node *next;
};


//list
int list_function(int user_id);
void add_to_list(int user_id, int socket_fd);
void remove_from_list(int user_id);
int get_socket_fd_from_list(int user_id);
//
void mx_deamon_start(void);
void sigint_handler(int n);
void change_user(int socket_fd, json_object *json_received_obj);
void get_user_profile(int socket_fd, json_object *json_received_obj);
void login(int socket_fd, json_object *json_received_obj);
void registration(int socket_fd, json_object *json_received_obj);
void init_user(int socket_fd, json_object *json_received_obj);
struct json_object* retrieve_user_chats(int user_id);
void delete_user(json_object *json_received_obj);
void *client_handler(void *arg);
//chat
void add_group(int socket_fd, json_object *json_received_obj);
void leave_chat(int socket_fd, json_object *json_received_obj);
void search(int socket_fd, json_object *json_received_obj);
void join_chat(int socket_fd, json_object *json_received_obj);
void chat_delete(int socket_fd, json_object *json_received_obj);
void get_chat_users(int socket_fd, json_object *json_received_obj);
void change_chat(json_object *json_received_obj);
void init_chat(json_object *json_received_obj);
void private_chat(json_object *json_received_obj);
//message
void message_handler(json_object *json_received_obj);
void get_chat_messages(int socket_fd, json_object *json_received_obj);
void delete_message(int socket_fd, json_object *json_received_obj);
void edit_message(json_object *json_received_obj);
void common_message(json_object *json_received_obj);
void broadcast_connection(struct json_object *json_received_obj, struct json_object *json_message);

// errors
void login_error(int client_socket, int mtd);
void password_error(int client_socket, int mtd);
void server_error(int client_socket);
void unknown_method_error(int client_socket);
void plain_error(int client_socket);
//db
int insert_user_into_chat(int chat_id, int user_id);
int get_user_id_from_db(char *login);
int delete_chat_from_db(int c_id);
int delete_message_from_db(int message_id);
//time
char* format_time(long int timestamp);
long int get_current_time(void);
