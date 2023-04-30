#include "../inc/server.h"

void delete_message(int socket_fd, json_object *json_received_obj) {
    struct json_object *jchat_id, *jmsg_id;
    json_object_object_get_ex(json_received_obj, "chat_id", &jchat_id);
    json_object_object_get_ex(json_received_obj, "message_id", &jmsg_id);
    
    int chat_id = json_object_get_int(jchat_id);
    int message_id = json_object_get_int(jmsg_id);
    
    if (delete_message_from_db(message_id) == -1) {
        plain_error(socket_fd);
        printf("error deleting msg from db.");
    } else {
        struct json_object *json_obj = json_object_new_object();
        json_object_object_add(json_obj, "method", json_object_new_string("delete_message"));
        json_object_object_add(json_obj, "chat_id", json_object_new_int(chat_id));
        json_object_object_add(json_obj, "message_id", json_object_new_int(message_id));
        
        broadcast_connection(json_received_obj, json_obj);
    }
}
