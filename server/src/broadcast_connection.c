#include "../inc/server.h"

void broadcast_connection(struct client *client_ptr, int chat_id, char* message_text) {
    // Construct the JSON message
    struct json_object *json_obj = json_object_new_object();
    json_object_object_add(json_obj, "method", json_object_new_string("message"));
    json_object_object_add(json_obj, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(json_obj, "text", json_object_new_string(message_text));

    const char *json_str = json_object_to_json_string(json_obj);

    // Send the JSON message to the client
    size_t len = strlen(json_str);

    if (send(client_ptr->socket_fd, json_str, len, 0) != len) {
        perror("Error sending message");
    }

    json_object_put(json_obj);
}
