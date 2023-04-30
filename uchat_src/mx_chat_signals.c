#include "../inc/ui.h"

// top box signals
gboolean mx_exit_chat_4_signal( GtkWidget *widget, gpointer data ) {
    if (widget == NULL && data == NULL) {}
    
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

    // change page
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
        "chat_list_page", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT );
    return true;
}
gboolean mx_show_chat_profile_4_signal( GtkWidget *widget, gpointer data ) { // chat profile

if (widget == NULL && data == NULL) {}

    if ( app->chat_page->chat->ct_type == 2 ) { 
   
        gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 

        gtk_stack_set_visible_child_full( 
            GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
            "chat_profile_4", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );

        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "o_chat_name_4")), 
            app->chat_page->chat->c_name );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "o_chat_status_4")), 
            "userarr count" );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "o_chat_desk_4")), 
            app->chat_page->chat->c_description );
        if ( app->chat_page->chat->cus_type == 3 ) {
            gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_edit_button_4")) );
            gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_del_button_4")) );
            gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_leave_button_4")) );
        }
        else {
            gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_edit_button_4")) );
            gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_del_button_4")) );
            gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_leave_button_4")) );
        }
    }
    else if ( app->chat_page->chat->ct_type == 1 ) { 

        gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
       
        gtk_stack_set_visible_child_full( 
            GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
            "user_profile_4", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "o_user_name_6")), 
            app->chat_page->chat->c_name );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "o_user_status_6")), 
            "online " );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "o_user_bio_6")), 
            app->chat_page->chat->c_description );
        gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_del_button_6")) );
        gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_send_mes_button_6")) );
    }
    
    return true;
}



// message selection signal
gboolean mx_message_button_4_signal( GtkWidget *widget, GdkEventButton *event, gpointer data ) {

    if (widget == NULL && data == NULL) {}
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) { 

        int i = 0;
        for ( ; i < app->chat_page->messagearr_count; i++) {
            if ( widget == app->chat_page->messagearr[i]->message_button ) {

                break;
            }
        }
        GtkWidget *menu = gtk_menu_new();
        bool show = false;
        if ( app->chat_page->chat->cus_type == 3 
        || app->chat_page->messagearr[i]->message->u_id == app->chats_list_page->user->u_id) {
            show = true;
            GtkWidget *item1 = gtk_menu_item_new_with_label("Delete Message");
            g_signal_connect( 
                item1, "activate", G_CALLBACK(mx_pmenu_del_4_signal), 
                app->chat_page->messagearr[i] );
            gtk_menu_attach( GTK_MENU(menu), item1, 0,1,0,1 );
            gtk_widget_show( item1 );
        }
        if (app->chat_page->messagearr[i]->message->u_id == app->chats_list_page->user->u_id) {
            show = true;
            GtkWidget *item2 = gtk_menu_item_new_with_label( "Edit Message" );
            g_signal_connect( 
                item2, "activate", G_CALLBACK(mx_pmenu_edit_4_signal), 
                app->chat_page->messagearr[i] );
            gtk_menu_attach( GTK_MENU(menu), item2, 0,1,1,2 );
            gtk_widget_show( item2 );
        }
        gtk_widget_show( menu );
        GtkWidget* wx = gtk_widget_get_parent(widget);
        while ( !GTK_IS_LIST_BOX_ROW(wx) ) {
            wx = gtk_widget_get_parent(wx);
        }
        app->menu_row = GTK_LIST_BOX_ROW( wx );
        app->menu_type = 1;
        if (show) {
            gtk_menu_popup_at_pointer( GTK_MENU(menu), (GdkEvent *)event );
        }
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) { 

    }
    return true;
}

gboolean mx_profile_button_4_signal( GtkWidget *widget, gpointer data ) {
write(1, "cls cs s2", 10);
if (widget == NULL && data == NULL) {}
    gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 

    int i = 0;
    for ( ; i < app->chat_page->messagearr_count; i++) {
        if ( widget == app->chat_page->messagearr[i]->profile_button ) {
            t_user* user1 = mx_get_user_request(app->chat_page->messagearr[i]->message->u_id);
            gtk_label_set_label(
                GTK_LABEL(gtk_builder_get_object(app->builder, "o_user_name_6")), 
                user1->u_name);
            gtk_label_set_label(
                GTK_LABEL(gtk_builder_get_object(app->builder, "o_user_bio_6")), 
                user1->u_bio);
            if ( false ) {
                gtk_widget_hide( 
                    GTK_WIDGET(gtk_builder_get_object(app->builder, "o_send_mes_button_6")) );
                gtk_button_set_label( 
                    GTK_BUTTON(gtk_builder_get_object(app->builder, "o_send_mes_button_6")), 
                    "Send Message" );
                gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_del_button_6")) );
                gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_send_mes_button_6")) );
            }
            else {
                gtk_widget_show( 
                    GTK_WIDGET(gtk_builder_get_object(app->builder, "o_send_mes_button_6")) );
                gtk_button_set_label( 
                    GTK_BUTTON(gtk_builder_get_object(app->builder, "o_send_mes_button_6")), 
                    "Create Chat With User" ); 
                gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_del_button_6")) );
                gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_send_mes_button_6")) );
            }
            gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_del_button_6")) ); 
        }
    }

    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "user_profile_4", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );
    return true;
}
gboolean mx_pmenu_del_4_signal( GtkWidget *widget, gpointer data ) {

if (widget == NULL && data == NULL) {}
    t_message_widget *mw = data;
    if (mx_delete_message_request(mw->message)) {
        mx_del_chat_message(
            GTK_LIST_BOX( gtk_builder_get_object(app->builder, "main_box_4") ),
            mw->list_item, mw->message->m_id);

        for (int i = 0; i < app->chat_page->chat->c_messagearr_count; i++) {
            if ( app->chat_page->chat->c_messagearr[i]->m_id == mw->message->m_id ) {
                for (int j = i; j < app->chat_page->chat->c_messagearr_count-1; j++) {
                    app->chat_page->chat->c_messagearr[j] = app->chat_page->chat->c_messagearr[j+1];
                }
                app->chat_page->chat->c_messagearr_count--;
            }
        }
    }
    else {
        mx_show_notification("Error", 5);
        return false;
    }
    
    return true;
}
gboolean mx_pmenu_edit_4_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL && data == NULL) {}
    t_message_widget *mw = data;
    
    for (int i = 0; i < app->chat_page->chat->c_messagearr_count; i++) {
        if ( app->chat_page->chat->c_messagearr[i]->m_id == mw->message->m_id ) {

            gtk_entry_set_text(
                GTK_ENTRY(gtk_builder_get_object(app->builder, "send_entry_4")), 
                mw->message->m_text );

            gtk_revealer_set_reveal_child(
                GTK_REVEALER(gtk_builder_get_object(app->builder, "message_revealer_4")),
                TRUE );
            gtk_label_set_text(
                GTK_LABEL(gtk_builder_get_object(app->builder, "message_label_4")),
                " Edit message ... " );
            
            app->chat_page->send_type = 2;
            app->chat_page->send_pointer = mw;
        }
    }
    return true;
}
gboolean mx_pmenu_edit_send_4_signal( GtkWidget *widget, gpointer data ) {
    if (widget == NULL && data == NULL) {}
    
    return true;
}
gint mx_message_list_sort_4( GtkListBoxRow *row1, GtkListBoxRow *row2, gpointer data ) {
    if (row1 == NULL && row2 == NULL && data == NULL) {}
    
    return 0;
}


// bottom box signals
gboolean mx_attach_4_signal( GtkWidget *widget, gpointer data ) {

    if (widget == NULL && data == NULL) {}

    
    return true;
}
gboolean mx_send_4_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL && data == NULL) {}

    if (app->chat_page->send_type == 1) {
        const gchar* text = gtk_entry_get_text( 
            GTK_ENTRY(gtk_builder_get_object(app->builder, "send_entry_4")) );
        if (text != NULL && strlen((char*)text) > 0) {
            
            t_message* m1 = (t_message*)malloc( sizeof(t_message) );
                m1->u_id = app->chats_list_page->user->u_id;
                m1->c_id = app->chat_page->chat->c_id;
                m1->m_text = (char*)text;
                m1->m_is_edited = false;
                m1->m_type = 1;
                m1->m_form = 1;
                m1->m_m_id = 0;

            if (app->chat_page->chat->c_messagearr == NULL) {

            }
            app->chat_page->chat->c_messagearr[app->chat_page->chat->c_messagearr_count] = m1;
            app->chat_page->chat->c_messagearr_count++;

            m1 = mx_send_message(m1);
            mx_add_chat_message( m1, NULL, 1 );
        }
        gtk_entry_set_text( 
            GTK_ENTRY(gtk_builder_get_object(app->builder, "send_entry_4")), "" );
    }
    else if (app->chat_page->send_type == 2) {
        const gchar* text = gtk_entry_get_text( 
            GTK_ENTRY(gtk_builder_get_object(app->builder, "send_entry_4")) );
        if (text != NULL && strlen((char*)text) > 0) {
            
            app->chat_page->send_pointer->message->m_text = (char *)text;

            mx_change_chat_message(
                GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")),
                app->chat_page->send_pointer->list_item, app->chat_page->send_pointer->message->m_id );
        }
        gtk_revealer_set_reveal_child(
            GTK_REVEALER(gtk_builder_get_object(app->builder, "message_revealer_4")),
            FALSE );
        gtk_entry_set_text( 
            GTK_ENTRY(gtk_builder_get_object(app->builder, "send_entry_4")), "" );
        app->chat_page->send_type = 1;
    }

    return true;
}


