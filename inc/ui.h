#pragma once

// ui 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gio/gio.h>
#include <curses.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>

#include "../inc/uchat.h"

//
typedef struct s_theme{
    int theme_id;
    char *theme_file;
}               t_theme;
//
/*typedef struct s_settings{
    int window_x;
    int window_y;
    int window_width;
    int window_height;

}               t_settings;*/

typedef struct s_sign_page {
    int type;
    // GtkWidget *page;
    // GtkWidget *login_entry;
    char *login_prev;
    // GtkWidget *name_entry;
    char *name_prev;
    // GtkWidget *password_entry;
    char *password_prev;
    // GtkWidget *repeat_password_entry;
    char *repeat_password_prev;
    // GtkWidget *change_button;
    // GtkWidget *enter_button;
}               t_sign_page;
//
typedef struct s_overlay{
    int type;
    //GObject *overlay_background;
    //GObject *overlay_main;
}               t_overlay;
// 
typedef struct s_chat_widget {
    GtkWidget *list_item;
    t_chat *chat;
    GtkWidget *main_button;
    GtkWidget *main_box;
    GtkWidget *profile_image;
    GtkWidget *name_count_box;
    GtkWidget *name_label;
    GtkWidget *count_label;
    GtkWidget *search_add_button; // CLCButton
    GtkWidget *search_add_image; 
    int sort; // time of last update
}               t_chat_widget;
// 
typedef struct s_chats_list_page{
    t_overlay *profile_overlay;
    t_overlay *add_chat_overlay;
    t_user* user;
    
    t_chat_widget **chatarr;
    int chatarr_count;
    int chatarr_size;

    t_chat_widget **searcharr;
    int searcharr_count;
    int searcharr_size;
}               t_chats_list_page;
//
typedef struct s_message_widget_attachment{
    int type;
    GtkWidget *main_image;
    GtkWidget *name_label;
    GtkWidget *message_label;
}               t_message_widget_attachment;
//
typedef struct s_message_widget{
    GtkWidget *list_item;
    t_message *message;
    int type;
    GtkWidget *profile_button;
    GtkWidget *message_button;
    GtkWidget *main_box;
    GtkWidget *message_box;
    GtkWidget *name_date_box;
    GtkWidget *message_label;
    int sort; // date of send 

    t_message_widget_attachment **message_attachment_boxes;
    int chat_count;
    int chat_size;
}               t_message_widget;
// 
typedef struct s_chat_page {
    t_overlay *profile_overlay;
    t_chat* chat;
    int send_type;
    t_message_widget *send_pointer;

    t_message_widget **messagearr;
    int messagearr_count;
    int messagearr_size;

    t_user **userarr;
    int userarr_count;
    int userarr_size;
}               t_chat_page;
// main app structure
typedef struct s_application {
    // t_settings *settings;
    int themeid;
    t_theme **themes;

    GtkBuilder* builder;

    t_sign_page *sign_in_page;
    t_sign_page *sign_up_page;
    t_chats_list_page *chats_list_page;
    t_chat_page *chat_page;

    int menu_type;
    GtkListBoxRow* menu_row;
}               t_application;
// 
// 
extern t_application *app;
extern pthread_mutex_t socket_mutex;
extern GAsyncQueue *message_queue;

//extern t_application app;

//Client main
gboolean mx_client_main( void );

t_application* mx_load_ui( int* argc, char*** argv );
//int ui_test(int argc, char** argv);



gboolean mx_change_b_1_signal( GtkWidget *widget, gpointer data );
gboolean mx_enter_b_1_signal( GtkWidget *widget, gpointer data );



gboolean mx_change_b_2_signal( GtkWidget *widget, gpointer data );
gboolean mx_enter_b_2_signal( GtkWidget *widget, gpointer data );



// search signals
gboolean mx_close_search_3_signal( GtkWidget *widget, gpointer data );
gboolean mx_search_3_signal( GtkWidget *widget, gpointer data );
gboolean mx_add_search_button_3_signal( GtkWidget *widget, gpointer data );
// chat selection signal
gboolean mx_chat_button_3_signal( GtkWidget *widget, GdkEventButton *event, gpointer data );

gboolean mx_pmenu_del_3_signal( GtkWidget *widget, gpointer data );
// chats list menu buttons
gboolean mx_add_chat_3_signal( GtkWidget *widget, gpointer data );
gint mx_chat_list_sort_3( GtkListBoxRow *row1, GtkListBoxRow *row2, gpointer data );
gint mx_search_list_sort_3( GtkListBoxRow *row1, GtkListBoxRow *row2, gpointer data );
// 1
gboolean mx_add_chat_3_signal( GtkWidget *widget, gpointer data );
gboolean mx_show_profile_3_signal( GtkWidget *widget, gpointer data );
gboolean mx_show_search_3_signal( GtkWidget *widget, gpointer data );
// 



// top box signals
gboolean mx_exit_chat_4_signal( GtkWidget *widget, gpointer data );
gboolean mx_show_chat_profile_4_signal( GtkWidget *widget, gpointer data );
// message selection signal
gboolean mx_message_button_4_signal( GtkWidget *widget, GdkEventButton *event, gpointer data );
gboolean mx_profile_button_4_signal( GtkWidget *widget, gpointer data );
gboolean mx_pmenu_del_4_signal( GtkWidget *widget, gpointer data );
gboolean mx_pmenu_edit_4_signal( GtkWidget *widget, gpointer data );
gboolean mx_pmenu_edit_send_4_signal( GtkWidget *widget, gpointer data );

gint mx_message_list_sort_4( GtkListBoxRow *row1, GtkListBoxRow *row2, gpointer data );
// bottom box signals
gboolean mx_attach_4_signal( GtkWidget *widget, gpointer data );
gboolean mx_send_4_signal( GtkWidget *widget, gpointer data );



// =============================================================================== overlay signals 
// chat list page // profile 
gboolean mx_overlay_1_exit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_1_edit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_1_sign_out_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_1_del_account_signal( void/*GtkWidget *widget, gpointer data*/ );
// edit profile 
gboolean mx_overlay_2_exit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_2_save_changes_signal( void/*GtkWidget *widget, gpointer data*/ );
// add chat 
gboolean mx_overlay_3_exit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_3_add_chat_signal( void/*GtkWidget *widget, gpointer data*/ );
// chat page // chat profile 
gboolean mx_overlay_4_exit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_4_edit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_4_leave_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_4_delete_signal( void/*GtkWidget *widget, gpointer data*/ );
// edit chat 
gboolean mx_overlay_5_exit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_5_save_changes_signal( void/*GtkWidget *widget, gpointer data*/ );
// user  profile 
gboolean mx_overlay_6_exit_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_6_send_message_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_6_ban_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_6_promote_signal( void/*GtkWidget *widget, gpointer data*/ );
gboolean mx_overlay_6_del_signal( void/*GtkWidget *widget, gpointer data*/ );



/* ======================================================================================== ui functions */
int mx_load_chat( t_chat* chat, int ltype );
int mx_add_chat_message( t_message *message, t_user *user, int type ); // 
int mx_del_chat_message( GtkListBox *lbox, GtkWidget *item, int mid ); // only gtk struct 
int mx_change_chat_message( GtkListBox *lbox, GtkWidget *item, int mid ); // only gtk struct 
// chats list 
int mx_load_chat_list( t_user* user );
int mx_add_chat( t_chat *chat ); // + 
int mx_del_chat( GtkListBox *lbox, GtkListBoxRow *item, int cid );
int mx_change_chat( GtkListBox *lbox, GtkListBoxRow *item, int cid );
//
int mx_show_search_list( t_chat **chat );
int mx_add_search( t_chat *chat );
// notifications 
gboolean mx_show_notification( char* notificationstr, int seconds );
gboolean mx_hide_notification( void );


void *socket_listener_thread(void *data);
gboolean process_messages(gpointer data);
void parse_message_broadcast(char* buffer, t_user *user1, t_user **users1, t_chat *chat1, t_message *message1, t_message **messages1);


