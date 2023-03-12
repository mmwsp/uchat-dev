#include "../inc/client.h"

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.57.128");
    server_addr.sin_port = htons(8080);
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }
    // sent login request on the server
    struct json_object* json_obj = json_object_new_object();
       json_object_object_add(json_obj, "method", json_object_new_string("login"));
       json_object_object_add(json_obj, "username", json_object_new_string("myusername"));
       json_object_object_add(json_obj, "password", json_object_new_string("mypassword"));

       // Send JSON object to server
       if (send_json_object(client_socket, json_obj) == -1) {
           exit(EXIT_FAILURE);
       }
    
    //receive response
    struct json_object* json_received_obj;
    if (receive_json_object(client_socket, &json_received_obj) == -1) {
        fprintf(stderr, "Failed to receive JSON message from server\n");
        exit(EXIT_FAILURE);
    }
    char *buffer = (char*)json_object_to_json_string(json_received_obj);
    parse_message(buffer);
    
    json_object_put(json_obj);
    json_object_put(json_received_obj);
    
    close(client_socket);
    return 0;
}
