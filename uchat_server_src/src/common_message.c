#include "../inc/server.h"

void common_message(json_object *json_received_obj) {
    int user_id, chat_id, message_type, message_form, edited;
    char *message_text;
    struct json_object *user_id_obj, *chat_id_obj, *message_text_obj, *message_form_obj;
    json_object_object_get_ex(json_received_obj, "user_id", &user_id_obj);
    json_object_object_get_ex(json_received_obj, "chat_id", &chat_id_obj);
    json_object_object_get_ex(json_received_obj, "message_text", &message_text_obj);
    json_object_object_get_ex(json_received_obj, "message_form", &message_form_obj);
    user_id = json_object_get_int(user_id_obj);
    chat_id = json_object_get_int(chat_id_obj);
    message_text = (char *)json_object_get_string(message_text_obj);
    message_form = json_object_get_int(message_form_obj);
    message_type = 1; // Always 1 for common message
    edited = 0;
    // Get the current time as a string
    long int timestamp_for_db = get_current_time();

    
    // Insert the message into the database
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    
    char sql[1024];
    snprintf(sql, sizeof(sql), "INSERT INTO Message (u_id, c_id, m_text, m_is_edited, m_date, m_type, m_form) "
             "VALUES (%d, %d, '%s', %d, %d, %d, %d);", user_id, chat_id, message_text, 0, (int) timestamp_for_db, message_type, message_form);
    
    char *err_msg;
    rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error inserting message: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    long long message_id = sqlite3_last_insert_rowid(db);
    
    sqlite3_close(db);
    
    // Construct the JSON message object
    struct json_object *json_msg_obj = json_object_new_object();
    json_object_object_add(json_msg_obj, "method", json_object_new_string("message"));
    json_object_object_add(json_msg_obj, "message_id", json_object_new_int((int)message_id));
    json_object_object_add(json_msg_obj, "user_id", json_object_new_int(user_id));
    json_object_object_add(json_msg_obj, "chat_id", json_object_new_int(chat_id));
    json_object_object_add(json_msg_obj, "message_type", json_object_new_int(message_type));
    json_object_object_add(json_msg_obj, "message_text", json_object_new_string(message_text));
    json_object_object_add(json_msg_obj, "message_form", json_object_new_int(message_form));
    json_object_object_add(json_msg_obj, "message_date", json_object_new_int(timestamp_for_db));
    json_object_object_add(json_msg_obj, "is_edited", json_object_new_int(edited));
    
    // Broadcast the message to all online users in the chat
    broadcast_connection(json_received_obj, json_msg_obj);
}
