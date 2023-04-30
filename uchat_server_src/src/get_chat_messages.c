#include "../inc/server.h"

void get_chat_messages(int socket_fd, json_object *json_received_obj) {
    struct json_object *jchat_id;
    json_object_object_get_ex(json_received_obj, "chat_id", &jchat_id);
    
    int chat_id = json_object_get_int(jchat_id);
    int found = 0;
    
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // create SQL statement
    char *sql = sqlite3_mprintf("SELECT * FROM Message WHERE c_id = %d", chat_id);

    // execute SQL statement and retrieve results
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_free(sql);
        sqlite3_close(db);
        return;
    }

    // create array for messages
    struct json_object *jmessages = json_object_new_array();

    // iterate over results and construct JSON objects for messages
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int message_id = sqlite3_column_int(stmt, 0);
        int user_id = sqlite3_column_int(stmt, 1);
        int chat_id = sqlite3_column_int(stmt, 2);
        int reply_to_id = sqlite3_column_int(stmt, 3);
        char *text = (char*)sqlite3_column_text(stmt, 4);
        int is_edited = sqlite3_column_int(stmt, 5);
        int date = sqlite3_column_int(stmt, 6);
        int type = sqlite3_column_int(stmt, 7);
        int form = sqlite3_column_int(stmt, 8);
        found = 1;

        // create JSON object for message and add to array
        struct json_object *jmessage = json_object_new_object();
        json_object_object_add(jmessage, "message_id", json_object_new_int(message_id));
        json_object_object_add(jmessage, "user_id", json_object_new_int(user_id));
        json_object_object_add(jmessage, "chat_id", json_object_new_int(chat_id));
        json_object_object_add(jmessage, "reply_to_id", json_object_new_int(reply_to_id));
        json_object_object_add(jmessage, "text", json_object_new_string(text));
        json_object_object_add(jmessage, "is_edited", json_object_new_int(is_edited));
        json_object_object_add(jmessage, "date", json_object_new_int(date));
        json_object_object_add(jmessage, "type", json_object_new_int(type));
        json_object_object_add(jmessage, "form", json_object_new_int(form));
        json_object_array_add(jmessages, jmessage);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    sqlite3_close(db);

    if(found == 1 && json_object_array_length(jmessages) != 0) {
        struct json_object *jresponse = json_object_new_object();
        json_object_object_add(jresponse, "method", json_object_new_string("get_chat_messages"));
        json_object_object_add(jresponse, "status", json_object_new_int(1));
        json_object_object_add(jresponse, "chat_id", json_object_new_int(chat_id));
        json_object_object_add(jresponse, "messages", jmessages);

        const char *json_str = json_object_to_json_string(jresponse);
        send(socket_fd, json_str, strlen(json_str), 0);
    }
    else if (json_object_array_length(jmessages) == 0){
        struct json_object *jresponse = json_object_new_object();
        json_object_object_add(jresponse, "method", json_object_new_string("get_chat_messages"));
        json_object_object_add(jresponse, "chat_id", json_object_new_int(chat_id));
        json_object_object_add(jresponse, "messages", jmessages);

        const char *json_str = json_object_to_json_string(jresponse);
        send(socket_fd, json_str, strlen(json_str), 0);
    }
    
}
