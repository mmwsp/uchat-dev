#include "../inc/server.h"

void delete_user(json_object *json_received_obj) {
    struct json_object *juser_id;
    json_object_object_get_ex(json_received_obj, "user_id", &juser_id);
    
    int user_id = json_object_get_int(juser_id);
    
    sqlite3 *db;
        char *err_msg = 0;
        
        int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error connecting to database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        char *update_query = sqlite3_mprintf("UPDATE users SET u_name='%q', u_bio='%q', u_online_date=%d, u_login='%q', u_password='%q' WHERE user_id=%d;",
            "Deleted Account", "User was deleted.", -1, "deleted_accountXXX", "91034u810924u298dfus8udfhj823uy109ikad", user_id);

        rc = sqlite3_exec(db, update_query, 0, 0, &err_msg);

        sqlite3_close(db);
}
