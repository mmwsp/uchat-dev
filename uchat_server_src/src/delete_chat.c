#include "../inc/server.h"

void chat_delete(int socket_fd, json_object *json_received_obj) {
    struct json_object *chat_id_obj;
    json_object_object_get_ex(json_received_obj, "chat_id", &chat_id_obj);
    int chat_id = json_object_get_int(chat_id_obj);
    
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error connecting to database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Query the ChatUser table to get a list of user IDs associated with the chat
    sqlite3_stmt *stmt;
    char *sql = "SELECT u_id FROM ChatUser WHERE c_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, chat_id);
    int num_users = 0;
    int *user_ids = NULL;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        num_users++;
        user_ids = realloc(user_ids, num_users * sizeof(int));
        user_ids[num_users - 1] = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    
    
    int res = delete_chat_from_db(chat_id);
    if(res == 1) {
        plain_error(socket_fd);
    } else {
        for (int i = 0; i < num_users; i++) {
            int user_id = user_ids[i];
            int receiver_socket_fd = get_socket_fd_from_list(user_id);
            if (receiver_socket_fd != -1) {
                struct json_object *json_message = json_object_new_object();
                json_object_object_add(json_message, "method", json_object_new_string("chat_delete"));
                json_object_object_add(json_message, "chat_id", json_object_new_int(chat_id));
                const char *json_str_message = json_object_to_json_string(json_message);
                send(receiver_socket_fd, json_str_message, strlen(json_str_message), 0);
                printf("\n Sent json to client (in delete_chat): %s \n", json_str_message);
                json_object_put(json_message);
            }
        }
        
    }
    
    free(user_ids);
}

int delete_chat_from_db(int c_id) {
    int rc;
    sqlite3 *db;

    rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Delete all message files related to the chat
    char *sql = "DELETE FROM MessageFile WHERE m_id IN (SELECT m_id FROM Message WHERE c_id = ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_bind_int(stmt, 1, c_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_finalize(stmt);

    // Delete all messages related to the chat
    sql = "DELETE FROM Message WHERE c_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_bind_int(stmt, 1, c_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_finalize(stmt);

    // Delete all chat users related to the chat
    sql = "DELETE FROM ChatUser WHERE c_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_bind_int(stmt, 1, c_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_finalize(stmt);

    // Delete the chat itself
    sql = "DELETE FROM Chat WHERE c_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_bind_int(stmt, 1, c_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}
