#include "../inc/uchat.h"

int connect_to_server(t_app_params app_params) {
    // Create socket
    app_params.client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Set server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(app_params.ip);    
    server_addr.sin_port = htons(app_params.port);             

    printf("Connecting to <IP: %s> <PORT: %i> ...\n", app_params.ip, app_params.port);
    
    return connect(app_params.client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
}



