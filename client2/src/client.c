#include "../inc/client.h"

bool logged_in = false;
UserInfo current_user = {0, NULL};

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
    server_addr.sin_addr.s_addr = inet_addr("192.168.43.98");
    server_addr.sin_port = htons(8083);
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    //registration(client_socket);
    //login(client_socket);
    init_user(client_socket);
    
    //listening for incoming messages
    struct json_object* json_received_obj;
    while (1) {
        if (receive_json_object(client_socket, &json_received_obj) == -1) {
            fprintf(stderr, "Failed to receive JSON message from server(main)\n");
            exit(EXIT_FAILURE);
        }
        char* buffer = (char*)json_object_to_json_string(json_received_obj);
        printf("\n RECEIVED: %s .", buffer);
        parse_message(buffer);
        free(buffer);
    }

    json_object_put(json_received_obj);
    close(client_socket);
    free(current_user.login);
    return 0;
}
