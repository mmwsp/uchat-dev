#include "../inc/client.h"

//send json to server
int send_json_object(int client_socket, struct json_object* json_obj) {
    char* json_str = (char*)json_object_to_json_string(json_obj);
    if (send(client_socket, json_str, strlen(json_str), 0) == -1) {
        perror("Error sending JSON object to server");
        return -1;
    }
    printf("\n sent msg: %s \n", json_str);
    return 0;
}

int receive_json_object(int socket_fd, struct json_object **json_obj) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = read(socket_fd, buffer, sizeof(buffer));
    if (bytes_received <= 0) {
        return -1;
    }

    *json_obj = json_tokener_parse(buffer);
    return 0;
}


// parse received message
void parse_message(char* buffer) {
    struct json_object* jobj;
    enum json_tokener_error jerr = json_tokener_success;

    jobj = json_tokener_parse_verbose(buffer, &jerr);
    if (jerr != json_tokener_success) {
        fprintf(stderr, "Error parsing JSON message: %s\n", json_tokener_error_desc(jerr));
        return;
    }

    //check type of receiving data
    enum json_type jtype = json_object_get_type(jobj);
    if (jtype != json_type_object) {
        fprintf(stderr, "Expected JSON object, received %s\n", json_type_to_name(jtype));
        return;
    }

    //take method from json
    struct json_object* jmethod;
    if (!json_object_object_get_ex(jobj, "method", &jmethod)) {
        fprintf(stderr, "Missing 'method' field in JSON object\n");
        return;
    }

    //value of method put in string
    const char* method = json_object_get_string(jmethod);
    printf("Received message with method: %s\n", method);

    /* Handle the message from server based on the method.
       Handle login response*/
    if (strcmp(method, "login") == 0) {
        struct json_object* jstatus;
        struct json_object* jmessage;
        if (!json_object_object_get_ex(jobj, "status", &jstatus)) {
            fprintf(stderr, "Missing status field in login message\n");
            return;
        }

        const char* status = json_object_get_string(jstatus);
        printf("Received login-confirmation status: %s \n", status);
        if(status && strcmp(status, "FAIL") == 0) {
            json_object_object_get_ex(jobj, "message", &jmessage);
            const char* message = json_object_get_string(jmessage);
            printf("Received status-message: %s \n", message);
        }
        if(status && strcmp(status, "OK") == 0) {
            logged_in = true;
        }

    } //handle registration response from server
    else if(strcmp(method, "registration") == 0) {
        struct json_object* jstatus;
        struct json_object* jmessage;
        if (!json_object_object_get_ex(jobj, "status", &jstatus)) {
            fprintf(stderr, "Missing status field in registration message\n");
            return;
        }
        if (json_object_get_type(jstatus) != json_type_string) {
            fprintf(stderr, "Expected 'status' field to be strings\n");
            return;
        }

        const char* status = json_object_get_string(jstatus);
        printf("\n Received registration-confirmation status: %s \n", status);
        if(status && strcmp(status, "FAIL") == 0) {
            json_object_object_get_ex(jobj, "message", &jmessage);
            const char* message = json_object_get_string(jmessage);
            printf("Received status-message: %s \n", message);
        }
    }//Response from init_user method. We need to save user`s credentials. login and id.
    else if (strcmp(method, "user_init") == 0) {
        struct json_object* jlogin;
        struct json_object* juser_id;
        json_object_object_get_ex(jobj, "login", &jlogin);
        json_object_object_get_ex(jobj, "user_id", &juser_id);
        
        const char* login = json_object_get_string(jlogin);
        int user_id = json_object_get_int(juser_id);
        printf("uid: %i", user_id);
        current_user.user_id = user_id;
        current_user.login = strdup(login);
    }
    //Handle chat_init response from server
    else if (strcmp(method, "chat_init") == 0) {
        struct json_object* jchat_type;
        struct json_object* jchat_id;
        struct json_object* juser1_id;
        struct json_object* juser2_id;
        struct json_object* jjoin_type;
        json_object_object_get_ex(jobj, "chat_type", &jchat_type);
        json_object_object_get_ex(jobj, "user2_id", &juser2_id);
        json_object_object_get_ex(jobj, "user1_id", &juser1_id);
        json_object_object_get_ex(jobj, "join_type", &jjoin_type);
        int chat_type = json_object_get_int(jchat_type);
        int chat_id = json_object_get_int(jchat_id);
        int user1_id = json_object_get_int(juser1_id);
        int user2_id = json_object_get_int(juser2_id);
        int join_type = json_object_get_int(jjoin_type);
        //may be save chat data in struct ?
        //here may be called function for render
    }
    else if (strcmp(method, "message") == 0) {
        char *message_text;
        struct json_object* jmessage_type;
        struct json_object* jmessage_id;
        struct json_object* jchat_id;
        struct json_object* juser_id;
        struct json_object* jmessage_form;
        struct json_object* jmessage_date;
        struct json_object* jmessage_is_edited;
        json_object_object_get_ex(jobj, "message_type", &jmessage_type);
        json_object_object_get_ex(jobj, "message_id", &jmessage_id);
        json_object_object_get_ex(jobj, "user_id", &juser_id);
        json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        json_object_object_get_ex(jobj, "message_form", &jmessage_form);
        json_object_object_get_ex(jobj, "message_date", &jmessage_date);
        json_object_object_get_ex(jobj, "is_edited", &jmessage_is_edited);
        
        int message_type = json_object_get_int(jmessage_type);
        int message_id = json_object_get_int(jmessage_id);
        int user_id = json_object_get_int(juser_id);
        int chat_id = json_object_get_int(jchat_id);
        int message_form = json_object_get_int(jmessage_form);
        char *message_date = json_object_get_string(jmessage_date);
        int is_edited = json_object_get_int(jmessage_is_edited);
        //may be all message data needs to be saved somewhere
        
        
        if(message_type == 1) {   // Condition for handling type 1(common message)
            struct json_object* jmessage_text; //get text message
            json_object_object_get_ex(jobj, "message_text", &jmessage_text);
            message_text = json_object_get_string(jmessage_text);
            //some client logic for rendering common msg?
        }
        
        if(message_type == 3) {   // Condition for handling type 3(file message)
            struct json_object* jmessage_file_id;
            json_object_object_get_ex(jobj, "message_file_id", &jmessage_file_id);
            struct json_object* jmessage_file;
            json_object_object_get_ex(jobj, "message_file", &jmessage_file);
            struct json_object* j_file_name;
            json_object_object_get_ex(jobj, "file_name", &j_file_name);
            struct json_object* j_file_size;
            json_object_object_get_ex(jobj, "file_size", &j_file_size);
            
            int message_file_id = json_object_get_int(jmessage_file_id);
            char *file_name = json_object_get_string(j_file_name);
            char *message_file = json_object_get_string(jmessage_file);
            int file_size = json_object_get_int(j_file_size);
            printf("\n FILE MESSAGE ID (ffrom db): %i . ", message_file_id);
            int result = decodeFile(message_file, file_name);
            if (result == 0) {
                printf("File encoded successfully.\n");
            } else {
                printf("Failed to encode file.\n");
            }
        }
        
        //checking receiving message (testing)
        printf("\n-----------------\n");
        printf("User: %i \n", user_id);
        if(message_text) {
            printf("%s \n", message_text);
        }
        printf("%s", message_date);
        printf("edited - %i", is_edited);
        printf("\n-----------------\n");
        printf("Chat ID: %i", chat_id);
        
    }
    if (strcmp(method, "chat_delete") == 0) {
        struct json_object* jchat_id;
        json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        int chat_id = json_object_get_int(jchat_id);
        printf("\n ====CHAT DELETED : %i ==== \n", chat_id);
    }
    if (strcmp(method, "search") == 0) {
        struct json_object* jctype;
        json_object_object_get_ex(jobj, "c_type", &jctype);
        int chat_id = json_object_get_int(jctype);
        printf("\n ====  %i ==== \n", chat_id);
    }
    
    json_object_put(jobj);
}
