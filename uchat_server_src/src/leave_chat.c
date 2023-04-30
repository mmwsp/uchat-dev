#include "../inc/server.h"

void leave_chat(int socket_fd, json_object *json_received_obj) {
    struct json_object *jchat, *juser;
    json_object_object_get_ex(json_received_obj, "chat_id", &jchat);
    json_object_object_get_ex(json_received_obj, "user_id", &juser);
    
    int st = 1;
    int u_id = json_object_get_int(juser);
    int c_id = json_object_get_int(jchat);
    
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM ChatUser WHERE u_id = ? AND c_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        st = 0;
        struct json_object *json_chat_obj = json_object_new_object();
        json_object_object_add(json_chat_obj, "method", json_object_new_string("leave_chat"));
        json_object_object_add(json_chat_obj, "status", json_object_new_string("FAIL"));
        char *json_str = (char *)json_object_to_json_string(json_chat_obj);
        send(socket_fd, json_str, strlen(json_str), 0);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, u_id);
    sqlite3_bind_int(stmt, 2, c_id);

    // Execute query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error deleting user from chat: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        st = 0;
        struct json_object *json_chat_obj = json_object_new_object();
        json_object_object_add(json_chat_obj, "method", json_object_new_string("leave_chat"));
        json_object_object_add(json_chat_obj, "status", json_object_new_string("FAIL"));
        char *json_str = (char *)json_object_to_json_string(json_chat_obj);
        send(socket_fd, json_str, strlen(json_str), 0);
        return;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    if(st == 1) {
        struct json_object *json_chat_obj = json_object_new_object();
        json_object_object_add(json_chat_obj, "method", json_object_new_string("leave_chat"));
        json_object_object_add(json_chat_obj, "status", json_object_new_string("OK"));
        char *json_str = (char *)json_object_to_json_string(json_chat_obj);
        send(socket_fd, json_str, strlen(json_str), 0);
    }

    
}
