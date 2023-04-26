#include "../inc/server.h"

void message_handler(int client_socket, json_object *json_received_obj) {
    // Parse the chat ID and message text from the JSON
    struct json_object *message_type_obj;
    json_object_object_get_ex(json_received_obj, "message_type", &message_type_obj);
    int message_type = json_object_get_int(message_type_obj);
    if (message_type == 1) {
        common_message(client_socket, json_received_obj);
    }
    if (message_type == 3) {
        file_message(client_socket, json_received_obj);
    }
}

