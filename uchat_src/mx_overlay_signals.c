#include "../inc/ui.h"



// ======================================================================================= chat list overlay signals
gboolean mx_overlay_1_exit_signal( void) { 
    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 

    return true;
}
gboolean mx_overlay_1_edit_signal( void ) { 

    gtk_entry_set_text(
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_uname_entry_2")), 
        app->chats_list_page->user->u_name);
    gtk_entry_set_text(
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_ubio_entry_2")), 
        app->chats_list_page->user->u_bio);
    
    gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
    
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "edit_profile_3", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN );
    return true;
}
gboolean mx_overlay_1_sign_out_signal( void ) { 

    GtkWidget *row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "chat_list_box_3")), 0 ) );
    while ( row != NULL) {
        
        gtk_widget_destroy( GTK_WIDGET(row) );

        row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "chat_list_box_3")), 0 ) );
    }
    row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
    while ( row != NULL) {
        
        gtk_widget_destroy( GTK_WIDGET(row) );

        row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
    }
    user = NULL;
    
    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
  
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
        "sign_up_page", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );
    return true;
}
gboolean mx_overlay_1_del_account_signal( void ) { 

    if ( mx_del_user_request(app->chats_list_page->user->u_id) ) {
        GtkWidget *row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "chat_list_box_3")), 0 ) );
        while ( row != NULL) {
            
            gtk_widget_destroy( GTK_WIDGET(row) );

            row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "chat_list_box_3")), 0 ) );
        }
        row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
        while ( row != NULL) {
            
            gtk_widget_destroy( GTK_WIDGET(row) );

            row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
        }
        user = NULL;
        gtk_stack_set_visible_child_full( 
            GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
            "sign_up_page", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );

        return true;
    }
    
    return true;
}



gboolean mx_overlay_2_exit_signal( void) { 
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "profile_3", GTK_STACK_TRANSITION_TYPE_SLIDE_UP );
    return true;
}
gboolean mx_overlay_2_save_changes_signal( void ) {

    const gchar* text1 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_uname_entry_2")) );
    const gchar* text2 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_ubio_entry_2")) );
    if ((text1 != NULL && strlen((const char *)text1) > 0) 
        || (text2 != NULL && strlen((const char *)text2) > 0)) { // 
        if (mx_change_user_data_request(app->chats_list_page->user->u_id, (char *)text1, (char *)text2)) { 
            app->chats_list_page->user->u_name = (char*)text1;
            app->chats_list_page->user->u_bio = (char*)text2;
            user->u_name = (char*)text1;
            user->u_bio = (char*)text2;
            gtk_label_set_label(
                GTK_LABEL(gtk_builder_get_object(app->builder, "o_profile_status_1")), 
                app->chats_list_page->user->u_name);
            gtk_label_set_label(
                GTK_LABEL(gtk_builder_get_object(app->builder, "o_profile_bio_1")), 
                app->chats_list_page->user->u_bio);
            gtk_stack_set_visible_child_full( 
                GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
                "profile_3", GTK_STACK_TRANSITION_TYPE_SLIDE_UP ); 
        }
    }
    return true;
}



gboolean mx_overlay_3_exit_signal( void ) { 

    gtk_entry_set_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_cname_entry_3")), "" );
    gtk_entry_set_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_cdesc_entry_3")), "" );

    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
    
    return true;
}
gboolean mx_overlay_3_add_chat_signal( void ) {

    const gchar* text1 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_cname_entry_3")) );
    const gchar* text2 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_cdesc_entry_3")) );
    if (text1 != NULL && strlen((char*)text1) > 0) { 
     
        t_chat *new_chat = mx_request_add_group((char *)text1, (char *)text2, app->chats_list_page->user->u_id);
        if (new_chat != NULL) {
            mx_add_chat( new_chat );
            
            app->chats_list_page->user->u_chatarr[app->chats_list_page->user->u_chatarr_count] = new_chat;
            app->chats_list_page->user->u_chatarr_count++;
        }
    }
    return true;
}



/* ============================================================================================================ */
gboolean mx_overlay_4_exit_signal( void/*GtkWidget *widget, gpointer data*/ ) { // chat profile 

//if (widget == NULL && data == NULL) {}
    // 
    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
    return true;
}
gboolean mx_overlay_4_edit_signal( void/*GtkWidget *widget, gpointer data*/ ) { 

//if (widget == NULL && data == NULL) {}
    // 
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "edit_chat_4", GTK_STACK_TRANSITION_TYPE_SLIDE_UP );
    return true;
}
gboolean mx_overlay_4_leave_signal( void/*GtkWidget *widget, gpointer data*/ ) { 

//if (widget == NULL && data == NULL) {}
    // leave chat request
    if ( mx_request_leave_chat(app->chat_page->chat->c_id, app->chats_list_page->user->u_id) ) {
        mx_del_chat( GTK_LIST_BOX( gtk_builder_get_object(app->builder, "chat_list_box_3") ),
             NULL, app->chat_page->chat->c_id);
        // delete from chat list & exit chat 
        GtkWidget *row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
        while ( row != NULL) {
            gtk_widget_destroy( GTK_WIDGET(row) );
            row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
        }
        app->chat_page->chat = NULL;
        gtk_stack_set_visible_child_full( 
            GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
            "chat_list_page", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT );
    }
    return true;
}
gboolean mx_overlay_4_delete_signal( void/*GtkWidget *widget, gpointer data*/ ) { 

//if (widget == NULL && data == NULL) {}
    // delete chat request
    if ( mx_del_chat_request(app->chat_page->chat->c_id) ) {
        mx_del_chat( GTK_LIST_BOX( gtk_builder_get_object(app->builder, "chat_list_box_3") ),
             NULL, app->chat_page->chat->c_id);
        // delete from chat list & exit chat
        GtkWidget *row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
        while ( row != NULL) {
            gtk_widget_destroy( GTK_WIDGET(row) );
            row = GTK_WIDGET(
            gtk_list_box_get_row_at_index( 
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 0 ) );
        }
        app->chat_page->chat = NULL;
        gtk_stack_set_visible_child_full( 
            GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
            "chat_list_page", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT );
    }
    return true;
}



gboolean mx_overlay_5_exit_signal( void/*GtkWidget *widget, gpointer data*/ ) { // edit chat 

// if (widget == NULL && data == NULL) {}
    // 
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "chat_profile_4", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN );
    return true;
}
gboolean mx_overlay_5_save_changes_signal( void/*GtkWidget *widget, gpointer data*/ ) {

// if (widget == NULL && data == NULL) {}
    // 
    const gchar* text1 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_cname_entry_5")) );
    const gchar* text2 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "o_cdesk_entry_5")) );
    if ((text1 != NULL && strlen((const char *)text1) > 0) 
        || (text2 != NULL && strlen((const char *)text2) > 0)) {
        //request edit_chat
        if (mx_edit_chat_request(app->chat_page->chat->c_id, (char *)text1, (char *)text2)) {
            app->chat_page->chat->c_name = (char*)text1;
            app->chat_page->chat->c_description = (char*)text2;
            
            gtk_label_set_label(
                GTK_LABEL(gtk_builder_get_object(app->builder, "o_chat_name_4")), 
                app->chat_page->chat->c_name);
            gtk_label_set_label(
                GTK_LABEL(gtk_builder_get_object(app->builder, "o_chat_desk_4")), 
                app->chat_page->chat->c_description);
            gtk_stack_set_visible_child_full( 
                GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
                "chat_profile_4", GTK_STACK_TRANSITION_TYPE_SLIDE_UP );
        }
    }

    return true;
}



gboolean mx_overlay_6_exit_signal( void/*GtkWidget *widget, gpointer data*/ ) { // user profile 

// if (widget == NULL && data == NULL) {}
    // 
    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
    return true;
}
gboolean mx_overlay_6_send_message_signal( void/*GtkWidget *widget, gpointer data*/ ) {

// if (widget == NULL && data == NULL) {}
    // create chat 1vs1 user request
    if ( false ) {
        
    }

    return true;
}
gboolean mx_overlay_6_ban_signal( void/*GtkWidget *widget, gpointer data*/ ) { // ? 

// if (widget == NULL && data == NULL) {}
    // 
    
    return true;
}
gboolean mx_overlay_6_promote_signal( void/*GtkWidget *widget, gpointer data*/ ) { // ? 

// if (widget == NULL && data == NULL) {}
    // 
    
    return true;
}
gboolean mx_overlay_6_del_signal( void/*GtkWidget *widget, gpointer data*/ ) { 

// if (widget == NULL && data == NULL) {}
    // delete chat with user (1vs1) request
    if ( false ) {

    }
    // 
    
    return true;
}


