#include "../inc/server.h"

volatile sig_atomic_t stop_server = 0;

void sigint_handler(int n) {
    stop_server = 1;
    if (n == 0) { }
}

int main(int argc, char **argv) {
    
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Server started with process ID %d\n", getpid());
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;
    socklen_t addr_len = sizeof(client_addr);
    int port = atoi(argv[1]);
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // Bind socket to address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }
    
    // Register signal handler for SIGINT
    signal(SIGINT, sigint_handler);
    mx_deamon_start();
    // Display process ID of this app

    
    
    // Accept incoming connections and create new thread/process for each client
    while (!stop_server) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }
        if (pthread_create(&thread_id, NULL, client_handler, &client_socket) != 0) {
            perror("Error creating thread");
            continue;
        }
    }
    
    close(server_socket);
    exit(EXIT_SUCCESS);
    return 0;

}
