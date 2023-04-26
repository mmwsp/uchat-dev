
#include "../inc/client.h"

void search(int client_socket) {
    struct json_object* json_obj = json_object_new_object();
       json_object_object_add(json_obj, "method", json_object_new_string("search"));
    json_object_object_add(json_obj, "search_string", json_object_new_string("asaas"));

       // Send JSON object to server
       if (send_json_object(client_socket, json_obj) == -1) {
           exit(EXIT_FAILURE);
       }

    json_object_put(json_obj);
}
