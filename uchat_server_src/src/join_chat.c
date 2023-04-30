#include "../inc/server.h"

void join_chat(int socket_fd, json_object *json_received_obj) {
    struct json_object *chat_type_obj;
    json_object_object_get_ex(json_received_obj, "c_type", &chat_type_obj);
    int chat_type = json_object_get_int(chat_type_obj);
    
    if(chat_type == 1) {
        struct json_object *jsender_id, *juser2_id;
        json_object_object_get_ex(json_received_obj, "sender_id", &jsender_id);
        json_object_object_get_ex(json_received_obj, "user2_id", &juser2_id);
        int sender_id = json_object_get_int(jsender_id);
        int user2_id = json_object_get_int(juser2_id);
        int join_type = 1; 
        char *sender_bio;
        char *sender_name;
        int sender_online;
        char *user2_name;
        char *user2_bio = NULL;
        int user2_online = 0;
        
        sqlite3 *db;
        int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        //sqlite3_busy_timeout(db, 5000);
        char sql[1024];
        snprintf(sql, sizeof(sql), "INSERT INTO Chat (c_name, jt_id, ct_id) VALUES ('', %d, %d);", join_type, chat_type);
        char *err_msg;
        rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error inserting chat: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return;
        }
        long long chat_id = sqlite3_last_insert_rowid(db);
        
        // Insert users into the ChatUser table
        snprintf(sql, sizeof(sql), "INSERT INTO ChatUser (u_id, c_id, cus_id) VALUES (%d, %lld, 3);", sender_id, chat_id);
        rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error inserting user1 into ChatUser: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return;
        }
        snprintf(sql, sizeof(sql), "INSERT INTO ChatUser (u_id, c_id, cus_id) VALUES (%d, %lld, 3);", user2_id, chat_id);
        rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error inserting user2 into ChatUser: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return;
        }
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, "SELECT u_name, u_bio, u_online_date FROM User WHERE u_id = ?;", -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparing SELECT statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        // Bind sender_id parameter
        rc = sqlite3_bind_int(stmt, 1, sender_id);
        if (rc != SQLITE_OK) {
           // write(1, " 1 ", 4);
            fprintf(stderr, "Error binding parameter: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // Retrieve sender_id data
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            sender_name = (char *)sqlite3_column_text(stmt, 0);
            sender_bio = (char *)sqlite3_column_text(stmt, 1);
            sender_online = sqlite3_column_int(stmt, 2);

        } else {
            fprintf(stderr, "Error retrieving sender_id data: %s\n", sqlite3_errmsg(db));
        }
        // stop
        
        sqlite3_stmt *stmt1;
        rc = sqlite3_prepare_v2(db, "SELECT u_name, u_bio, u_online_date FROM User WHERE u_id = ?;", -1, &stmt1, NULL);

        // Bind user2_id parameter
        rc = sqlite3_bind_int(stmt1, 1, user2_id);
        if (rc != SQLITE_OK) {
       //     write(1, " 2 ", 4);
            fprintf(stderr, "Error binding parameter: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // Retrieve user2_id data
        rc = sqlite3_step(stmt1);
        if (rc == SQLITE_ROW) {
            user2_name = (char *)sqlite3_column_text(stmt1, 0);
            user2_bio = (char *)sqlite3_column_text(stmt1, 1);
            user2_online = sqlite3_column_int(stmt1, 2);
        } else {
            fprintf(stderr, "Error retrieving user2_id data: %s\n", sqlite3_errmsg(db));
        }


        
        struct json_object *json_chat_obj = json_object_new_object();
        json_object_object_add(json_chat_obj, "method", json_object_new_string("join_chat"));
        json_object_object_add(json_chat_obj, "c_type", json_object_new_int(chat_type));
        json_object_object_add(json_chat_obj, "chat_id", json_object_new_int((int)chat_id));
        json_object_object_add(json_chat_obj, "user1_id", json_object_new_int(sender_id));
        json_object_object_add(json_chat_obj, "user1_name", json_object_new_string(sender_name));
        if(sender_bio != NULL) {
            json_object_object_add(json_chat_obj, "user1_bio", json_object_new_string(sender_bio));
        } else {
            json_object_object_add(json_chat_obj, "user1_bio", json_object_new_string(" "));
        }

        json_object_object_add(json_chat_obj, "user1_img", json_object_new_string(" "));
        json_object_object_add(json_chat_obj, "user1_online", json_object_new_int(sender_online));
        json_object_object_add(json_chat_obj, "user2_name", json_object_new_string(user2_name));
        json_object_object_add(json_chat_obj, "user2_id", json_object_new_int(user2_id));
        if(user2_bio != NULL) {
            json_object_object_add(json_chat_obj, "user2_bio", json_object_new_string(user2_bio));
        } else {
            json_object_object_add(json_chat_obj, "user2_bio", json_object_new_string(" "));
        }

        json_object_object_add(json_chat_obj, "user2_img", json_object_new_string(" "));
        json_object_object_add(json_chat_obj, "user2_online", json_object_new_int(user2_online));
        json_object_object_add(json_chat_obj, "join_type", json_object_new_int(join_type));
        json_object_object_add(json_chat_obj, "cus_type", json_object_new_int(3));
        
        int receiver_socket_fd1 = get_socket_fd_from_list(sender_id);
        if (receiver_socket_fd1 != -1) {
            char *json_str_message = (char *)json_object_to_json_string(json_chat_obj);
            send(receiver_socket_fd1, json_str_message, strlen(json_str_message), 0);
            printf("\n sent json to cleint (init chat): %s \n", json_str_message);
        }
        int receiver_socket_fd2 = get_socket_fd_from_list(user2_id);
        if (receiver_socket_fd2 != -1) {
            char *json_str_message = (char *)json_object_to_json_string(json_chat_obj);
            send(receiver_socket_fd2, json_str_message, strlen(json_str_message), 0);
            printf("\n sent json to cleint (init chat): %s \n", json_str_message);
        }
        sqlite3_finalize(stmt1);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    else if(chat_type == 2) {
        struct json_object *jchat_id, *jsender;
        json_object_object_get_ex(json_received_obj, "chat_id", &jchat_id);
        int chat_id = json_object_get_int(jchat_id);
        json_object_object_get_ex(json_received_obj, "sender_id", &jsender);
        int sender_id = json_object_get_int(jsender);
        
        int res = insert_user_into_chat(chat_id, sender_id);
        if(res == -1){
            printf("\n error inserting user in chat (db).  ");
        }
        
        
        sqlite3 *db;
        int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error connecting to database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        sqlite3_stmt *stmt;
        const char *sql = "SELECT c_id, c_name, c_description, jt_id, ct_id FROM Chat WHERE c_id = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, chat_id);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int c_id = sqlite3_column_int(stmt, 0);
                const char *c_name = (const char *) sqlite3_column_text(stmt, 1);
                const char *c_description = (const char *) sqlite3_column_text(stmt, 2);
                int jt_id = sqlite3_column_int(stmt, 3);
                int ct_id = sqlite3_column_int(stmt, 4);
                struct json_object *json_chat_obj = json_object_new_object();
                json_object_object_add(json_chat_obj, "method", json_object_new_string("join_chat"));
                json_object_object_add(json_chat_obj, "c_type", json_object_new_int(ct_id));
                json_object_object_add(json_chat_obj, "chat_id", json_object_new_int(c_id));
                json_object_object_add(json_chat_obj, "chat_name", json_object_new_string(c_name));
                if(c_description != NULL) {
                    json_object_object_add(json_chat_obj, "chat_desc", json_object_new_string(c_description));
                }
                else {
                    json_object_object_add(json_chat_obj, "chat_desc", json_object_new_string(" "));
                }
       
                json_object_object_add(json_chat_obj, "chat_img", json_object_new_string(" "));
                json_object_object_add(json_chat_obj, "join_type", json_object_new_int(jt_id));
                json_object_object_add(json_chat_obj, "cus_type", json_object_new_int(1));
                char *json_str = (char *)json_object_to_json_string(json_chat_obj);
                send(socket_fd, json_str, strlen(json_str), 0);
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }

}
