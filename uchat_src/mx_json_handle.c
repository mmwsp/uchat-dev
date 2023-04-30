#include "../inc/uchat.h"

//send json to server
int send_json_object(int client_socket, struct json_object* json_obj) {
    char* json_str = (char*)json_object_to_json_string(json_obj);
    if (send(client_socket, json_str, strlen(json_str), 0) == -1) {
        perror("Error sending JSON object to server");
        return -1;
    }
    
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
void parse_message(char* buffer, t_user *user1, t_user **users1, t_chat *chat1, t_message *message1, t_message **messages1) {
    struct json_object* jobj;
    enum json_tokener_error jerr = json_tokener_success;

    if (users1 == NULL || messages1 == NULL) { }   

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
    json_object_object_get_ex(jobj, "method", &jmethod);
    //value of method put in string
    const char* method = json_object_get_string(jmethod);

    /* Handle the message from server based on the method.
       Handle login response*/
    if (strcmp(method, "login") == 0) {
        struct json_object* jstatus;
        struct json_object* jmessage;

        json_object_object_get_ex(jobj, "status", &jstatus);
        const char* status = json_object_get_string(jstatus);
        printf("Received login-confirmation status: %s \n", status);

        if(status && strcmp(status, "FAIL") == 0) {
            json_object_object_get_ex(jobj, "message", &jmessage);
            const char* message = json_object_get_string(jmessage);
            printf("Received status-message: %s \n", message);
            user1->u_id = 0;
        }
        else {
            if (user1 != NULL) {
                struct json_object *user_id;
                struct json_object *login;
                struct json_object *user_name;
                struct json_object *user_bio;
                struct json_object *profile_image;
                struct json_object *user_online_date;
                struct json_object *chats_array = NULL;

                json_object_object_get_ex(jobj, "u_id", &user_id);
                json_object_object_get_ex(jobj, "u_login", &login);
                json_object_object_get_ex(jobj, "u_name", &user_name);
                json_object_object_get_ex(jobj, "u_bio", &user_bio);
                json_object_object_get_ex(jobj, "u_profile_image", &profile_image);
                json_object_object_get_ex(jobj, "u_online_date", &user_online_date);
                json_object_object_get_ex(jobj, "chats", &chats_array);

                user1->u_id = json_object_get_int(user_id);
                user1->u_login = strdup((char *)json_object_get_string(login));
                user1->u_name = strdup((char *)json_object_get_string(user_name));
                user1->u_bio = strdup((char *)json_object_get_string(user_bio));
                user1->u_profile_image = NULL;
                user1->u_online_date = json_object_get_int(user_online_date); 
                user1->u_chatarr_count = json_object_array_length(chats_array);

                user1->u_chatarr = (t_chat**)malloc(sizeof(t_chat*) * (user1->u_chatarr_count + 50));
                user1->u_chatarr_size = (user1->u_chatarr_count + 50);
                if (user1->u_chatarr_count != 0) {
                    //Chats
                    for (int i = 0; i < user1->u_chatarr_count; i++) {
                        t_chat *chat_1 = (t_chat*)malloc(sizeof(t_chat));
                        struct json_object *chat = json_object_array_get_idx(chats_array, i);

                        struct json_object *ct_type;
                        json_object_object_get_ex(chat, "ct_id", &ct_type);
                        chat_1->ct_type = json_object_get_int(ct_type);
                        
                        if (chat_1->ct_type == 1) {
                            //struct json_object *juser_id;
                            struct json_object *jchat_id;
                            struct json_object *juser_name;
                            struct json_object *juser_bio;
                            struct json_object *juser_profile_img;
                            struct json_object *jjoin_type;
                            //struct json_object *jchat_type;
                            struct json_object *jcus_type;

                            //json_object_object_get_ex(chat, "u_id", &juser_id);
                            json_object_object_get_ex(chat, "c_id", &jchat_id);
                            json_object_object_get_ex(chat, "u_name", &juser_name);
                            json_object_object_get_ex(chat, "u_bio", &juser_bio);
                            json_object_object_get_ex(chat, "u_profile_img", &juser_profile_img);
                            json_object_object_get_ex(chat, "jt_id", &jjoin_type);
                            //json_object_object_get_ex(chat, "ct_id", &jchat_type);
                            json_object_object_get_ex(chat, "cus_type", &jcus_type); // ?

                            //int user_id = json_object_get_int(juser_id);
                            int chat_id = json_object_get_int(jchat_id);
                            char *user_name = strdup((char *)json_object_get_string(juser_name));
                            char *user_bio = strdup((char *)json_object_get_string(juser_bio));
                            char *user_profile_img = strdup((char *)json_object_get_string(juser_profile_img));
                            int join_type = json_object_get_int(jjoin_type);
                            //int c_type = json_object_get_int(jchat_id);
                            int cus_type = json_object_get_int(jcus_type);

                            chat_1->c_id = chat_id;
                            chat_1->c_name = user_name;
                            chat_1->c_description = user_bio;
                            chat_1->c_profile_image = user_profile_img;
                            chat_1->jt_type = join_type;
                            chat_1->cus_type = cus_type;
                        }
                        else {
                            struct json_object *chat_id;
                            struct json_object *chat_name;
                            struct json_object *chat_description;
                            struct json_object *profile_image;
                            struct json_object *jt_type;
                            struct json_object *cus_type;
                            
                            json_object_object_get_ex(chat, "c_id", &chat_id);
                            json_object_object_get_ex(chat, "c_name", &chat_name);
                            json_object_object_get_ex(chat, "c_description", &chat_description);
                            json_object_object_get_ex(chat, "c_profile_img", &profile_image);
                            json_object_object_get_ex(chat, "jt_id", &jt_type);
                            json_object_object_get_ex(chat, "cus_type", &cus_type);
                            
                            chat_1->c_id = json_object_get_int(chat_id);
                            chat_1->c_name = strdup((char *)json_object_get_string(chat_name));
                            chat_1->c_description = strdup((char *)json_object_get_string(chat_description));
                            chat_1->c_profile_image = NULL;
                            chat_1->jt_type = json_object_get_int(jt_type);
                            chat_1->cus_type = json_object_get_int(cus_type);
                        }

                        

                        user1->u_chatarr[i] = chat_1;
                        
                    }
                }
            }
        }
    } //handle registration response from server
    else if(strcmp(method, "registration") == 0) {
        struct json_object* jstatus;
        struct json_object* jmessage;
        json_object_object_get_ex(jobj, "status", &jstatus);
        const char* status = json_object_get_string(jstatus);
        

        if(status && strcmp(status, "FAIL") == 0) {
            json_object_object_get_ex(jobj, "message", &jmessage);
            const char* message = json_object_get_string(jmessage);

            if (message == NULL) { }
            
            user1->u_id = 0;
        }
        if(status && strcmp(status, "OK") == 0) {
            if (user1 != NULL) {
                struct json_object *user_id;
                struct json_object *login;
                struct json_object *user_name;
                struct json_object *user_bio;
                struct json_object *profile_image;
                struct json_object *user_online_date;
                //struct json_object **chats_array;
                
                json_object_object_get_ex(jobj, "user_id", &user_id);
                json_object_object_get_ex(jobj, "login", &login);
                json_object_object_get_ex(jobj, "name", &user_name);
                json_object_object_get_ex(jobj, "bio", &user_bio);
                json_object_object_get_ex(jobj, "profile_img", &profile_image);
                json_object_object_get_ex(jobj, "online_date", &user_online_date);
                //json_object_object_get_ex(jobj, "chats", chats_array);
                
                user1->u_id = json_object_get_int(user_id);
                user1->u_login = strdup((char *)json_object_get_string(login));
                user1->u_name = strdup((char *)json_object_get_string(user_name));
                user1->u_bio = strdup((char *)json_object_get_string(user_bio));
                user1->u_profile_image = NULL;
                user1->u_online_date = json_object_get_int(user_online_date);

                user1->u_chatarr_count = 0;
                user1->u_chatarr = (t_chat**)malloc(sizeof(t_chat*) * (user1->u_chatarr_count + 50));
                user1->u_chatarr_size = (user1->u_chatarr_count + 50);
            }
        }
    }//Response from init_user method. We need to save user`s credentials. login and id.
    else if (strcmp(method, "user_init") == 0) {
        struct json_object* jlogin;
        struct json_object* juser_id;
        json_object_object_get_ex(jobj, "login", &jlogin);
        json_object_object_get_ex(jobj, "user_id", &juser_id);
        
        char* login = strdup((char *)json_object_get_string(jlogin));
        int user_id = json_object_get_int(juser_id);
        // ???
        user1->u_id = user_id;
        user1->u_login = login;
    }
    else if (strcmp(method, "message") == 0) {
        if (message1 != NULL) {
            struct json_object* jmessage_text;
            struct json_object* jmessage_type;
            struct json_object* jmessage_id;
            struct json_object* jchat_id;
            struct json_object* juser_id;
            struct json_object* jmessage_form;
            struct json_object* jmessage_date;
            struct json_object* jmessage_is_edited;
            json_object_object_get_ex(jobj, "message_text", &jmessage_text);
            json_object_object_get_ex(jobj, "message_type", &jmessage_type);
            json_object_object_get_ex(jobj, "message_id", &jmessage_id);
            json_object_object_get_ex(jobj, "user_id", &juser_id);
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "message_form", &jmessage_form);
            json_object_object_get_ex(jobj, "message_date", &jmessage_date);
            json_object_object_get_ex(jobj, "is_edited", &jmessage_is_edited);
            
            //may be all message data needs to be saved somewhere
            message1->m_type = json_object_get_int(jmessage_type);
            message1->m_id = json_object_get_int(jmessage_id);
            message1->u_id = json_object_get_int(juser_id);
            message1->c_id = json_object_get_int(jchat_id);
            message1->m_form = json_object_get_int(jmessage_form);
            message1->m_date = json_object_get_int(jmessage_date);
            message1->m_is_edited = json_object_get_int(jmessage_is_edited);
            message1->m_text = strdup((char *)json_object_get_string(jmessage_text));
        }
    }
    else if (strcmp(method, "chat_delete") == 0) {
        //struct json_object* jchat_id;
        //json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        //int chat_id = json_object_get_int(jchat_id);
        

        
        //printf("\n ====CHAT DELETED : %i ==== \n", chat_id);
    }
    else if (strcmp(method, "change_chat") == 0) {
        if (chat1 != NULL) {
            struct json_object *jchat_id;
            struct json_object *jnew_name;
            struct json_object *jnew_desc;
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "name", &jnew_name);
            json_object_object_get_ex(jobj, "description", &jnew_desc);

            // save it to the t_chat
            chat1->c_id = json_object_get_int(jchat_id);
            chat1->c_name = strdup((char *)json_object_get_string(jnew_name));
            chat1->c_description = strdup((char *)json_object_get_string(jnew_desc));
        }
    }
    else if (strcmp(method, "edit_message") == 0) {
        if (message1 != NULL) {
            struct json_object *jchat_id;
            struct json_object *jmessage_id;
            struct json_object *jedited_text = NULL;
            int c_id = json_object_get_int(jchat_id);
            int m_id = json_object_get_int(jmessage_id);
            char *edited_msg = strdup((char *)json_object_get_string(jedited_text));

            //save it to the t_message
            message1->c_id = c_id;
            message1->m_id = m_id;
            message1->m_text = edited_msg;
        }
    }
    else if (strcmp(method, "delete_message") == 0) {
        struct json_object *jchat_id;
        struct json_object *jmessage_id;
        json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        json_object_object_get_ex(jobj, "message_id", &jmessage_id);

        message1->c_id = json_object_get_int(jchat_id);
        message1->m_id = json_object_get_int(jmessage_id);
    }
    else if (strcmp(method, "get_user_profile") == 0) {
        struct json_object *u_id;
        struct json_object *u_name;
        struct json_object *u_bio;
        struct json_object *u_online_date;
        //struct json_object *user_profile_image;

        json_object_object_get_ex(jobj, "user_id", &u_id);
        json_object_object_get_ex(jobj, "u_name", &u_name);
        json_object_object_get_ex(jobj, "u_bio", &u_bio);
        json_object_object_get_ex(jobj, "u_online_date", &u_online_date);
        //user_profile_image

        user1->u_id = json_object_get_int(u_id);
        user1->u_name = strdup((char *)json_object_get_string(u_name));
        user1->u_bio = strdup((char *)json_object_get_string(u_bio));
        user1->u_online_date = json_object_get_int(u_online_date);
    }
    else if (strcmp(method, "get_chat_messages") == 0) {
        if (chat1 != NULL) {
            struct json_object *jchat_id;
            struct json_object *jstatus;
            struct json_object *jmessages;
            
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "status", &jstatus);

            //chat1->c_id = json_object_get_int(jchat_id);
            
            int status = json_object_get_int(jstatus);
            if(status == 1) {
                json_object_object_get_ex(jobj, "messages", &jmessages);

                chat1->c_messagearr_count = json_object_array_length(jmessages);
                chat1->c_messagearr_size = chat1->c_messagearr_count + 50;
                chat1->c_messagearr = (t_message **)malloc(sizeof(t_message *) * chat1->c_messagearr_size);
                for (int i = 0; i < chat1->c_messagearr_count; i++) {
                    t_message *message_i = (t_message *)malloc(sizeof(t_message));

                    struct json_object *jmessage = json_object_array_get_idx(jmessages, i);
                    struct json_object *message_id;
                    struct json_object *user_id;
                    struct json_object *chat_id;
                    struct json_object *reply_to_id;
                    struct json_object *text;
                    struct json_object *is_edited;
                    struct json_object *date;
                    struct json_object *type;
                    struct json_object *form;

                    // extract the message data
                    json_object_object_get_ex(jmessage, "message_id", &message_id);
                    json_object_object_get_ex(jmessage, "user_id", &user_id);
                    json_object_object_get_ex(jmessage, "chat_id", &chat_id);
                    json_object_object_get_ex(jmessage, "reply_to_id", &reply_to_id);
                    json_object_object_get_ex(jmessage, "text", &text);
                    json_object_object_get_ex(jmessage, "is_edited", &is_edited);
                    json_object_object_get_ex(jmessage, "date", &date);
                    json_object_object_get_ex(jmessage, "type", &type);
                    json_object_object_get_ex(jmessage, "form", &form);
                    
                    // save data to struct
                    message_i->m_id = json_object_get_int(message_id);
                    message_i->u_id = json_object_get_int(user_id);
                    message_i->c_id = json_object_get_int(chat_id);
                    message_i->m_m_id = json_object_get_int(reply_to_id);
                    message_i->m_text = strdup((char *)json_object_get_string(text));
                    message_i->m_is_edited = json_object_get_int(is_edited);
                    message_i->m_date = json_object_get_int(date);
                    message_i->m_type = json_object_get_int(type);
                    message_i->m_form = json_object_get_int(form);

                    chat1->c_messagearr[i] = message_i;
                    //chat1->c_messagearr_count++;
                }
            }
            else {
                // return empty array of messages
                chat1->c_messagearr_count = 0;
                chat1->c_messagearr_size = chat1->c_messagearr_count + 50;
                chat1->c_messagearr = (t_message **)malloc(sizeof(t_message *) * chat1->c_messagearr_size);
            }
        }

        
    }
    else if (strcmp(method, "join_chat") == 0) {
        struct json_object *jc_type;
        
        json_object_object_get_ex(jobj, "c_type", &jc_type);
        int c_type = json_object_get_int(jc_type);

        if (c_type == 1) { //private chat
            struct json_object *jchat_id;
            struct json_object *jc_type;
            struct json_object *juser1_id;
            struct json_object *juser1_name;
            struct json_object *juser1_bio;
            struct json_object *juser1_img;
            struct json_object *juser2_id;
            struct json_object *juser2_name;
            struct json_object *juser2_bio;
            struct json_object *juser2_img;
            struct json_object *jjoin_type;
            struct json_object *jcus_type; // would be 3.(users role, in private chat all have 3)

            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "c_type", &jc_type);

            json_object_object_get_ex(jobj, "user1_id", &juser1_id);
            json_object_object_get_ex(jobj, "user1_name", &juser1_name);
            json_object_object_get_ex(jobj, "user1_bio", &juser1_bio);
            json_object_object_get_ex(jobj, "user1_img", &juser1_img);

            json_object_object_get_ex(jobj, "user2_id", &juser2_id);
            json_object_object_get_ex(jobj, "user2_name", &juser2_name);
            json_object_object_get_ex(jobj, "user2_bio", &juser2_bio);
            json_object_object_get_ex(jobj, "user2_img", &juser2_img);

            json_object_object_get_ex(jobj, "join_type", &jjoin_type);
            json_object_object_get_ex(jobj, "cus_type", &jcus_type);

            int chat_id = json_object_get_int(jchat_id);
            int c_type = json_object_get_int(jc_type);
            
            int user1_id = json_object_get_int(juser1_id);
            char *user1_name = strdup((char *)json_object_get_string(juser1_name));
            char *user1_bio = strdup((char *)json_object_get_string(juser1_bio));
            char *user1_img = strdup((char *)json_object_get_string(juser1_img));

            int user2_id = json_object_get_int(juser2_id);
            char *user2_name = strdup((char *)json_object_get_string(juser2_name));
            char *user2_bio = strdup((char *)json_object_get_string(juser2_bio));
            char *user2_img = strdup((char *)json_object_get_string(juser2_img));
            
            int join_type = json_object_get_int(jjoin_type);
            int cus_type = json_object_get_int(jcus_type);

            chat1->c_id = chat_id;
            chat1->ct_type = c_type;
            chat1->jt_type = join_type;
            chat1->cus_type = cus_type;

            if (user->u_id == user1_id) {
                //set from user2
                chat1->c_id = user2_id;
                chat1->c_name = user2_name;
                chat1->c_description = user2_bio;
                chat1->c_profile_image = user2_img;
            }
            else if (user->u_id == user2_id) {
                //set from user1
                chat1->c_id = user1_id;
                chat1->c_name = user1_name;
                chat1->c_description = user1_bio;
                chat1->c_profile_image = user1_img;
            }
        }
        
        if (c_type == 2) { // group chat
            struct json_object *jchat_id;
            struct json_object *jchat_name;
            struct json_object *jchat_desc;
            struct json_object *jchat_img;
            struct json_object *jjoin_type;
            struct json_object *jcus_type;

            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "chat_name", &jchat_name);
            json_object_object_get_ex(jobj, "chat_desc", &jchat_desc);
            json_object_object_get_ex(jobj, "chat_img", &jchat_img);
            json_object_object_get_ex(jobj, "join_type", &jjoin_type);
            json_object_object_get_ex(jobj, "cus_type", &jcus_type);
            
            int chat_id = json_object_get_int(jchat_id);
            char *chat_name = strdup((char *)json_object_get_string(jchat_name));
            char *chat_desc = strdup((char *)json_object_get_string(jchat_desc));
            char *chat_img = strdup((char *)json_object_get_string(jchat_img));
            int join_type = json_object_get_int(jjoin_type);
            int cus_type = json_object_get_int(jcus_type);

            chat1->c_id = chat_id;
            chat1->c_name = chat_name;
            chat1->c_description = chat_desc;
            chat1->c_profile_image = chat_img;
            chat1->jt_type = join_type;
            chat1->cus_type = cus_type;
            chat1->ct_type = c_type;
        }
    }
    else if (strcmp(method, "get_chat_users") == 0) {
        struct json_object *jchat_id;
        json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        int chat_id = json_object_get_int(jchat_id);

        struct json_object **jusers_array = NULL;
        int n_users = json_object_array_length(*jusers_array);

        t_user **users = (t_user**)malloc(sizeof(t_user *) * (n_users + 10));
        for (int i = 0; i < n_users; i++) {
            t_user *user_i = (t_user*)malloc(sizeof(t_user));

            struct json_object *juser_id;
            struct json_object *juser_name;
            struct json_object *juser_bio;
            struct json_object *juser_online_date;

            json_object_object_get_ex(jobj, "user_id", &juser_id);
            json_object_object_get_ex(jobj, "user_name", &juser_name);
            json_object_object_get_ex(jobj, "user_bio", &juser_bio);
            json_object_object_get_ex(jobj, "user_online_date", &juser_online_date);

            int user_id = json_object_get_int(juser_id);
            char *user_name = strdup((char *)json_object_get_string(juser_name));
            char *user_bio = strdup((char *)json_object_get_string(juser_bio));
            int user_online_date = json_object_get_int(juser_online_date);

            user_i->u_id = user_id;
            user_i->u_name = user_name;
            user_i->u_bio = user_bio;
            user_i->u_online_date = user_online_date;

            users[i] = user_i;
        }

        users1 = users;
        chat1->c_id = chat_id;
    }
    else if (strcmp(method, "search") == 0) {
        struct json_object *jc_type;
        json_object_object_get_ex(jobj, "c_type", &jc_type);
        int c_type = json_object_get_int(jc_type);

        struct json_object *juser_id;
        struct json_object *juser_name;

        json_object_object_get_ex(jobj, "u_id", &juser_id);
        json_object_object_get_ex(jobj, "u_name", &juser_name);

        int user_id = json_object_get_int(juser_id);
        char *user_name = strdup((char *)json_object_get_string(juser_name));

        chat1->c_id = user_id;
        chat1->c_name = user_name;
        chat1->ct_type = c_type;
        
    }
    else if (strcmp(method, "add_group") == 0) {
        if (chat1 != NULL) {
            struct json_object *jchat_type;
            struct json_object *jchat_id;
            struct json_object *jchat_name;
            struct json_object *jchat_desc;
            //struct json_object *juser_id;
            struct json_object *jchat_img;
            struct json_object *jjoin_type;
            struct json_object *jcus_name;

            json_object_object_get_ex(jobj, "c_type", &jchat_type);
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "chat_name", &jchat_name);
            json_object_object_get_ex(jobj, "chat_desc", &jchat_desc);
            //json_object_object_get_ex(jobj, "user_id", &juser_id);
            json_object_object_get_ex(jobj, "chat_img", &jchat_img);
            json_object_object_get_ex(jobj, "join_type", &jjoin_type);
            json_object_object_get_ex(jobj, "cus_type", &jcus_name);

            int c_type = json_object_get_int(jchat_type);
            int c_id = json_object_get_int(jchat_id);
            char *c_name = strdup((char *)json_object_get_string(jchat_name));
            char *c_desc = strdup((char *)json_object_get_string(jchat_desc));
            //int u_id = json_object_get_int(juser_id);
            char *c_image = strdup((char *)json_object_get_string(jchat_img));
            int join_type = json_object_get_int(jjoin_type);
            int cus_type = json_object_get_int(jcus_name);

            // fill struct 
            chat1->ct_type = c_type;
            chat1->c_id = c_id;
            chat1->c_name = c_name;
            chat1->c_description = c_desc;
            chat1->c_profile_image = c_image;
            chat1->jt_type = join_type;
            chat1->cus_type = cus_type;
        }

    }

    json_object_put(jobj);
}





