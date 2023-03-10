#include "../inc/server.h"

void *client_handler(void *arg) {
    int client_socket = *(int*)arg;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    read(client_socket, buffer, sizeof(buffer));
    printf("Received message from client (raw): %s\n", buffer);

    // Parse JSON message
    json_object *json_received_obj = json_tokener_parse(buffer);
    json_object *method_obj;
    json_object_object_get_ex(json_received_obj, "method", &method_obj);
    char *method = (char*)json_object_get_string(method_obj);
    
    // Handle login method
    if (strcmp(json_object_get_string(method_obj), "login") == 0) {
        // Check if username and password fields exist
        json_object *username_obj, *password_obj;
        if (json_object_object_get_ex(json_received_obj, "username", &username_obj) && json_object_object_get_ex(json_received_obj, "password", &password_obj)) {
            // TODO: authenticate user and send response confirmed or rejected
                json_object *response = json_object_new_object();
                json_object_object_add(response, "method", json_object_new_string("login"));
                json_object_object_add(response, "status", json_object_new_string("CONFIRMED"));
                const char *response_str = json_object_to_json_string(response);
                write(client_socket, response_str, strlen(response_str));
            // TODO: if auth false send response with status "REJECTED"
        }
    }
    else {          // TODO: implement more conditions with other methods

        // Unknown method
        json_object *response = json_object_new_object();
        json_object_object_add(response, "method", json_object_new_string("error"));
        json_object_object_add(response, "err", json_object_new_string("Unknown method"));
        const char *response_str = json_object_to_json_string(response);
        write(client_socket, response_str, strlen(response_str));
    }

    // Free memory and close socket
    json_object_put(json_received_obj);
    close(client_socket);
    return NULL;
}
