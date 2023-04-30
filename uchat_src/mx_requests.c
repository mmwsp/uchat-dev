#include "../inc/uchat.h"

//                          REQUESTS

// TODO: Don't forget about json_object_put() for free() memory (???)

// Creating request 'login' 
// return - 'request' in JSON object
json_object *mx_client_request_login(char *login, char *password) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("login"));
    json_object_object_add(request, "username", json_object_new_string(login));
    json_object_object_add(request, "password", json_object_new_string(password));
    return request;
}

// Creating request 'registration'
// return - 'request' in JSON object
json_object *mx_client_request_registration(char *login, char *username, char *password) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("registration"));
    json_object_object_add(request, "login", json_object_new_string(login));
    json_object_object_add(request, "name", json_object_new_string(username));
    json_object_object_add(request, "password", json_object_new_string(password));
    return request;
}

// Creating request 'user_init'
// return - 'request' in JSON object
json_object *mx_client_request_user_init(char *login) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("user_init"));
    json_object_object_add(request, "login", json_object_new_string(login));
    return request;
}

// Creating request 'chat_init'
// return - 'request' in JSON object
json_object *mx_client_request_chat_init(int chat_type, char *user_id1, char *user_id2) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("chat_init"));
    json_object_object_add(request, "chat_type", json_object_new_int(chat_type));
    json_object_object_add(request, "user_id1", json_object_new_string(user_id1));
    json_object_object_add(request, "user_id2", json_object_new_string(user_id2));
    return request;
}

// Creating request 'load_chat'
// return - 'request' in JSON object
json_object *mx_client_request_load_chat(int chat_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("load_chat"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    return request;
}

// Creating request 'chat_delete'
// return - 'request' in JSON object
json_object *mx_client_request_delete_chat(int chat_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("chat_delete"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    return request;
}

// Creating request 'get_chat_id' 
// return - 'request' in JSON object
json_object *mx_client_request_get_chat_id(int chat_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("get_chat_id"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    return request;
}

// Creating request 'change_chat' - name and descroption
// return - 'request' in JSON object
json_object *mx_client_request_change_chat(int chat_id, char *new_name, char *new_desc) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("change_chat"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(request, "new_name", json_object_new_string(new_name));
    json_object_object_add(request, "new_desc", json_object_new_string(new_desc));
    return request;
}

// Creating request 'message'
// return - 'request' in JSON object
json_object *mx_client_request_message(int message_type, int user_id, int chat_id, char *message_text, int message_form) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("message"));
    json_object_object_add(request, "message_type", json_object_new_int(message_type));
    json_object_object_add(request, "user_id", json_object_new_int(user_id));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(request, "message_text", json_object_new_string(message_text));
    json_object_object_add(request, "message_form", json_object_new_int(message_form));
    return request;
}

// Creating request 'edit_message'
// return - 'request' in JSON object
json_object *mx_client_request_edit_message(int chat_id, int message_id, char *edited_text) { //maybe need to change params??
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("edit_message"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(request, "message_id", json_object_new_int(message_id));
    json_object_object_add(request, "edited_text", json_object_new_string(edited_text));
    return request;
}

// Creating request 'delete_message'
// return - 'request' in JSON object
json_object *mx_client_request_delete_message(int chat_id, int message_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("delete_message"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(request, "message_id", json_object_new_int(message_id));
    return request;
}

// Creating request 'load_profile'
// return - 'request' in JSON object
json_object *mx_client_request_load_profile(int user_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("del_chat"));
    json_object_object_add(request, "user_id", json_object_new_int(user_id));
    return request;
}

// Creating request 'get_user_profile' to get information of user profile.
// return - 'request' in JSON object
json_object *mx_client_request_get_user_profile(int user_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("get_user_profile"));
    json_object_object_add(request, "user_id", json_object_new_int(user_id));
    return request;
}

// Creating request 'get_chat_users' to get all users in chat by chat_id.
// return - 'request' in JSON object
json_object *mx_client_request_get_chat_users(int chat_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("get_chat_users"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    return request;
}

// Creating request 'get_chat_messages' to get all messages (array of messages) in chat by chat_id.
// return - 'request' in JSON object
json_object *mx_client_request_get_chat_messages(int chat_id) {
printf(" - - - - -chat_id: %i\n", chat_id);
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("get_chat_messages"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    return request;
}

// return - 'request' in JSON object
json_object *mx_client_request_change_user(int user_id, char *new_name, char* new_bio) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("change_user"));
    json_object_object_add(request, "user_id", json_object_new_int(user_id));
    json_object_object_add(request, "new_name", json_object_new_string(new_name));
    json_object_object_add(request, "new_bio", json_object_new_string(new_bio));
    return request;
}

// Creating request 'delete_user'
// return - 'request' in JSON object
json_object *mx_client_request_delete_user(int user_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("delete_user"));
    json_object_object_add(request, "user_id", json_object_new_int(user_id));
    return request;
}

// Creating request 'join_chat'
// return - 'request' in JSON object
json_object *mx_client_request_join_chat(int u_id, int c_id, int c_type) {
    json_object *request = json_object_new_object();
    
    json_object_object_add(request, "method", json_object_new_string("join_chat"));
    json_object_object_add(request, "sender_id", json_object_new_int(u_id));
    if (c_type == 1) {
        json_object_object_add(request, "user2_id", json_object_new_int(c_id));
    }
    else if (c_type == 2) {
        json_object_object_add(request, "chat_id", json_object_new_int(c_id));
    }
    json_object_object_add(request, "c_type", json_object_new_int(c_type));
    
    return request;
}

json_object *mx_client_request_search(char *c_name) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("search"));
    json_object_object_add(request, "search_string", json_object_new_string(c_name));
    return request;
}

// Creating request 'add_group'
// return - 'request' in JSON object
json_object *mx_client_request_add_group(char *c_name, char *c_desc, int u_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("add_group"));
    json_object_object_add(request, "c_name", json_object_new_string(c_name));
    json_object_object_add(request, "c_desc", json_object_new_string(c_desc));
    json_object_object_add(request, "user_id", json_object_new_int(u_id));
    return request;
}

// Creating request 'leave_chat'
// return - 'request' in JSON object
json_object *mx_client_request_leave_chat(int chat_id, int user_id) {
    json_object *request = json_object_new_object();
    json_object_object_add(request, "method", json_object_new_string("leave_chat"));
    json_object_object_add(request, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(request, "user_id", json_object_new_int(user_id));
    return request;
}




