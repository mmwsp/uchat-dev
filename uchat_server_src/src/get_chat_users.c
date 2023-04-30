#include "../inc/server.h"

void get_chat_users(int socket_fd, json_object *json_received_obj) {
    struct json_object *jchat_id;
    json_object_object_get_ex(json_received_obj, "chat_id", &jchat_id);
    int chat_id = json_object_get_int(jchat_id);

    // connect to database
    sqlite3 *db;
    if (sqlite3_open("uchat_server_src/src/uchatdb.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // prepare SQL statement
    sqlite3_stmt *stmt;
    const char *sql = "SELECT User.u_id, User.u_name, User.u_bio, User.u_online_date FROM ChatUser JOIN User ON ChatUser.u_id=User.u_id WHERE ChatUser.c_id=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, chat_id);


    struct json_object *jusers = json_object_new_array();

    // iterate over results and construct JSON objects for users
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        char *user_name = (char*)sqlite3_column_text(stmt, 1);
        char *user_bio = (char*)sqlite3_column_text(stmt, 2);
        int user_online_date = sqlite3_column_int(stmt, 3);

        // create JSON object for user and add to array
        struct json_object *juser = json_object_new_object();
        json_object_object_add(juser, "user_id", json_object_new_int(user_id));
        json_object_object_add(juser, "user_name", json_object_new_string(user_name));
        json_object_object_add(juser, "user_bio", json_object_new_string(user_bio));
        json_object_object_add(juser, "user_online_date", json_object_new_int(user_online_date));
        json_object_array_add(jusers, juser);
    }

    // finalize SQL statement and close database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // create JSON response object
    struct json_object *jresponse = json_object_new_object();
    json_object_object_add(jresponse, "method", json_object_new_string("get_chat_users"));
    json_object_object_add(jresponse, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(jresponse, "users", jusers);

    const char *json_str = json_object_to_json_string(jresponse);
    send(socket_fd, json_str, strlen(json_str), 0);
}
