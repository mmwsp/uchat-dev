#include "../inc/uchat.h"
#include "../inc/ui.h"


t_user *mx_sign_in_up_request(char *login, char *name, char *password, char *repeat_password) {
    t_user *user1 = (t_user*)malloc(sizeof(t_user));
    pthread_mutex_lock(&socket_mutex);

    if (login != NULL && name == NULL && password != NULL && repeat_password == NULL) {
        if (send_json_object(app_params.client_socket, mx_client_request_login(login, password)) == -1) { 
            mx_show_notification("Error: wrong input data.\n", 5);
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        else {
            struct json_object *json_received;
            if (receive_json_object(app_params.client_socket, &json_received) != 0) {
                fprintf(stderr, "Failed to receive JSON message from server\n");
                pthread_mutex_unlock(&socket_mutex);
                return NULL;
            }

            char *buffer = (char *)json_object_to_json_string(json_received);
            parse_message(buffer, user1, NULL, NULL, NULL, NULL);
        }
        pthread_mutex_unlock(&socket_mutex);
        return user1;
    }
    else if (login != NULL && name != NULL && password != NULL && repeat_password != NULL) {
        if (strcmp(password, repeat_password) != 0) {
            mx_show_notification("Error: wrong input data.\n", 5);
            return NULL;
        }

        char* json_str = (char*)json_object_to_json_string(mx_client_request_registration(login, name, password));
        if (send(app_params.client_socket, json_str, strlen(json_str), 0) == -1) {

            mx_show_notification("Error: wrong input data.\n", 5);
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        else {
            struct json_object *json_received;
            if (receive_json_object(app_params.client_socket, &json_received) != 0) {
                fprintf(stderr, "Failed to receive JSON message from server\n");
                pthread_mutex_unlock(&socket_mutex);
                return NULL;
            }

            char *buffer = (char *)json_object_to_json_string(json_received);
            parse_message(buffer, user1, NULL, NULL, NULL, NULL);
        }
        pthread_mutex_unlock(&socket_mutex);
        return user1;
    }

    mx_show_notification("Error: wrong input data.\n", 5);
    pthread_mutex_unlock(&socket_mutex);
    return NULL;
}

bool mx_chat_request(t_chat *chat) {
    pthread_mutex_lock(&socket_mutex);
    if (send_json_object(app_params.client_socket, mx_client_request_get_chat_messages(chat->c_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }

        char *buffer = (char *)json_object_to_json_string(json_received);
        parse_message(buffer, NULL, NULL, chat, NULL, NULL);
    }

    pthread_mutex_unlock(&socket_mutex);
    return true;
}

t_user *mx_get_user_request(int u_id) {
    pthread_mutex_lock(&socket_mutex);
    t_user *user1 = (t_user*)malloc(sizeof(t_user));
    if (send_json_object(app_params.client_socket, mx_client_request_get_user_profile(u_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return NULL;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }

        char *buffer = (char *)json_object_to_json_string(json_received);
        parse_message(buffer, user1, NULL, NULL, NULL, NULL);
    }

    pthread_mutex_unlock(&socket_mutex);
    return user1;
}

t_chat *mx_get_chat_request(int c_id) {
    t_chat *chat1 = (t_chat*)malloc(sizeof(t_chat));
    pthread_mutex_lock(&socket_mutex);

    if (send_json_object(app_params.client_socket, mx_client_request_get_chat_id(c_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return NULL;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        parse_message(buffer, NULL, NULL, chat1, NULL, NULL);
        pthread_mutex_unlock(&socket_mutex);
        return chat1;
    }

    pthread_mutex_unlock(&socket_mutex);
    return NULL;
}

t_chat *mx_join_chat(int u_id, int c_id, int c_type) {
    t_chat *chat1 = (t_chat*)malloc(sizeof(t_chat));
    pthread_mutex_lock(&socket_mutex);

    if (send_json_object(app_params.client_socket, mx_client_request_join_chat(u_id, c_id, c_type)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return NULL;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        parse_message(buffer, NULL, NULL, chat1, NULL, NULL);
        pthread_mutex_unlock(&socket_mutex);
       return chat1;
    }

    pthread_mutex_unlock(&socket_mutex);
    return NULL;
}

t_chat **mx_search_chat(char *name) {
    t_chat ** chats = (t_chat**)malloc(sizeof(t_chat*) * 2);
    t_chat *chat1 = (t_chat*)malloc(sizeof(t_chat));

    pthread_mutex_lock(&socket_mutex);

    if (send_json_object(app_params.client_socket, mx_client_request_search(name)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return NULL;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&socket_mutex);
        char *buffer = (char *)json_object_to_json_string(json_received);

        parse_message(buffer, NULL, NULL, chat1, NULL, NULL);

        chats[0] = chat1;
        chats[1] = NULL;

        return chats;
    }

    pthread_mutex_unlock(&socket_mutex);
    return NULL;
}

t_message *mx_send_message(t_message *message) {
    pthread_mutex_lock(&socket_mutex);

    if (send_json_object(app_params.client_socket, 
        mx_client_request_message(message->m_type, message->u_id, message->c_id, message->m_text, message->m_form)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return NULL;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        parse_message(buffer, NULL, NULL, NULL, message, NULL);
        pthread_mutex_unlock(&socket_mutex);
        return message;
    }

    pthread_mutex_unlock(&socket_mutex);
    return NULL;
}

bool mx_delete_message_request(t_message *message) {
    pthread_mutex_lock(&socket_mutex);
    if (send_json_object(app_params.client_socket, 
        mx_client_request_delete_message(message->c_id, message->m_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");\
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        parse_message(buffer, NULL, NULL, NULL, message, NULL);
    }

    pthread_mutex_unlock(&socket_mutex);
    return true;
}

bool mx_change_user_data_request(int user_id, char *new_name, char *new_bio) {
    pthread_mutex_lock(&socket_mutex);
    if (send_json_object(app_params.client_socket, 
        mx_client_request_change_user(user_id, new_name, new_bio)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {

    }
    
    pthread_mutex_unlock(&socket_mutex);
    return true;
}

t_chat *mx_request_add_group(char *c_name, char *c_desc, int u_id) {
    t_chat *return_chat = (t_chat*)malloc(sizeof(t_chat));
    pthread_mutex_lock(&socket_mutex);

    if (send_json_object(app_params.client_socket, 
        mx_client_request_add_group(c_name, c_desc, u_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return NULL;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return NULL;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        parse_message(buffer, NULL, NULL, return_chat, NULL, NULL);
    }

    pthread_mutex_unlock(&socket_mutex);
    return return_chat;
}

bool mx_request_leave_chat(int chat_id, int user_id) {
    pthread_mutex_lock(&socket_mutex);
    if (send_json_object(app_params.client_socket, 
        mx_client_request_leave_chat(chat_id, user_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }

        char *buffer = (char *)json_object_to_json_string(json_received);

        struct json_object* jobj;
        enum json_tokener_error jerr = json_tokener_success;

        enum json_type jtype = json_object_get_type(jobj);
        if (jtype != json_type_object) {
            fprintf(stderr, "Expected JSON object, received %s\n", json_type_to_name(jtype));
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }

        jobj = json_tokener_parse_verbose(buffer, &jerr);
        if (jerr != json_tokener_success) {
            fprintf(stderr, "Error parsing JSON message: %s\n", json_tokener_error_desc(jerr));
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }


        struct json_object* jmethod;
        json_object_object_get_ex(jobj, "method", &jmethod);
        const char* method = json_object_get_string(jmethod);

        if (strcmp(method, "leave_chat") == 0) {
            struct json_object *jstatus;
            json_object_object_get_ex(jobj, "status", &jstatus);
            char *status = (char *)json_object_get_string(jstatus);

            if (strcmp(status, "FAIL") == 0) {
                pthread_mutex_unlock(&socket_mutex);
                return false;
            }

            if (strcmp(status, "OK") == 0) {
                pthread_mutex_unlock(&socket_mutex);
                return true;
            }
        }
    }

    pthread_mutex_unlock(&socket_mutex);
    return false;    
}

bool mx_del_chat_request(int c_id) {
    pthread_mutex_lock(&socket_mutex);
    if (send_json_object(app_params.client_socket, 
        mx_client_request_delete_chat(c_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {
   
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        struct json_object* jobj;
        enum json_tokener_error jerr = json_tokener_success;

        jobj = json_tokener_parse_verbose(buffer, &jerr);
        if (jerr != json_tokener_success) {
            fprintf(stderr, "Error parsing JSON message: %s\n", json_tokener_error_desc(jerr));
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }

        enum json_type jtype = json_object_get_type(jobj);
        if (jtype != json_type_object) {
            fprintf(stderr, "Expected JSON object, received %s\n", json_type_to_name(jtype));
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }

    
        struct json_object* jmethod;
        json_object_object_get_ex(jobj, "method", &jmethod);
     
        const char* method = json_object_get_string(jmethod);
        printf("Received message with method: %s\n", method);

        if (strcmp(method, "chat_delete") == 0) {
            struct json_object* jchat_id;
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            int chat_id = json_object_get_int(jchat_id);
            
            printf("\n ====CHAT DELETED : %i ==== \n", chat_id);
            pthread_mutex_unlock(&socket_mutex);
            return true;
        }

    }

    pthread_mutex_unlock(&socket_mutex);
    return false;
}

bool mx_del_user_request(int u_id) {
    pthread_mutex_lock(&socket_mutex);
    if (send_json_object(app_params.client_socket, 
        mx_client_request_delete_user(u_id)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {
        pthread_mutex_unlock(&socket_mutex);
        return true;
    }
    
    pthread_mutex_unlock(&socket_mutex);
    return false;
}

bool mx_edit_chat_request(int c_id, char *new_name, char *new_desc) {
    t_chat *temp_chat = (t_chat*)malloc(sizeof(t_chat));
    pthread_mutex_lock(&socket_mutex);

    if (send_json_object(app_params.client_socket, 
        mx_client_request_change_chat(c_id, new_name, new_desc)) == -1) {
        mx_show_notification("Error: wrong input data.\n", 5);
        pthread_mutex_unlock(&socket_mutex);
        return false;
    }
    else {
        struct json_object *json_received;
        if (receive_json_object(app_params.client_socket, &json_received) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server\n");
            pthread_mutex_unlock(&socket_mutex);
            return false;
        }
        
        char *buffer = (char *)json_object_to_json_string(json_received);

        temp_chat->c_id = 0;

        parse_message(buffer, NULL, NULL, temp_chat, NULL, NULL);

        if (temp_chat->c_id != 0) {
            pthread_mutex_unlock(&socket_mutex);
            return true;
        }
    }

    pthread_mutex_unlock(&socket_mutex);
    return false;
}


