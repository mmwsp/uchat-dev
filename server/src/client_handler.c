#include "../inc/server.h"

void *client_handler(void *arg) {
    int client_socket = *(int*)arg;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    read(client_socket, buffer, sizeof(buffer));
    printf("Received message from client (raw): %s\n", buffer);

    // Parse JSON message
    json_object *json_received_obj = json_tokener_parse(buffer);
    json_object *method_obj;
    json_object_object_get_ex(json_received_obj, "method", &method_obj);
    char *method = (char*)json_object_get_string(method_obj);
    
    // Handle login method
    if (strcmp(json_object_get_string(method_obj), "login") == 0) {
        // Check if username and password fields exist
        json_object *username_obj, *password_obj;
        if (json_object_object_get_ex(json_received_obj, "username", &username_obj) && json_object_object_get_ex(json_received_obj, "password", &password_obj)) {
            const char *username = json_object_get_string(username_obj);
            const char *password = json_object_get_string(password_obj);
            
            sqlite3 *db;
            sqlite3_stmt *stmt;
            const char *query = "SELECT * FROM User WHERE u_login = ?;";
            int rc = sqlite3_open("src/uchatdb.db", &db);
            rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
            rc = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                server_error(client_socket);
            }
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                // The user exists in the database
                const char *stored_password = (const char *) sqlite3_column_text(stmt, 2);
                if (strcmp(stored_password, password) == 0) {
                    // The password is correct
                    json_object *response = json_object_new_object();
                    json_object_object_add(response, "method", json_object_new_string("login"));
                    json_object_object_add(response, "status", json_object_new_string("OK"));
                    const char *response_str = json_object_to_json_string(response);
                    write(client_socket, response_str, strlen(response_str));
                } else {
                    // The password is incorrect
                    password_error(client_socket);
                }
            } else {
                // The user does not exist in the database
                login_error(client_socket);
            }
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
    }

    //Handle registration method
    if (strcmp(json_object_get_string(method_obj), "registration") == 0) {
      json_object *login_obj = json_object_object_get(json_received_obj, "login");
      json_object *name_obj = json_object_object_get(json_received_obj, "name");
      json_object *password_obj = json_object_object_get(json_received_obj, "password");

      const char *login = json_object_get_string(login_obj);
      const char *name = json_object_get_string(name_obj);
      const char *password = json_object_get_string(password_obj);

      bool err = false;
      // Open the database connection
      sqlite3 *db;
      int rc = sqlite3_open("src/uchatdb.db", &db);
      if (rc != SQLITE_OK) {
          // Error opening the database
          server_error(client_socket);
          err = true;
      }
      sqlite3_stmt *stmt;
      char *sql = "SELECT u_login FROM User WHERE u_login = ?";
      rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
      if (rc != SQLITE_OK) {
        // Error preparing the SQL statement
        server_error(client_socket);
        sqlite3_close(db);
          err = true;
      }
      rc = sqlite3_bind_text(stmt, 1, login, -1, SQLITE_TRANSIENT);
      if (rc != SQLITE_OK) {
        server_error(client_socket);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
          err = true;
      }
      rc = sqlite3_step(stmt);
      if (rc == SQLITE_ROW) {
        // User already exists in the database
        login_error(client_socket);
        err = true;
      }
      sqlite3_finalize(stmt);


      // Prepare the SQL statement to insert the new user
      sql = "INSERT INTO User (u_login, u_name, u_password) VALUES (?, ?, ?)";

      rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
      if (rc != SQLITE_OK) {
          // Error preparing the SQL statement
          server_error(client_socket);
          sqlite3_close(db);
          err = true;
    }

      // Bind the values to the parameters in the SQL statement
      rc = sqlite3_bind_text(stmt, 1, login, -1, SQLITE_TRANSIENT);
      if (rc != SQLITE_OK) {
          server_error(client_socket);
          sqlite3_finalize(stmt);
          sqlite3_close(db);
          err = true;
      }
      rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
      if (rc != SQLITE_OK) {
          err = true;
          server_error(client_socket);
          sqlite3_finalize(stmt);
          sqlite3_close(db);
      }
      rc = sqlite3_bind_text(stmt, 3, password, -1, SQLITE_TRANSIENT);
      if (rc != SQLITE_OK) {
          err = true;
          server_error(client_socket);
          sqlite3_finalize(stmt);
          sqlite3_close(db);
      }

      // Execute the SQL statement
      rc = sqlite3_step(stmt);
      if (rc != SQLITE_DONE) {
          server_error(client_socket);
          sqlite3_finalize(stmt);
          sqlite3_close(db);
          err = true;
      }

      sqlite3_finalize(stmt);
      sqlite3_close(db);
    if(!err) {
        json_object *response = json_object_new_object();
        json_object_object_add(response, "method", json_object_new_string("registration"));
        json_object_object_add(response, "status", json_object_new_string("OK"));
        const char *response_str = json_object_to_json_string(response);
        write(client_socket, response_str, strlen(response_str));
        }
    }

    
    //Handle simple txt message
    if (strcmp(json_object_get_string(method_obj), "message") == 0) {
            // Parse the chat ID and message text from the JSON
            struct json_object *chat_id_obj;
            struct json_object *message_text_obj;
            json_object_object_get_ex(json_received_obj, "chat_id", &chat_id_obj);
            json_object_object_get_ex(json_received_obj, "text", &message_text_obj);
            int chat_id = json_object_get_int(chat_id_obj);
            char *message_text = (char*)json_object_get_string(message_text_obj);

            // Find the list of clients for the chat
            struct chat *chat_ptr = find_chat_by_id(chat_id);
            if (chat_ptr == NULL) {
                printf("No chat found with ID %d\n", chat_id);
            }
                
            // Send the message to all clients in the chat
            struct client *client_ptr = chat_ptr->clients;
            while (client_ptr != NULL) {
                broadcast_connection(client_ptr, chat_id, message_text);
                client_ptr = client_ptr->next;
            }
        }
    else {          // TODO: implement more conditions with other methods

        // Unknown method
        json_object *response = json_object_new_object();
        json_object_object_add(response, "method", json_object_new_string("error"));
        json_object_object_add(response, "err", json_object_new_string("Unknown method"));
        const char *response_str = json_object_to_json_string(response);
        write(client_socket, response_str, strlen(response_str));
    }

    // Free memory and close socket
    json_object_put(json_received_obj);
    close(client_socket);
    return NULL;
}
