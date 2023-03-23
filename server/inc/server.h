#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <json-c/json.h>


//structs

struct client {
    int socket_fd;
    struct client *next;
};

struct chat {
    int id;
    struct client *clients;
    struct chat *next;
};

extern struct chat *chat_list;

//

void *client_handler(void *arg);
void broadcast_connection(struct client *client_ptr, int chat_id, char* message_text);
struct chat *find_chat_by_id(int chat_id);
void add_chat(int chat_id);
void delete_chat(int chat_id);



