#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <json-c/json.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

extern bool logged_in;

typedef struct {
    int user_id;
    char* login;
} UserInfo;

extern UserInfo current_user;

//JSON
int send_json_object(int client_socket, struct json_object* json_obj);
int receive_json_object(int client_socket, struct json_object** json_obj);
void parse_message(char* buffer);
//auth
void login(int client_socket);
void init_user(int client_socket);
void registration(int client_socket);
//
void init_private_chat(int client_socket);
void send_common_message(int client_socket);
void* listen_for_messages(void* arg);
