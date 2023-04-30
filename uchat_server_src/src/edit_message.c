#include "../inc/server.h"

void edit_message(json_object *json_received_obj) {
    struct json_object *jchat_id, *jmsg_id, *jedited_text;
    json_object_object_get_ex(json_received_obj, "chat_id", &jchat_id);
    json_object_object_get_ex(json_received_obj, "message_id", &jmsg_id);
    json_object_object_get_ex(json_received_obj, "edited_text", &jedited_text);
    
    int chat_id = json_object_get_int(jchat_id);
    int message_id = json_object_get_int(jmsg_id);
    const char *edited_text = json_object_get_string(jedited_text);
    
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error connecting to database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Message SET m_text = ?, m_is_edited = 1 WHERE m_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, edited_text, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, message_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error updating message: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    struct json_object *json_obj = json_object_new_object();
    json_object_object_add(json_obj, "method", json_object_new_string("edit_message"));
    json_object_object_add(json_obj, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(json_obj, "message_id", json_object_new_int(message_id));
    json_object_object_add(json_obj, "edited_text", json_object_new_string(edited_text));
    json_object_object_add(json_obj, "is_edited", json_object_new_int(1));
    
    broadcast_connection(json_received_obj, json_obj);
}
