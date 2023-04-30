#include "../inc/server.h"

void login_error(int client_socket, int mtd) {
    json_object *response = json_object_new_object();
    if(mtd == 0) {
        json_object_object_add(response, "method", json_object_new_string("login"));
    } else if (mtd == 1){
        json_object_object_add(response, "method", json_object_new_string("registration"));
    
    }
    
    json_object_object_add(response, "status", json_object_new_string("FAIL"));
    json_object_object_add(response, "message", json_object_new_string("invalid login"));
    const char *response_str = json_object_to_json_string(response);
    send(client_socket, response_str, strlen(response_str), 0);
}

void password_error(int client_socket, int mtd) {
    json_object *response = json_object_new_object();
    if(mtd == 0) {
        json_object_object_add(response, "method", json_object_new_string("login"));
    } else if (mtd == 1){
        json_object_object_add(response, "method", json_object_new_string("registration"));
    }
    json_object_object_add(response, "status", json_object_new_string("FAIL"));
    json_object_object_add(response, "message", json_object_new_string("invalid password"));
    const char *response_str = json_object_to_json_string(response);
    send(client_socket, response_str, strlen(response_str), 0);
}

void server_error(int client_socket) {
    json_object *response = json_object_new_object();
    json_object_object_add(response, "method", json_object_new_string("login"));
    json_object_object_add(response, "status", json_object_new_string("FAIL"));
    json_object_object_add(response, "message", json_object_new_string("server error"));
    const char *response_str = json_object_to_json_string(response);
    send(client_socket, response_str, strlen(response_str), 0);
}

void unknown_method_error(int client_socket) {
    json_object *response = json_object_new_object();
    json_object_object_add(response, "method", json_object_new_string("error"));
    json_object_object_add(response, "err", json_object_new_string("Unknown method"));
    const char *response_str = json_object_to_json_string(response);
    send(client_socket, response_str, strlen(response_str), 0);
}

void plain_error(int client_socket) {
    json_object *response = json_object_new_object();
    json_object_object_add(response, "method", json_object_new_string("error"));
    json_object_object_add(response, "err", json_object_new_string("server error"));
    const char *response_str = json_object_to_json_string(response);
    send(client_socket, response_str, strlen(response_str), 0);
}
