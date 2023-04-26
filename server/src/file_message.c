#include "../inc/server.h"


void file_message(int client_socket, json_object *json_received_obj) {
    int user_id, chat_id, message_type, message_form, edited, file_size;
    char *message_text, file_name, message_file;
    struct json_object *user_id_obj, *chat_id_obj, *message_text_obj, *message_form_obj, *message_file_obj, *file_size_obj, *file_name_obj;
    json_object_object_get_ex(json_received_obj, "user_id", &user_id_obj);
    json_object_object_get_ex(json_received_obj, "chat_id", &chat_id_obj);
    json_object_object_get_ex(json_received_obj, "message_text", &message_text_obj);
    json_object_object_get_ex(json_received_obj, "file_name", &file_name_obj);
    json_object_object_get_ex(json_received_obj, "message_file", &message_file_obj);
    json_object_object_get_ex(json_received_obj, "file_size", &file_size_obj);
    
    file_size = json_object_get_int(file_size_obj);
    user_id = json_object_get_int(user_id_obj);
    chat_id = json_object_get_int(chat_id_obj);
    file_name = (char *)json_object_get_string(file_name_obj);
    message_file = (char *)json_object_get_string(message_file_obj);
    message_text = (char *)json_object_get_string(message_text_obj);
    message_form = json_object_get_int(message_form_obj);
    message_type = 3; //  3 for file message
    edited = 0;
    // Get the current time as a string
    long int timestamp_for_db = get_current_time();
    char* timestamp_for_msg = format_time(timestamp_for_db);
    
    // Insert the message into the database
    sqlite3 *db;
    int rc = sqlite3_open("src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    char sql[1024];
    snprintf(sql, sizeof(sql), "INSERT INTO Message (u_id, c_id, m_text, m_is_edited, m_date, m_type, m_form) "
             "VALUES (%d, %d, '%s', %d, %d, %d, %d);", user_id, chat_id, message_text, 0, timestamp_for_db, message_type, message_form);
    
    char *err_msg;
    rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error inserting message: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    long long message_id = sqlite3_last_insert_rowid(db);
    //insert file into files table
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "INSERT INTO MessageFile (m_id, mf_file, mf_name, mf_size) VALUES (?, ?, ?, ?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Bind the parameters to the statement
    sqlite3_bind_int64(stmt, 1, message_id); // m_id
    sqlite3_bind_blob(stmt, 2, message_file, file_size, SQLITE_STATIC); // mf_file
    sqlite3_bind_text(stmt, 3, file_name, -1, SQLITE_STATIC); // mf_name
    sqlite3_bind_int(stmt, 4, file_size); // mf_size

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error inserting message file: %s\n", sqlite3_errmsg(db));
    }
    
    long long mf_id = sqlite3_last_insert_rowid(db);
    
    sqlite3_finalize(stmt);
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
    json_object_object_add(json_msg_obj, "message_date", json_object_new_string(timestamp_for_msg));
    json_object_object_add(json_msg_obj, "is_edited", json_object_new_int(edited));
    json_object_object_add(json_msg_obj, "message_file_id", json_object_new_int(mf_id));
    json_object_object_add(json_msg_obj, "file_name", json_object_new_string(file_name));
    json_object_object_add(json_msg_obj, "message_file", json_object_new_string(message_file));
    json_object_object_add(json_msg_obj, "file_size", json_object_new_int(file_size));
    
    // Broadcast the message to all online users in the chat
    broadcast_connection(json_received_obj, client_socket, json_msg_obj);
    
}
