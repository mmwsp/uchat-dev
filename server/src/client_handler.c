#include "../inc/server.h"

void *client_handler(void *arg) {
    printf("\n CLIENT HANDLER \n");
    int client_socket = *(int*)arg;
    char buffer[2048];
    memset(buffer, 0, sizeof(buffer));
    int mtd = 0;
    
    while(1) {
        int bytes_received = read(client_socket, buffer, sizeof(buffer));
        if (bytes_received <= 0) {
            perror("Error receiving message from client");
            break;
        }else if (bytes_received == 0) {
            printf("Connection closed by client\n");
            close(client_socket);
        }

        printf("Received message from client (raw): %s\n", buffer);
        
        // Parse JSON message
        json_object *json_received_obj = json_tokener_parse(buffer);
        json_object *method_obj;
        json_object_object_get_ex(json_received_obj, "method", &method_obj);
        char *method = (char*)json_object_get_string(method_obj);
        
        if (strcmp(method, "search") == 0) {
            search(client_socket, json_received_obj);
        }
        if (strcmp(method, "join_chat") == 0) {
            join_chat(client_socket, json_received_obj);
        }
        if (strcmp(method, "get_chat_messages") == 0) {
            get_chat_messages(client_socket, json_received_obj);
        }
        if (strcmp(method, "delete_message") == 0) {
            delete_message(client_socket, json_received_obj);
        }
        if (strcmp(method, "delete_user") == 0) {
            delete_user(client_socket, json_received_obj);
        }
        if (strcmp(method, "change_user") == 0) {
            change_user(client_socket, json_received_obj);
        }
        if (strcmp(method, "change_chat") == 0) {
            change_chat(client_socket, json_received_obj);
        }
        if (strcmp(method, "get_user_profile") == 0) {
            get_user_profile(client_socket, json_received_obj);
        }
        if (strcmp(method, "edit_message") == 0) {
            edit_message(client_socket, json_received_obj);
        }
        if (strcmp(method, "chat_delete") == 0) {
            chat_delete(client_socket, json_received_obj);
        }
        if (strcmp(method, "chat_init") == 0) {
            init_chat(client_socket, json_received_obj);
        }
        if (strcmp(json_object_get_string(method_obj), "registration") == 0) {
            registration(client_socket, json_received_obj);
        }
        if (strcmp(json_object_get_string(method_obj), "login") == 0) {
            login(client_socket, json_received_obj);
        }
        if (strcmp(json_object_get_string(method_obj), "message") == 0) {
            message_handler(client_socket, json_received_obj);
        }
        json_object_put(json_received_obj);
        memset(buffer, 0, sizeof(buffer));
    }

    close(client_socket);
    pthread_detach(pthread_self());
    return NULL;
}
