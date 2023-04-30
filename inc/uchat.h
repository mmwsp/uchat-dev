#pragma once

// all
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>
#include <time.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

//
typedef struct s_app_params {
    int client_socket;
    char *ip;
    int port;
}               t_app_params;

//
typedef struct s_message_file {
    int mf_id;
    void* mf_file;
    char* mf_name;
    int mf_size;
}               t_message_file;
// 
typedef struct s_message {
    int m_id;
    int u_id;
    char* m_text;
    bool m_is_edited;
    int m_date;

    int m_type;

    int m_form;
    int m_m_id;
    int c_id;
    
    t_message_file **m_filearr;
    int m_filearr_count;
    int m_filearr_size;
}               t_message;
// 
typedef struct s_chat {
    int c_id;
    char *c_name;
    void* c_profile_image;
    char *c_description;

    int jt_type;
    int ct_type;
    int cus_type;

    t_message **c_messagearr;
    int c_messagearr_count;
    int c_messagearr_size;
}               t_chat;
// 
typedef struct s_user {
    int u_id;
    char *u_login;
    char *u_name;
    void* u_profile_image;
    char *u_bio;
    int u_online_date;

    t_chat **u_chatarr;
    int u_chatarr_count;
    int u_chatarr_size;
}               t_user;

extern t_user *user;
extern t_app_params app_params;

//Requests

json_object *mx_client_request_login(char *login, char *password);
json_object *mx_client_request_registration(char *login, char *username, char *password);
json_object *mx_client_request_user_init(char *login);
json_object *mx_client_request_message(int message_type, int user_id, int chat_id, char *message_text, int message_form);
json_object *mx_client_request_edit_message(int chat_id, int message_id, char *edited_text);
json_object *mx_client_request_delete_message(int chat_id, int message_id);
json_object *mx_client_request_chat_init(int chat_type, char *user_id1, char *user_id2);
json_object *mx_client_request_load_chat(int chat_id);
json_object *mx_client_request_delete_chat(int chat_id);
json_object *mx_client_request_get_chat_id(int chat_id);
json_object *mx_client_request_load_profile(int user_id);
json_object *mx_client_request_get_user_profile(int user_id);
json_object *mx_client_request_get_chat_messages(int chat_id);
json_object *mx_client_request_search(char *c_name);
json_object *mx_client_request_join_chat(int u_id, int c_id, int c_type);
json_object *mx_client_request_change_user(int user_id, char *new_name, char* new_bio);
json_object *mx_client_request_add_group(char *c_name, char *c_desc, int u_id);
json_object *mx_client_request_leave_chat(int chat_id, int user_id);
json_object *mx_client_request_delete_user(int user_id);
json_object *mx_client_request_change_chat(int chat_id, char *new_name, char *new_desc);

//Cryptographys


//Connection

int connect_to_server(t_app_params app_params);

//Functions

t_user *mx_sign_in_up_request(char *login, char *name, char *password, char *repeat_password);
t_user *mx_get_user_request(int u_id);
t_chat *mx_get_chat_request(int c_id);
t_chat *mx_join_chat(int u_id, int c_id, int c_type);
t_chat **mx_search_chat(char *name);
t_chat *mx_request_add_group(char *c_name, char *c_desc, int u_id);
t_message *mx_send_message(t_message *message);

bool mx_chat_request(t_chat *chat);
bool mx_delete_message_request(t_message *message);
bool mx_del_chat_request(int c_id);
bool mx_request_leave_chat(int chat_id, int user_id);
bool mx_change_user_data_request(int user_id, char *new_name, char *new_bio);
bool mx_del_user_request(int u_id);
bool mx_edit_chat_request(int c_id, char *new_name, char *new_desc);

//Utils
char* format_time(long int timestamp);
int check_password(char *password);
int check_login(char *login);
//char *get_method(char *buffer);
char *get_formated_date(char *full_date);
char **strsplit(const char *s, char c);
char *strnew(const int size);
char *strjoin(char const *s1, char const *s2);

//JSON

int send_json_object(int client_socket, struct json_object* json_obj);
//int send_json_object(int client_socket, char *buffer);
int receive_json_object(int client_socket, struct json_object** json_obj);
//void parse_message(char* buffer);
void parse_message(char* buffer, t_user *user1, t_user **users1, t_chat *chat1, t_message *message1, t_message **messages1);

