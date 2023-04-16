#include "../inc/client.h"

//send json to server
int send_json_object(int client_socket, struct json_object* json_obj) {
    char* json_str = (char*)json_object_to_json_string(json_obj);
    if (send(client_socket, json_str, strlen(json_str), 0) == -1) {
        perror("Error sending JSON object to server");
        return -1;
    }
    return 0;
}

//receive json from server
int receive_json_object(int client_socket, struct json_object** json_obj) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    
    if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving response from server");
        return -1;
    }

    *json_obj = json_tokener_parse(buffer);
    if (*json_obj == NULL) {
        perror("Error parsing JSON object received from server");
        return -1;
    }
    
    return 0;
}

// parse received message
void parse_message(char* buffer) {
    struct json_object* jobj;
    enum json_tokener_error jerr = json_tokener_success;

    jobj = json_tokener_parse_verbose(buffer, &jerr);
    if (jerr != json_tokener_success) {
        fprintf(stderr, "Error parsing JSON message: %s\n", json_tokener_error_desc(jerr));
        return;
    }

    //check type of receiving data
    enum json_type jtype = json_object_get_type(jobj);
    if (jtype != json_type_object) {
        fprintf(stderr, "Expected JSON object, received %s\n", json_type_to_name(jtype));
        return;
    }

    //take method from json
    struct json_object* jmethod;
    if (!json_object_object_get_ex(jobj, "method", &jmethod)) {
        fprintf(stderr, "Missing 'method' field in JSON object\n");
        return;
    }

    //check method type
    if (json_object_get_type(jmethod) != json_type_string) {
        fprintf(stderr, "Expected 'method' field to be a string\n");
        return;
    }

    //value of method put in string
    const char* method = json_object_get_string(jmethod);
    printf("Received message with method: %s\n", method);

    // Handle the message based on the method
    if (strcmp(method, "login") == 0) {
        struct json_object* jstatus;
        struct json_object* jmessage;
        if (!json_object_object_get_ex(jobj, "status", &jstatus)) {
            fprintf(stderr, "Missing status field in login message\n");
            return;
        }

        if (json_object_get_type(jstatus) != json_type_string) {
            fprintf(stderr, "Expected 'status' field to be strings\n");
            return;
        }

        const char* status = json_object_get_string(jstatus);
        printf("Received login-confirmation status: %s \n", status);
        const char* message = json_object_get_string(jmessage);
        if(message) {
            printf("Received status-message: %s \n", message);
        }


        // TODO:  login logic here
        
    } else if(strcmp(method, "registration") == 0) {
        struct json_object* jstatus;
        struct json_object* jmessage;
        if (!json_object_object_get_ex(jobj, "status", &jstatus)) {
            fprintf(stderr, "Missing status field in registration message\n");
            return;
        }

        if (json_object_get_type(jstatus) != json_type_string) {
            fprintf(stderr, "Expected 'status' field to be strings\n");
            return;
        }

        const char* status = json_object_get_string(jstatus);
        printf("\n Received registration-confirmation status: %s \n", status);
        const char* message = json_object_get_string(jmessage);
        if(message) {
            printf("Received status-message: %s \n", message);
        }
    }
      else {
        fprintf(stderr, "Unknown method: %s\n", method);
        return;
    }
}
