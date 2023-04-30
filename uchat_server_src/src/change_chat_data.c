#include "../inc/server.h"

void change_chat(json_object *json_received_obj) {
    struct json_object *jchat_id, *jname, *jbio;
    json_object_object_get_ex(json_received_obj, "chat_id", &jchat_id);
    json_object_object_get_ex(json_received_obj, "name", &jname);
    json_object_object_get_ex(json_received_obj, "description", &jbio);
    
    int chat_id = json_object_get_int(jchat_id);
    const char *name = json_object_get_string(jname);
    const char *description = json_object_get_string(jbio);
    
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
       char *sql = "UPDATE Chats SET c_name = ?, c_description = ? WHERE c_id = ?;";
       rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
       if (rc != SQLITE_OK) {
           fprintf(stderr, "Error preparing update statement: %s\n", sqlite3_errmsg(db));
           return;
       }

       sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
       sqlite3_bind_int(stmt, 3, chat_id);

       rc = sqlite3_step(stmt);
       if (rc != SQLITE_DONE) {
           fprintf(stderr, "Error updating chat: %s\n", sqlite3_errmsg(db));
       }
    
    struct json_object *json_msg_obj = json_object_new_object();
    json_object_object_add(json_msg_obj, "method", json_object_new_string("change_chat"));
    json_object_object_add(json_msg_obj, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(json_msg_obj, "name", json_object_new_string(name));
    json_object_object_add(json_msg_obj, "description", json_object_new_string(description));

       sqlite3_finalize(stmt);
       sqlite3_close(db);
    
    broadcast_connection(json_received_obj, json_msg_obj);
    
}
