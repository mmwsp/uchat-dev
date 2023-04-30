#include "../inc/server.h"

void broadcast_connection(struct json_object *json_received_obj, struct json_object *json_message) {

    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error connecting to database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    struct json_object *chat_id_obj;
    json_object_object_get_ex(json_received_obj, "chat_id", &chat_id_obj);
    int chat_id = json_object_get_int(chat_id_obj);

    sqlite3_stmt *stmt;
    char *sql = "SELECT u_id FROM ChatUser WHERE c_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, chat_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        // Check if the user is online using list
        int receiver_socket_fd = get_socket_fd_from_list(user_id);
        if (receiver_socket_fd != -1) {
            // Send the JSON message to the online user
            char *json_str_message = (char *)json_object_to_json_string(json_message);
            send(receiver_socket_fd, json_str_message, strlen(json_str_message), 0);
            printf("\n sent json to cleint (in broadcast): %s \n", json_str_message);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
