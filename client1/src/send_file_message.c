#include "../inc/client.h"

static int get_size(const char* filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}

void send_file_message(int client_socket) {
    char *base64_data;
    size_t base64_len;
    int file_size = get_size("file.txt");
    
    if (encodeFile("file.txt", &base64_data, &base64_len) == 0) {
        // Use base64_data and base64_len to send file in JSON
        int message_type = 3;
        char *message = "Text to message with file.";
        struct json_object* json_obj = json_object_new_object();
        json_object_object_add(json_obj, "method", json_object_new_string("message"));
        json_object_object_add(json_obj, "message_type", json_object_new_int(message_type));
        json_object_object_add(json_obj, "user_id",   json_object_new_int(current_user.user_id));
        json_object_object_add(json_obj, "chat_id", json_object_new_int(1)); //we need to throw here chat id, that we have saved from chat_init response
        json_object_object_add(json_obj, "message_text", json_object_new_string(message));
        json_object_object_add(json_obj, "message_form", json_object_new_int(1));
        json_object_object_add(json_obj, "message_file", json_object_new_string(base64_data));
        json_object_object_add(json_obj, "file_name", json_object_new_string("file.txt"));
        json_object_object_add(json_obj, "file_size", json_object_new_int(file_size));
        printf("\n ENCODED base64: %s ", base64_data);
        printf("Encoded data length: %zu\n", base64_len);
        if (send_json_object(client_socket, json_obj) == -1) {
            exit(EXIT_FAILURE);
        }
        // Free memory allocated by decodeFile
        free(base64_data);
        json_object_put(json_obj);
    } else {
        printf("\n ERROR DECODING.");
    }
}
