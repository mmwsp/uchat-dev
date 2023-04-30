#include "../inc/server.h"

void add_group(int socket_fd, json_object *json_received_obj) {
    struct json_object *jname, *jdesc, *juser;
    json_object_object_get_ex(json_received_obj, "c_name", &jname);
    json_object_object_get_ex(json_received_obj, "c_desc", &jdesc);
    json_object_object_get_ex(json_received_obj, "user_id", &juser);
    
    int uid = json_object_get_int(juser);
    char *description = (char *) json_object_get_string(jdesc);
    char *name = (char *) json_object_get_string(jname);
    int ct = 2;
    int jt = 1;
    
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    char sql[1024];
    snprintf(sql, sizeof(sql), "INSERT INTO Chat (c_name, jt_id, ct_id, c_description) VALUES ('%s', %d, %d, '%s');", name, jt, ct, description);
    char *err_msg;
    rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error inserting chat: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    long long chat_id = sqlite3_last_insert_rowid(db);
    
    snprintf(sql, sizeof(sql), "INSERT INTO ChatUser (u_id, c_id, cus_id) VALUES (%d, %lld, 3);", uid, chat_id);
    rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error inserting user1 into ChatUser: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    
    struct json_object *json_chat_obj = json_object_new_object();
    json_object_object_add(json_chat_obj, "method", json_object_new_string("add_group"));
    json_object_object_add(json_chat_obj, "c_type", json_object_new_int(ct));
    json_object_object_add(json_chat_obj, "chat_id", json_object_new_int((int)chat_id));
    json_object_object_add(json_chat_obj, "chat_name", json_object_new_string(name));
    json_object_object_add(json_chat_obj, "chat_desc", json_object_new_string(description));
    json_object_object_add(json_chat_obj, "user_id", json_object_new_int(uid));
    json_object_object_add(json_chat_obj, "chat_img", json_object_new_string(" "));
    json_object_object_add(json_chat_obj, "join_type", json_object_new_int(jt));
    json_object_object_add(json_chat_obj, "cus_type", json_object_new_int(3));
    char *json_str = (char *)json_object_to_json_string(json_chat_obj);
    send(socket_fd, json_str, strlen(json_str), 0);
    
    sqlite3_close(db);

}

