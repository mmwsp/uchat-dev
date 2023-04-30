#include "../inc/server.h"

static int callback(void *data, int argc, char **argv, char **azColName __attribute__((unused))){
    int *user_id = data;
    if (argc > 0) {
        *user_id = atoi(argv[0]);

    }
    return 0;
}

int get_user_id_from_db(char *login) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    int user_id = -1;

    rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    char sql[100];
    sprintf(sql, "SELECT u_id FROM User WHERE u_login='%s'", login);

    rc = sqlite3_exec(db, sql, callback, &user_id, &err_msg);

    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select user: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    } else if (user_id == -1) {
        printf("No user found with login '%s'\n", login);
    }

    sqlite3_close(db);
    
    return user_id;
}

int delete_message_from_db(int message_id) {
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM Messages WHERE m_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    sqlite3_bind_int(stmt, 1, message_id);


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error deleting message: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

struct json_object* retrieve_user_chats(int user_id) {

    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    struct json_object* chat_array = json_object_new_array();
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return chat_array;
    }

    char sql[1024];
   /* snprintf(sql, sizeof(sql), "SELECT Chat.c_id, c_name, c_description, jt_id, ct_id FROM Chat JOIN ChatUser ON Chat.c_id = ChatUser.c_id WHERE ChatUser.u_id = ?;"); */
    snprintf(sql, sizeof(sql), "SELECT Chat.c_id, c_name, c_description, jt_id, ct_id, cus_id FROM Chat JOIN ChatUser ON Chat.c_id = ChatUser.c_id WHERE ChatUser.u_id = ?;");

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return chat_array;
    }

    rc = sqlite3_bind_int(stmt, 1, user_id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return chat_array;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int c_id = sqlite3_column_int(stmt, 0);
        const char* c_name = (const char*)sqlite3_column_text(stmt, 1);
        const char* c_description = (const char*)sqlite3_column_text(stmt, 2);
        int jt_id = sqlite3_column_int(stmt, 3);
        int ct_id = sqlite3_column_int(stmt, 4);
        int cus_id = sqlite3_column_int(stmt, 5);
        
        if(ct_id == 1) {
            sqlite3_stmt* stmt1;
            snprintf(sql, sizeof(sql), "SELECT u.u_id, u.u_name, u.u_bio FROM User u JOIN ChatUser cu ON cu.u_id = u.u_id WHERE cu.c_id = %d AND cu.u_id != %d;", c_id, user_id);
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt1, 0);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt1);
                sqlite3_close(db);
                return chat_array;
            }
            rc = sqlite3_step(stmt1);
            if (rc == SQLITE_ROW) {
                int partner_id = sqlite3_column_int(stmt1, 0);
                const char* partner_name = (const char*) sqlite3_column_text(stmt1, 1);
                const char* partner_bio = (const char*) sqlite3_column_text(stmt1, 2);

                char *partner_name_copy = NULL;
                char *partner_bio_copy = NULL;

                if (partner_name != NULL) {
                    partner_name_copy = strdup(partner_name);
                }

                if (partner_bio != NULL) {
                    partner_bio_copy = strdup(partner_bio);
                }

                // Add partner info to JSON object
                struct json_object* chat_obj = json_object_new_object();
                json_object_object_add(chat_obj, "u_id", json_object_new_int(partner_id));
                json_object_object_add(chat_obj, "c_id", json_object_new_int(c_id));
                json_object_object_add(chat_obj, "u_name", json_object_new_string(partner_name_copy));
                if(partner_bio != NULL) {
                    json_object_object_add(chat_obj, "u_bio", json_object_new_string(partner_bio_copy));
                } else {
                    json_object_object_add(chat_obj, "u_bio", json_object_new_string(" "));
                }
                json_object_object_add(chat_obj, "u_profile_img", json_object_new_string(" "));
                json_object_object_add(chat_obj, "jt_id", json_object_new_int(jt_id));
                json_object_object_add(chat_obj, "ct_id", json_object_new_int(ct_id));
                json_object_object_add(chat_obj, "cus_id", json_object_new_int(3));
                json_object_array_add(chat_array, chat_obj);
                free(partner_name_copy);
                free(partner_bio_copy);
            }
            sqlite3_finalize(stmt1);
        }
        else {
            struct json_object* chat_obj = json_object_new_object();
            json_object_object_add(chat_obj, "c_id", json_object_new_int(c_id));
            json_object_object_add(chat_obj, "c_name", json_object_new_string(c_name));
            if(c_description != NULL) {
                json_object_object_add(chat_obj, "c_description", json_object_new_string(c_description));
            } else {

                json_object_object_add(chat_obj, "c_description", json_object_new_string(" "));
            }
            json_object_object_add(chat_obj, "c_profile_img", json_object_new_string(" "));
            json_object_object_add(chat_obj, "jt_id", json_object_new_int(jt_id));
            json_object_object_add(chat_obj, "cus_id", json_object_new_int(cus_id));
            json_object_object_add(chat_obj, "ct_id", json_object_new_int(ct_id));

            json_object_array_add(chat_array, chat_obj);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return chat_array;
}

int insert_user_into_chat(int chat_id, int user_id) {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }


    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ChatUser (u_id, c_id) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, chat_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // handle error
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
