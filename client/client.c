#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[] = "\n  salam from client  \n";
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
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
    
    // Send message to server
    if (send(client_socket, message, strlen(message), 0) == -1) {
        perror("Error sending message to server");
        exit(EXIT_FAILURE);
    }
    
    // Receive response from server
    if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving response from server");
        exit(EXIT_FAILURE);
    }
    printf("Received message from server: %s\n", buffer);
    
    close(client_socket);
    return 0;
}
