#include "../inc/server.h"

void search(int socket_fd, json_object *json_received_obj) {
    struct json_object *jsearch_string;
    json_object_object_get_ex(json_received_obj, "search_string", &jsearch_string);

    char *search_string = (char *)json_object_get_string(jsearch_string);
    int chat_f = 0;
    int user_f = 0;

    // Search for chat with exact name match
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);

    rc = sqlite3_prepare_v2(db, "SELECT c_id, c_name, ct_id FROM Chat WHERE c_name = ?;", -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, search_string, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    int step_result = sqlite3_step(stmt);
    if (step_result == SQLITE_ROW) {
        int c_id = sqlite3_column_int(stmt, 0);
        if(c_id != 0) {
            chat_f = 1;
            const char *c_name = (const char*)sqlite3_column_text(stmt, 1);
            int c_type = sqlite3_column_int(stmt, 2);
            struct json_object *jchat_result = json_object_new_object();
            json_object_object_add(jchat_result, "method", json_object_new_string("search"));
            json_object_object_add(jchat_result, "c_type", json_object_new_int(c_type));
            json_object_object_add(jchat_result, "c_id", json_object_new_int(c_id));
            json_object_object_add(jchat_result, "c_name", json_object_new_string(c_name));
            const char *response_str = json_object_to_json_string(jchat_result);
            send(socket_fd, response_str, strlen(response_str), 0);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

    }
    sqlite3_finalize(stmt);

    // If chat not found, search for user with exact name match
    rc = sqlite3_prepare_v2(db, "SELECT u_id, u_name FROM User WHERE u_name = ?;", -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, search_string, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    step_result = sqlite3_step(stmt);
    if (step_result == SQLITE_ROW) {
        // User found
        int u_id = sqlite3_column_int(stmt, 0);
        if(u_id != 0) {
            user_f = 1;

            const char *u_name = (const char*)sqlite3_column_text(stmt, 1);
            struct json_object *juser_result = json_object_new_object();
            json_object_object_add(juser_result, "method", json_object_new_string("search"));
            json_object_object_add(juser_result, "c_type", json_object_new_int(1));
            json_object_object_add(juser_result, "u_id", json_object_new_int(u_id));
            json_object_object_add(juser_result, "u_name", json_object_new_string(u_name));
            const char *response_str = json_object_to_json_string(juser_result);
            send(socket_fd, response_str, strlen(response_str), 0);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        
    }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    if(user_f == 0 && chat_f == 0) {
        struct json_object *juser_result = json_object_new_object();
        json_object_object_add(juser_result, "method", json_object_new_string("search"));
        json_object_object_add(juser_result, "c_type", json_object_new_int(-1));
        json_object_object_add(juser_result, "err", json_object_new_string("nothing found"));
        const char *response_str = json_object_to_json_string(juser_result);
        send(socket_fd, response_str, strlen(response_str), 0);
    }
    

}



