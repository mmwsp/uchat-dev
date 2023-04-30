#include "../inc/ui.h"

// search signals
gboolean mx_close_search_3_signal( GtkWidget *widget, gpointer data ) {
    if (widget == NULL && data == NULL) {}

    gtk_entry_set_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "search_entry_3")), "" );
 
    GtkWidget *row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "search_list_3")), 0 ) );
    while ( row != NULL) {
        gtk_widget_destroy( GTK_WIDGET(row) );
        row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "search_list_3")), 0 ) );
    }

    app->chats_list_page->searcharr = NULL;
    app->chats_list_page->searcharr_count = 0;
    app->chats_list_page->searcharr_size = 0;

    gtk_stack_set_visible_child_full( 
        GTK_STACK( gtk_builder_get_object(app->builder, "chat_list_stack") ), 
        "chat_list", GTK_STACK_TRANSITION_TYPE_SLIDE_UP );

    gtk_revealer_set_reveal_child( 
        GTK_REVEALER(gtk_builder_get_object(app->builder, "search_revealer_3")), false ); 
    return true;
}
gboolean mx_search_3_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL && data == NULL) {}

    GtkWidget *row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "search_list_3")), 0 ) );
    while ( row != NULL) {
        gtk_widget_destroy( GTK_WIDGET(row) );
        row = GTK_WIDGET(
        gtk_list_box_get_row_at_index( 
            GTK_LIST_BOX(gtk_builder_get_object(app->builder, "search_list_3")), 0 ) );
    }

    app->chats_list_page->searcharr = NULL;
    app->chats_list_page->searcharr_count = 0;
    app->chats_list_page->searcharr_size = 0;


    const gchar* text2 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "search_entry_3")) );
    if (text2 != NULL && strlen((char *)text2) > 0) { 
         
        t_chat** chats = mx_search_chat((char *)text2);

        mx_show_search_list( chats);
    }

    gtk_stack_set_visible_child_full( 
        GTK_STACK( gtk_builder_get_object(app->builder, "chat_list_stack") ), 
        "search_list", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN );
 
    
    return true;
}
gboolean mx_add_search_button_3_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL && data == NULL) {}
    int i = 0;
    for ( ; i < app->chats_list_page->searcharr_count; i++) {
        if ( app->chats_list_page->searcharr[i]->search_add_button == widget ) {
            break;
        }
    }

    t_chat *chat2 = mx_join_chat(app->chats_list_page->user->u_id, 
        app->chats_list_page->searcharr[i]->chat->c_id, 
        app->chats_list_page->searcharr[i]->chat->ct_type);

    mx_add_chat( chat2 );

    app->chats_list_page->user->u_chatarr[app->chats_list_page->user->u_chatarr_count] = chat2;
    app->chats_list_page->user->u_chatarr_count++;

    gtk_stack_set_visible_child_full( 
        GTK_STACK( gtk_builder_get_object(app->builder, "chat_list_stack") ), 
        "chat_list", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN );
    return true;
}




gboolean mx_chat_button_3_signal( GtkWidget *widget, GdkEventButton *event, gpointer data ) {
if (widget == NULL && data == NULL) {}
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) { 
 
        int i = 0;
        for ( ; i < app->chats_list_page->chatarr_count; i++) {
            if ( widget == app->chats_list_page->chatarr[i]->main_button ) {
  
                break;
            }
        }
     
        GtkWidget *menu = gtk_menu_new();
        GtkWidget *item1 = NULL;

        if (app->chats_list_page->chatarr[i]->chat->cus_type == 3) {
            item1 = gtk_menu_item_new_with_label( "Delete Chat" );
        }
        else {
            item1 = gtk_menu_item_new_with_label( "Leave Chat" );
        }
        
        g_signal_connect( item1, "activate", G_CALLBACK(mx_pmenu_del_3_signal), app->chats_list_page->chatarr[i] );
        
        gtk_menu_attach( GTK_MENU(menu), item1, 0,1,0,1 );
        gtk_widget_show( menu );
        gtk_widget_show( item1 );
        app->menu_row = GTK_LIST_BOX_ROW( gtk_widget_get_parent(widget) );
        app->menu_type = 1;
        gtk_menu_popup_at_pointer( GTK_MENU(menu), (GdkEvent *)event );
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) { 
   
        int i = 0;
        for ( ; i < app->chats_list_page->chatarr_count; i++) {
            if ( widget == app->chats_list_page->chatarr[i]->main_button ) {

                break;
            }
        }
        
        mx_chat_request(app->chats_list_page->chatarr[i]->chat);

        mx_load_chat( app->chats_list_page->chatarr[i]->chat, 0 );

        gtk_stack_set_visible_child_full( 
            GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
            "chat_page", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );
    }
    return true;
}
gboolean mx_pmenu_del_3_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL && data == NULL) {}

    t_chat_widget *cw = data;

    if (cw->chat->cus_type == 3) {
        if (mx_del_chat_request(cw->chat->c_id)) {

            mx_del_chat(
                GTK_LIST_BOX( gtk_builder_get_object(app->builder, "chat_list_box_3") ),
                GTK_LIST_BOX_ROW(cw->list_item), cw->chat->c_id);

            for (int i = 0; i < user->u_chatarr_count; i++) {
                if ( user->u_chatarr[i]->c_id == cw->chat->c_id ) {
  
                    for (int j = i; j < user->u_chatarr_count-1; j++) {
                        user->u_chatarr[j] = user->u_chatarr[j+1];
                    }
                    user->u_chatarr_count--;
                }
            }
        }
    }
    else {
        if (mx_request_leave_chat(cw->chat->c_id, app->chats_list_page->user->u_id)) {
        
            mx_del_chat(
                GTK_LIST_BOX( gtk_builder_get_object(app->builder, "chat_list_box_3") ),
                GTK_LIST_BOX_ROW(cw->list_item), cw->chat->c_id);
         
            for (int i = 0; i < user->u_chatarr_count; i++) {
                if ( user->u_chatarr[i]->c_id == cw->chat->c_id ) {
                  
                    for (int j = i; j < user->u_chatarr_count-1; j++) {
                        user->u_chatarr[j] = user->u_chatarr[j+1];
                    }
                    user->u_chatarr_count--;
                }
            }
        }
    }

    return true;
}
gint mx_chat_list_sort_3( GtkListBoxRow *row1, GtkListBoxRow *row2, gpointer data ) {
    if (row1 == NULL && row2 == NULL && data == NULL) {}
    int sort1 = 0;
    int sort2 = 0;
    for (int i = 0; i < app->chats_list_page->chatarr_count; i++) {
        
        if ( GTK_LIST_BOX_ROW(app->chats_list_page->chatarr[i]->list_item) == row1) {
            sort1 = app->chats_list_page->chatarr[i]->sort;
        }
        if ( GTK_LIST_BOX_ROW(app->chats_list_page->chatarr[i]->list_item) == row2) {
            sort2 = app->chats_list_page->chatarr[i]->sort;
        }
    }

    return sort1 - sort2 * -1;
}
gint mx_search_list_sort_3( GtkListBoxRow *row1, GtkListBoxRow *row2, gpointer data ) {
    if (row1 == NULL && row2 == NULL && data == NULL) {}
    
    return 0;
}



gboolean mx_add_chat_3_signal( GtkWidget *widget, gpointer data ) {
    if (widget == NULL && data == NULL) {}
    
    gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) ); 
  
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "add_chat_3", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN );

    return true;
}
gboolean mx_show_profile_3_signal( GtkWidget *widget, gpointer data ) {
    if (widget == NULL && data == NULL) {}

    gtk_label_set_label(
        GTK_LABEL(gtk_builder_get_object(app->builder, "o_profile_name_1")), 
        app->chats_list_page->user->u_login);
    gtk_label_set_label(
        GTK_LABEL(gtk_builder_get_object(app->builder, "o_profile_status_1")), 
        app->chats_list_page->user->u_name);
    gtk_label_set_label(
        GTK_LABEL(gtk_builder_get_object(app->builder, "o_profile_bio_1")), 
        app->chats_list_page->user->u_bio);

    gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_overlay")) );
   
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "o_overlay")), 
        "profile_3", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN );
    
    return true;
}
gboolean mx_show_search_3_signal( GtkWidget *widget, gpointer data ) {
    if (widget == NULL && data == NULL) {}
   
    gtk_revealer_set_reveal_child( 
        GTK_REVEALER(gtk_builder_get_object(app->builder, "search_revealer_3")), true );
    return true;
}


