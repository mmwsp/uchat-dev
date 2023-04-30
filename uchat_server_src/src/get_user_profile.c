#include "../inc/server.h"

void get_user_profile(int socket_fd, json_object *json_received_obj) {
    struct json_object *juser_id;
    json_object_object_get_ex(json_received_obj, "user_id", &juser_id);
    int user_id = json_object_get_int(juser_id);
    
    sqlite3 *db;
       int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
       if (rc != SQLITE_OK) {
           fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
           sqlite3_close(db);
           return;
       }

       sqlite3_stmt *stmt;
       rc = sqlite3_prepare_v2(db, "SELECT u_name, u_bio, u_online_date FROM User WHERE u_id = ?", -1, &stmt, NULL);
       if (rc != SQLITE_OK) {
           fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
           sqlite3_finalize(stmt);
           sqlite3_close(db);
           return;
       }
       
       sqlite3_bind_int(stmt, 1, user_id);
       
       rc = sqlite3_step(stmt);
       if (rc == SQLITE_ROW) {
           const char *u_name = (const char *) sqlite3_column_text(stmt, 0);
           const char *u_bio = (const char *) sqlite3_column_text(stmt, 1);
           int u_online_date = sqlite3_column_int(stmt, 2);
    
           json_object *juser = json_object_new_object();
           json_object_object_add(juser, "method", json_object_new_string("get_user_profile"));
           json_object_object_add(juser, "user_id", json_object_new_int(user_id));
           json_object_object_add(juser, "u_name", json_object_new_string(u_name));
           if(u_bio != NULL) {
               json_object_object_add(juser, "u_bio", json_object_new_string(u_bio));
           }
           else {
               json_object_object_add(juser, "u_bio", json_object_new_string(" "));
           }
    
           json_object_object_add(juser, "u_online_date", json_object_new_int(u_online_date));
           json_object_object_add(juser, "u_profile_image", json_object_new_string(" "));
           
           const char *json_str = json_object_to_json_string(juser);
           send(socket_fd, json_str, strlen(json_str), 0);
           
           json_object_put(juser);
       } else {
           fprintf(stderr, "User with ID %d not found\n", user_id);
       }
       
       sqlite3_finalize(stmt);
       sqlite3_close(db);

}

