#include "../inc/server.h"

void registration(int socket_fd, json_object *json_received_obj) {
    json_object *login_obj = json_object_object_get(json_received_obj, "login");
    json_object *name_obj = json_object_object_get(json_received_obj, "name");
    json_object *password_obj = json_object_object_get(json_received_obj, "password");
    
    char *login = (char *) json_object_get_string(login_obj);
    const char *name = json_object_get_string(name_obj);
    const char *password = json_object_get_string(password_obj);
    int mtd = 1;
    bool err = false;
    bool already_exist = false;
    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("uchat_server_src/src/uchatdb.db", &db);
    if (rc != SQLITE_OK) {
        // Error opening the database
        server_error(socket_fd);
        err = true;
    }
    sqlite3_stmt *stmt;
    char *sql = "SELECT u_login FROM User WHERE u_login = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // Error preparing the SQL statement
        server_error(socket_fd);
        sqlite3_close(db);
        err = true;
    }
    rc = sqlite3_bind_text(stmt, 1, login, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        server_error(socket_fd);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        err = true;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // User already exists in the database
        err = true;
        already_exist = true;
    }
    sqlite3_finalize(stmt);
    
    
    // Prepare the SQL statement to insert the new user
    if (!already_exist) {
        sql = "INSERT INTO User (u_login, u_name, u_password) VALUES (?, ?, ?)";
        err = false;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            // Error preparing the SQL statement
            server_error(socket_fd);
            sqlite3_close(db);
            err = true;
        }
        
        // Bind the values to the parameters in the SQL statement
        rc = sqlite3_bind_text(stmt, 1, login, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            server_error(socket_fd);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            err = true;
        }
        rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            err = true;
            server_error(socket_fd);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        rc = sqlite3_bind_text(stmt, 3, password, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            err = true;
            server_error(socket_fd);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        
        // Execute the SQL statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            server_error(socket_fd);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            err = true;
        }
        
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        if(!err) {
            int user_id = get_user_id_from_db(login);
            if (user_id != -1) {
                add_to_list(user_id, socket_fd);
                printf("\nuser id and socket_fd added. ");
            }
            
            json_object *response = json_object_new_object();
            json_object_object_add(response, "method", json_object_new_string("registration"));
            json_object_object_add(response, "login", json_object_new_string(login));
            json_object_object_add(response, "user_id", json_object_new_int(user_id));
            json_object_object_add(response, "name", json_object_new_string(name));
            json_object_object_add(response, "bio", json_object_new_string(" "));
            json_object_object_add(response, "profile_img", json_object_new_string(" "));
            json_object_object_add(response, "online_date", json_object_new_int(1));
            json_object_object_add(response, "status", json_object_new_string("OK"));
            const char *response_str = json_object_to_json_string(response);
            send(socket_fd, response_str, strlen(response_str), 0);
        }
    } else if (err){
        login_error(socket_fd, mtd);
    }
}
