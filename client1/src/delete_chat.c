#include "../inc/client.h"

void delete_chat(int client_socket) {
    int chat_id = 4;
    struct json_object* json_obj = json_object_new_object();
       json_object_object_add(json_obj, "method", json_object_new_string("chat_delete"));
       json_object_object_add(json_obj, "chat_id", json_object_new_int(chat_id));

       // Send JSON object to server
       if (send_json_object(client_socket, json_obj) == -1) {
           exit(EXIT_FAILURE);
       }

    json_object_put(json_obj);
}

void add_group(int client_socket) {
    struct json_object *json_chat_obj = json_object_new_object();
    json_object_object_add(json_chat_obj, "method", json_object_new_string("add_group"));
    json_object_object_add(json_chat_obj, "user_id", json_object_new_int(5));
    json_object_object_add(json_chat_obj, "c_name", json_object_new_string("ZALUPA"));
    json_object_object_add(json_chat_obj, "c_desc", json_object_new_string("Chat dlya zhestkih hyliganov"));
    
    if (send_json_object(client_socket, json_chat_obj) == -1) {
        exit(EXIT_FAILURE);
    }
}
