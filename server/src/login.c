#include "../inc/server.h"

void login(int socket_fd, json_object *json_received_obj) {
    // Check if username and password fields exist
    int logged = 0;
    int mtd = 0;
    json_object *username_obj, *password_obj;
    if (json_object_object_get_ex(json_received_obj, "username", &username_obj) && json_object_object_get_ex(json_received_obj, "password", &password_obj)) {
        const char *username = json_object_get_string(username_obj);
        const char *password = json_object_get_string(password_obj);
        
        sqlite3 *db;
        sqlite3_stmt *stmt;
        const char *query = "SELECT * FROM User WHERE u_login = ?;";
        int rc = sqlite3_open("src/uchatdb.db", &db);
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        rc = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            server_error(socket_fd);
        }
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // The user exists in the database
            const char *stored_password = (const char *) sqlite3_column_text(stmt, 2);
            if (strcmp(stored_password, password) == 0) {
                logged = 1;
                int user_id = get_user_id_from_db(username);
                if (user_id != -1) {
                    add_to_list(user_id, socket_fd);
                }
                sqlite3_finalize(stmt);
                
                struct json_object *chat_array = retrieve_user_chats(user_id);
                
                const char *query2 = "SELECT u_login, u_id, u_name, u_bio, u_online_date FROM User WHERE u_id = ?";
                sqlite3_stmt *stmt2;
                rc = sqlite3_prepare_v2(db, query2, -1, &stmt2, NULL);
                if (rc != SQLITE_OK) {
                    // handle error
                }
                rc = sqlite3_bind_int(stmt2, 1, user_id);
                if (rc != SQLITE_OK) {
                    // handle error
                }
                rc = sqlite3_step(stmt2);
                if (rc == SQLITE_ROW) {
                    // retrieve user data
                    const char *login = (const char *) sqlite3_column_text(stmt2, 0);
                    int uid = sqlite3_column_int(stmt2, 1);
                    const char *name = (const char *) sqlite3_column_text(stmt2, 2);
                    const char *bio = (const char *) sqlite3_column_text(stmt2, 3);
                    int online_date = sqlite3_column_text(stmt2, 4);
                    
                    // create json object with user data and chat array
                    struct json_object *user_data = json_object_new_object();
                    json_object_object_add(user_data, "method", json_object_new_string("login"));
                    json_object_object_add(user_data, "u_login", json_object_new_string(login));
                    json_object_object_add(user_data, "u_id", json_object_new_int(uid));
                    json_object_object_add(user_data, "u_name", json_object_new_string(name));
                    json_object_object_add(user_data, "u_bio", json_object_new_string(bio));
                    json_object_object_add(user_data, "u_profile_image", json_object_new_string(" "));
                    json_object_object_add(user_data, "u_online_date", json_object_new_int(online_date));
                    json_object_object_add(user_data, "chats", chat_array);
                    
                    // send the json object as response
                    const char *response_str = json_object_to_json_string(user_data);
                    int send_status = send(socket_fd, response_str, strlen(response_str), 0);
                    
                    sqlite3_finalize(stmt2);
                }
                
                
            } else if(logged == 0){
                // The password is incorrect
                password_error(socket_fd, mtd);
            }
        } else if (logged == 0){
            // The user does not exist in the database
            login_error(socket_fd, mtd);
        }
        if (stmt) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        
    }
}
