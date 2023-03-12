#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <json-c/json.h>

#define BUFFER_SIZE 1024

//JSON
int send_json_object(int client_socket, struct json_object* json_obj);
int receive_json_object(int client_socket, struct json_object** json_obj);
void parse_message(char* buffer);
