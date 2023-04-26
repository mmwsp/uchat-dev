#include "../inc/server.h"

void init_user(int socket_fd, json_object *json_received_obj) {
    struct json_object *login_obj;
    json_object_object_get_ex(json_received_obj, "login", &login_obj);
    char *login = (char*)json_object_get_string(login_obj);
    int user_id = get_user_id_from_db(login); // Perform database query to retrieve user ID
    // Construct the JSON object
    struct json_object *json_response_obj = json_object_new_object();
    json_object_object_add(json_response_obj, "method", json_object_new_string("user_init"));
    json_object_object_add(json_response_obj, "login", json_object_new_string(login));
    json_object_object_add(json_response_obj, "user_id", json_object_new_int(user_id));

    const char *json_response_str = json_object_to_json_string(json_response_obj);
    send(socket_fd, json_response_str, strlen(json_response_str), 0);

    json_object_put(json_response_obj);
    
    if (user_id != -1) {
        add_to_list(user_id, socket_fd);
    } else {
        plain_error(socket_fd);
    }
}
