#include "../inc/server.h"

struct chat *find_chat_by_id(int chat_id) {
    struct chat *chat_ptr = chat_list;
    while (chat_ptr != NULL) {
        if (chat_ptr->id == chat_id) {
            return chat_ptr;
        }
        chat_ptr = chat_ptr->next;
    }
    return NULL; // No chat found with the given ID
}

// Adds a new chat to the chat list
void add_chat(int chat_id) {
    // Create a new chat struct
    struct chat *new_chat = malloc(sizeof(struct chat));
    new_chat->id = chat_id;
    new_chat->clients = NULL;
    new_chat->next = NULL;
    
    // Find the last chat in the list and set its next pointer to the new chat
    struct chat *chat_ptr = chat_list;
    while (chat_ptr->next != NULL) {
        chat_ptr = chat_ptr->next;
    }
    chat_ptr->next = new_chat;
}

// Deletes a chat from the chat list with the given ID
void delete_chat(int chat_id) {
    struct chat *chat_ptr = chat_list;
    struct chat *prev_chat_ptr = NULL;

    // Find the chat with the given ID
    while (chat_ptr != NULL && chat_ptr->id != chat_id) {
        prev_chat_ptr = chat_ptr;
        chat_ptr = chat_ptr->next;
    }

    // If the chat was found, remove it from the list
    if (chat_ptr != NULL) {
        // Update the previous chat's next pointer to skip the deleted chat
        if (prev_chat_ptr != NULL) {
            prev_chat_ptr->next = chat_ptr->next;
        } else {
            // If the deleted chat was the first one in the list, update the chat_list pointer to point to the next chat
            chat_list = chat_ptr->next;
        }
    }
    free(chat_ptr);
    
}
