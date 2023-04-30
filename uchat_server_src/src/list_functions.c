#include "../inc/server.h"

struct list_node *list[LIST_SIZE] = { NULL };

int list_function(int user_id) {
    return user_id % LIST_SIZE;
}

void add_to_list(int user_id, int socket_fd) {
    int index = list_function(user_id);
    struct list_node *node = malloc(sizeof(struct list_node));
    node->user_id = user_id;
    node->socket_fd = socket_fd;
    printf("\nsocket desc: %i \n", socket_fd);
    printf("\nuId : %i \n", user_id);
    printf("\n set uIs : %i", node->user_id);
    node->next = list[index];
    list[index] = node;
}

void remove_from_list(int user_id) {
    int index = list_function(user_id);
    struct list_node *prev = NULL;
    struct list_node *curr = list[index];
    while (curr != NULL) {
        if (curr->user_id == user_id) {
            if (prev == NULL) {
                list[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int get_socket_fd_from_list(int user_id) {
    int index = list_function(user_id);
    struct list_node *curr = list[index];
    printf("\n INSIDE GET SOCKET_FD FROM LIST. ");
    while (curr != NULL) {
        if (curr->user_id == user_id) {
            return curr->socket_fd;
        }
        curr = curr->next;
    }
    return -1;  // User not found
}
