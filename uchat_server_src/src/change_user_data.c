#include "../inc/server.h"

void change_user(int socket_fd, json_object *json_received_obj) {
    struct json_object *juser_id, *jname, *jbio;
    json_object_object_get_ex(json_received_obj, "user_id", &juser_id);
    json_object_object_get_ex(json_received_obj, "name", &jname);
    json_object_object_get_ex(json_received_obj, "bio", &jbio);
    
    int user_id = json_object_get_int(juser_id);
    const char *name = json_object_get_string(jname);
    const  char *bio = json_object_get_string(jbio);
    
    sqlite3 *db;
        int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
        if (rc != SQLITE_OK) {
            plain_error(socket_fd);
        }

        // Construct the SQL statement
        char *sql = sqlite3_mprintf("UPDATE User SET u_name = '%q', u_bio = '%q' WHERE u_id = %d", name, bio, user_id);

        // Execute the SQL statement
        rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            plain_error(socket_fd);
        }

        // Clean up
        sqlite3_free(sql);
        sqlite3_close(db);
    
}
