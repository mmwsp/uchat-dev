#include "../inc/uchat.h"
#include "../inc/ui.h"

pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;
GAsyncQueue *message_queue = NULL;

t_application* app = NULL;
t_user* user = NULL;

t_app_params app_params = {0, NULL, 0};

int main(int argc, char const *argv[]) {
    if (argc != 3) {
		printf("Usage: ./uchat_client <ip> <port>\n");
		exit(EXIT_FAILURE);
	}

    app_params.ip = (char *)argv[1];     
    app_params.port = atoi(argv[2]);     
    app_params.client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(app_params.ip);    
    server_addr.sin_port = htons(app_params.port);             

    if (connect(app_params.client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        int connection_counter = 0;
        while (connect_to_server(app_params) == -1) {
            printf("Attemp #%i:\n", connection_counter + 1);
            if (connection_counter == 3) {
                exit(EXIT_FAILURE);
            }

            connection_counter++;
        }
    }
    

    app = mx_load_ui(&argc, (char***)&argv);

    gtk_widget_show_all( GTK_WIDGET(gtk_builder_get_object(app->builder, "main_window")) );

    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")));
    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_notification_box")));
    

    message_queue = g_async_queue_new();

    pthread_t thread;
    pthread_create(&thread, NULL, socket_listener_thread, NULL);
    pthread_mutex_init(&socket_mutex, NULL);


    g_idle_add(process_messages, NULL);

    gtk_main();

    g_object_unref(app->builder);
    close(app_params.client_socket);
    return 0;
}

void *socket_listener_thread(void *data) {
    if (data == NULL) { }

    char buffer[2048];
    while (true) {
        fd_set rfds;
        struct timeval tv;
        int retval;

        
        FD_ZERO(&rfds);
        FD_SET(app_params.client_socket, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        retval = select(app_params.client_socket + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) {
            perror("select()");
        } else if (retval) {
            pthread_mutex_lock(&socket_mutex);
            ssize_t n = recv(app_params.client_socket, buffer, 2048, 0);
            pthread_mutex_unlock(&socket_mutex);
            if (n > 0) {
                
                buffer[n] = '\0';

                
                struct json_object *json_received = json_tokener_parse(buffer);
                char *json_string = strdup(json_object_to_json_string(json_received));
                printf("\n 2 THRD : %s . ", json_string);
                g_async_queue_push(message_queue, json_string);

                
                json_object_put(json_received);
              
            }
        } else {
            
            
        }
    }
    return NULL;
}

gboolean process_messages(gpointer data) {
    if (data == NULL) { }

    while (true) {
        char *message = g_async_queue_try_pop(message_queue);
        if (message == NULL) {
            break;
        }
        parse_message_broadcast(message, NULL, NULL, NULL, NULL, NULL);
        
        free(message);
    }
    return true;
}


