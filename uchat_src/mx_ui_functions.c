#include "../inc/uchat.h"
#include "../inc/ui.h"

//int number = 0;


// parse received message
void parse_message_broadcast(char* buffer, t_user *user1, t_user **users1, t_chat *chat1, t_message *message1, t_message **messages1) {
    struct json_object* jobj;
    enum json_tokener_error jerr = json_tokener_success;

    if (users1 == NULL || user1 == NULL || messages1 == NULL) { }

    jobj = json_tokener_parse_verbose(buffer, &jerr);
    if (jerr != json_tokener_success) {
        fprintf(stderr, "Error parsing JSON message: %s\n", json_tokener_error_desc(jerr));
        return;
    }

    //check type of receiving data
    enum json_type jtype = json_object_get_type(jobj);
    if (jtype != json_type_object) {
        fprintf(stderr, "Expected JSON object, received %s\n", json_type_to_name(jtype));
        return;
    }

    //take method from json
    struct json_object* jmethod;
    json_object_object_get_ex(jobj, "method", &jmethod);
    //value of method put in string
    const char* method = json_object_get_string(jmethod);
    printf("\n Received BROADCAST message with method: %s\n", method);

    if (strcmp(method, "chat_delete") == 0) {
        return;
        struct json_object* jchat_id;
        json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        int chat_id = json_object_get_int(jchat_id);
        
        printf("\n ====CHAT DELETED : %i ==== \n", chat_id);
    }
    else if (strcmp(method, "change_chat") == 0) {
return;
        if (chat1 != NULL) {
            struct json_object *jchat_id;
            struct json_object *jnew_name;
            struct json_object *jnew_desc;
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "name", &jnew_name);
            json_object_object_get_ex(jobj, "description", &jnew_desc);

            //save it to the t_chat
            chat1->c_id = json_object_get_int(jchat_id);
            chat1->c_name = strdup((char *)json_object_get_string(jnew_name));
            chat1->c_description = strdup((char *)json_object_get_string(jnew_desc));
        }
    }
    else if (strcmp(method, "join_chat") == 0) {
        chat1 = (t_chat*)malloc(sizeof(t_chat));
        struct json_object *jc_type;
        
        json_object_object_get_ex(jobj, "c_type", &jc_type);
        int c_type = json_object_get_int(jc_type);

        if (c_type == 1) { //private chat
            struct json_object *jchat_id;
            struct json_object *jc_type;
            struct json_object *juser1_id;
            struct json_object *juser1_name;
            struct json_object *juser1_bio;
            struct json_object *juser1_img;
            struct json_object *juser2_id;
            struct json_object *juser2_name;
            struct json_object *juser2_bio;
            struct json_object *juser2_img;
            struct json_object *jjoin_type;
            struct json_object *jcus_type; // would be 3.(users role, in private chat all have 3)

            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "c_type", &jc_type);

            json_object_object_get_ex(jobj, "user1_id", &juser1_id);
            json_object_object_get_ex(jobj, "user1_name", &juser1_name);
            json_object_object_get_ex(jobj, "user1_bio", &juser1_bio);
            json_object_object_get_ex(jobj, "user1_img", &juser1_img);

            json_object_object_get_ex(jobj, "user2_id", &juser2_id);
            json_object_object_get_ex(jobj, "user2_name", &juser2_name);
            json_object_object_get_ex(jobj, "user2_bio", &juser2_bio);
            json_object_object_get_ex(jobj, "user2_img", &juser2_img);

            json_object_object_get_ex(jobj, "join_type", &jjoin_type);
            json_object_object_get_ex(jobj, "cus_type", &jcus_type);

            int chat_id = json_object_get_int(jchat_id);
            int c_type = json_object_get_int(jc_type);
            
            int user1_id = json_object_get_int(juser1_id);
            char *user1_name = strdup((char *)json_object_get_string(juser1_name));
            char *user1_bio = strdup((char *)json_object_get_string(juser1_bio));
            char *user1_img = strdup((char *)json_object_get_string(juser1_img));

            int user2_id = json_object_get_int(juser2_id);
            char *user2_name = strdup((char *)json_object_get_string(juser2_name));
            char *user2_bio = strdup((char *)json_object_get_string(juser2_bio));
            char *user2_img = strdup((char *)json_object_get_string(juser2_img));
            
            int join_type = json_object_get_int(jjoin_type);
            int cus_type = json_object_get_int(jcus_type);

            chat1->c_id = chat_id;
            chat1->ct_type = c_type;
            chat1->jt_type = join_type;
            chat1->cus_type = cus_type;

            if (user->u_id == user1_id) {
                //set from user2
                chat1->c_id = user2_id;
                chat1->c_name = user2_name;
                chat1->c_description = user2_bio;
                chat1->c_profile_image = user2_img;
            }
            else if (user->u_id == user2_id) {
                //set from user1
                chat1->c_id = user1_id;
                chat1->c_name = user1_name;
                chat1->c_description = user1_bio;
                chat1->c_profile_image = user1_img;
            }
        }

        bool state = true;
        for (int i = 0; i < app->chats_list_page->user->u_chatarr_count; i++) {
            if (chat1->c_id == app->chats_list_page->user->u_chatarr[i]->c_id) {
                state = false;
            }
        }

        if(state) {
            mx_add_chat(chat1);
            app->chats_list_page->user->u_chatarr[app->chats_list_page->user->u_chatarr_count] = chat1;
            app->chats_list_page->user->u_chatarr_count++;
        }

        
    }
    else if (strcmp(method, "message") == 0) {
        message1 = (t_message*)malloc(sizeof(t_message));
        if (message1 != NULL) {
            
            struct json_object* jmessage_text;
            struct json_object* jmessage_type;
            struct json_object* jmessage_id;
            struct json_object* jchat_id;
            struct json_object* juser_id;
            struct json_object* jmessage_form;
            struct json_object* jmessage_date;
            struct json_object* jmessage_is_edited;
            json_object_object_get_ex(jobj, "message_text", &jmessage_text);
            json_object_object_get_ex(jobj, "message_type", &jmessage_type);
            json_object_object_get_ex(jobj, "message_id", &jmessage_id);
            json_object_object_get_ex(jobj, "user_id", &juser_id);
            json_object_object_get_ex(jobj, "chat_id", &jchat_id);
            json_object_object_get_ex(jobj, "message_form", &jmessage_form);
            json_object_object_get_ex(jobj, "message_date", &jmessage_date);
            json_object_object_get_ex(jobj, "is_edited", &jmessage_is_edited);
            
            //may be all message data needs to be saved somewhere
            message1->m_type = json_object_get_int(jmessage_type);
            message1->m_id = json_object_get_int(jmessage_id);
            message1->u_id = json_object_get_int(juser_id);
            message1->c_id = json_object_get_int(jchat_id);
            message1->m_form = json_object_get_int(jmessage_form);
            message1->m_date = json_object_get_int(jmessage_date);
            message1->m_is_edited = json_object_get_int(jmessage_is_edited);
            message1->m_text = strdup((char *)json_object_get_string(jmessage_text));
        }
        if(app->chat_page->chat->c_id == message1->c_id && app->chats_list_page->user->u_id != message1->u_id) {
            app->chat_page->chat->c_messagearr[app->chat_page->chat->c_messagearr_count] = message1;
            app->chat_page->chat->c_messagearr_count++;
            
            mx_add_chat_message( message1, NULL, 2 );
        }
    }
    else if (strcmp(method, "edit_message") == 0) {
return;
        if (message1 != NULL) {
            struct json_object *jchat_id;
            struct json_object *jmessage_id;
            struct json_object *jedited_text = NULL;
            int c_id = json_object_get_int(jchat_id);
            int m_id = json_object_get_int(jmessage_id);
            char *edited_msg = strdup((char *)json_object_get_string(jedited_text));

            //save it to the t_message
            message1->c_id = c_id;
            message1->m_id = m_id;
            message1->m_text = edited_msg;
        }
    }
    else if (strcmp(method, "delete_message") == 0) {
return;
        struct json_object *jchat_id;
        struct json_object *jmessage_id;
        json_object_object_get_ex(jobj, "chat_id", &jchat_id);
        json_object_object_get_ex(jobj, "message_id", &jmessage_id);

        message1->c_id = json_object_get_int(jchat_id);
        message1->m_id = json_object_get_int(jmessage_id);
    }
    
    json_object_put(jobj);
}

// chat
int mx_load_chat(/* chat_t *chat, gpointer data,*/t_chat* chat, int ltype ) {
    if (ltype == 0) {}
    // 
    app->chat_page->chat = chat;
    if ( chat->ct_type == 2 ) { 
        // load image 
        gtk_image_set_from_file( 
            GTK_IMAGE(gtk_builder_get_object(app->builder, "chat_image_4")), 
            "./resources/chat_icon_40.png" );
        // load name
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "chat_name_l_4")), 
            chat->c_name );
        // load status ?
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "chat_status_l_4")), 
            /*mx_itoa(app->chat_page->userarr_count)*/"userarr count" );
    }
    else if ( chat->ct_type == 1 ) { 

        gtk_image_set_from_file( 
            GTK_IMAGE(gtk_builder_get_object(app->builder, "chat_image_4")), 
            "./resources/profile_icon_40.png" );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "chat_name_l_4")), 
            chat->c_name );
        gtk_label_set_text( 
            GTK_LABEL(gtk_builder_get_object(app->builder, "chat_status_l_4")), 
            "online " );
    }

    if (chat->c_messagearr == NULL) { 

    }

    for (int i = 0; i < chat->c_messagearr_count; i++) {
        int type = 0;
        if (chat->c_messagearr[i]->u_id == app->chats_list_page->user->u_id) {
            type = 1;
        }
        else {
            type = 2;
        }

        mx_add_chat_message( chat->c_messagearr[i], NULL, type );

    }
    return 0;
}
int mx_add_chat_message( t_message *message, t_user *user, int type ) {
    if (message == NULL || user == NULL) {}
    // 
    GtkWidget *newitem = gtk_list_box_row_new();
    GtkWidget *main_box = NULL;
    GtkWidget *message_button = NULL;
    GtkWidget *profile_button = NULL;
    GtkWidget *profile_image = NULL;
    GtkWidget *name_label = NULL;
    GtkWidget *date_label = NULL;
    GtkWidget *message_box = NULL;
    GtkWidget *name_date_box = NULL;
    // GtkWidget *message_list_box = NULL;
    GtkWidget *message_text_label = NULL;

    
    if ( type == 1 ) { // right message
        gtk_widget_set_margin_top(newitem, 5);
        gtk_widget_set_margin_bottom(newitem, 5);
        gtk_widget_set_margin_start(newitem, 60);
        gtk_widget_set_margin_end(newitem, 7);
        message_button = gtk_button_new(  );
        message_box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 );
        name_date_box = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 );
        
        message_text_label = gtk_label_new( message->m_text );
        gtk_widget_set_margin_top(message_text_label, 5);
        gtk_widget_set_margin_bottom(message_text_label, 5);
        gtk_widget_set_margin_start(message_text_label, 5);
        gtk_widget_set_margin_end(message_text_label, 5);
        gtk_label_set_xalign( GTK_LABEL(message_text_label), 0 );
        gtk_label_set_yalign( GTK_LABEL(message_text_label), 0 );
        gtk_label_set_selectable( GTK_LABEL(message_text_label), TRUE );
        gtk_label_set_line_wrap_mode( GTK_LABEL(message_text_label), PANGO_WRAP_WORD_CHAR );

        name_label = gtk_label_new( "" );

        //date_label = gtk_label_new( get_formated_date(ctime( (const time_t*)(&(message->m_date)) ) ) );
        date_label = gtk_label_new( format_time(message->m_date));

        // gtk_label_set_line_wrap_mode( GTK_LABEL(name_label), PANGO_WRAP_WORD );
        // gtk_label_set_line_wrap_mode( GTK_LABEL(date_label), PANGO_WRAP_WORD );
        gtk_widget_set_margin_start(date_label, 5);
        gtk_widget_set_margin_end(date_label, 10);
        gtk_widget_set_margin_start(name_label, 10);
        gtk_widget_set_margin_end(name_label, 5);
        // gtk_widget_set_valign(name_label, GTK_ALIGN_END);
        // gtk_widget_set_valign(date_label, GTK_ALIGN_END);
        gtk_widget_set_halign(name_label, GTK_ALIGN_END);
        gtk_widget_set_halign(date_label, GTK_ALIGN_START);

        gtk_box_pack_end( GTK_BOX(name_date_box), name_label, FALSE, TRUE, 0 ); // date, name to box
        gtk_box_pack_start( GTK_BOX(name_date_box), date_label, FALSE, TRUE, 0 );

        gtk_box_pack_end( GTK_BOX(message_box), name_date_box, FALSE, FALSE, 0 ); // date/name box, message to box
        gtk_box_pack_end( GTK_BOX(message_box), message_text_label, TRUE, TRUE, 0 );

        gtk_container_add( GTK_CONTAINER(message_button), message_box );
        // 
        GtkStyleContext *context;
        context = gtk_widget_get_style_context( GTK_WIDGET( name_date_box) );
        gtk_style_context_add_class(context,"ClearButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( date_label) );
        gtk_style_context_add_class(context,"ChatMessageDate");
        context = gtk_widget_get_style_context( GTK_WIDGET( name_label) );
        gtk_style_context_add_class(context,"ChatMessageDate");
        context = gtk_widget_get_style_context( GTK_WIDGET( message_text_label) );
        gtk_style_context_add_class(context,"ChatMessageText");
        context = gtk_widget_get_style_context( GTK_WIDGET( message_button) );
        gtk_style_context_add_class(context,"ChatMessageButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( newitem) );
        gtk_style_context_add_class(context,"ClearButton");
        //
        g_signal_connect( G_OBJECT(message_button), "button-press-event", G_CALLBACK(mx_message_button_4_signal), NULL );
        gtk_container_add( GTK_CONTAINER(newitem), message_button );
        gtk_widget_set_halign( GTK_WIDGET(newitem), GTK_ALIGN_END );
    }
    else if ( type == 2 && app->chat_page->chat->ct_type == 2 ) { // left message 
        gtk_widget_set_margin_top(newitem, 5);
        gtk_widget_set_margin_bottom(newitem, 5);
        gtk_widget_set_margin_start(newitem, 2);
        gtk_widget_set_margin_end(newitem, 20);
        main_box = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 );
        profile_button = gtk_button_new(  );
        message_button = gtk_button_new(  );

        message_box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 );
        name_date_box = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 );
        
        message_text_label = gtk_label_new( message->m_text );
        gtk_widget_set_margin_top(message_text_label, 5);
        gtk_widget_set_margin_bottom(message_text_label, 5);
        gtk_widget_set_margin_start(message_text_label, 5);
        gtk_widget_set_margin_end(message_text_label, 5);
        gtk_label_set_xalign( GTK_LABEL(message_text_label), 0 );
        gtk_label_set_yalign( GTK_LABEL(message_text_label), 0 );
        gtk_label_set_selectable( GTK_LABEL(message_text_label), TRUE );
        gtk_label_set_line_wrap_mode( GTK_LABEL(message_text_label), PANGO_WRAP_WORD_CHAR );

        t_user* user1 = mx_get_user_request(message->u_id);
        name_label = gtk_label_new(user1->u_name);
     //   date_label = gtk_label_new(get_formated_date(ctime((time_t*)(&message->m_date))));
        date_label = gtk_label_new( format_time(message->m_date));
        gtk_widget_set_margin_start(name_label, 5);
        gtk_widget_set_margin_end(name_label, 10);
        gtk_widget_set_margin_start(date_label, 10);
        gtk_widget_set_margin_end(date_label, 5);
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        gtk_widget_set_halign(date_label, GTK_ALIGN_END);

        gtk_box_pack_start( GTK_BOX(name_date_box), name_label, FALSE, FALSE, 0 ); // date, name to box
        gtk_box_pack_end( GTK_BOX(name_date_box), date_label, FALSE, FALSE, 0 );

        gtk_box_pack_end( GTK_BOX(message_box), name_date_box, FALSE, FALSE, 0 ); // date/name box, message to box
        gtk_box_pack_end( GTK_BOX(message_box), message_text_label, TRUE, TRUE, 0 );

        gtk_widget_set_halign(profile_button, GTK_ALIGN_START);
        gtk_widget_set_valign(profile_button, GTK_ALIGN_END);
        gtk_widget_set_margin_end(profile_button, 2);
        profile_image = gtk_image_new_from_file( "./resources/profile_icon_40.png" );

        gtk_container_add( GTK_CONTAINER(profile_button), profile_image );
        gtk_container_add( GTK_CONTAINER(message_button), message_box );

        gtk_box_pack_start( GTK_BOX(main_box), profile_button, FALSE, FALSE, 0 ); // date, name to box
        gtk_box_pack_end( GTK_BOX(main_box), message_button, TRUE, TRUE, 0 );
        //
        GtkStyleContext *context;
        context = gtk_widget_get_style_context( GTK_WIDGET( name_date_box) );
        gtk_style_context_add_class(context,"ClearButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( message_text_label) );
        gtk_style_context_add_class(context,"ChatMessageText");
        context = gtk_widget_get_style_context( GTK_WIDGET( name_label) );
        gtk_style_context_add_class(context,"ChatMessageName");
        context = gtk_widget_get_style_context( GTK_WIDGET( date_label) );
        gtk_style_context_add_class(context,"ChatMessageDate");
        context = gtk_widget_get_style_context( GTK_WIDGET( profile_button) );
        gtk_style_context_add_class(context,"ChatProfileButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( message_button) );
        gtk_style_context_add_class(context,"ChatMessageButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( newitem) );
        gtk_style_context_add_class(context,"ClearButton");
        //
        g_signal_connect( G_OBJECT(message_button), "button-press-event", G_CALLBACK(mx_message_button_4_signal), NULL );
        g_signal_connect( G_OBJECT(profile_button), "clicked", G_CALLBACK(mx_profile_button_4_signal), &(message->u_id) );
        gtk_container_add( GTK_CONTAINER(newitem), main_box );
        gtk_widget_set_halign( GTK_WIDGET(newitem), GTK_ALIGN_START );
    }
    else if ( type == 2 && app->chat_page->chat->ct_type == 1 ) {
        gtk_widget_set_margin_top(newitem, 5);
        gtk_widget_set_margin_bottom(newitem, 5);
        gtk_widget_set_margin_start(newitem, 2);
        gtk_widget_set_margin_end(newitem, 60);
        message_button = gtk_button_new(  );

        message_box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 );
        name_date_box = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 );
        
        message_text_label = gtk_label_new( message->m_text );
        gtk_widget_set_margin_top(message_text_label, 5);
        gtk_widget_set_margin_bottom(message_text_label, 5);
        gtk_widget_set_margin_start(message_text_label, 5);
        gtk_widget_set_margin_end(message_text_label, 5);
        gtk_label_set_xalign( GTK_LABEL(message_text_label), 0 );
        gtk_label_set_yalign( GTK_LABEL(message_text_label), 0 );
        gtk_label_set_selectable( GTK_LABEL(message_text_label), TRUE );
        gtk_label_set_line_wrap_mode( GTK_LABEL(message_text_label), PANGO_WRAP_WORD_CHAR );
        
        t_user* user1 = mx_get_user_request(message->u_id);
        name_label = gtk_label_new(user1->u_name);
        //date_label = gtk_label_new(get_formated_date(ctime((time_t*)(&message->m_date))));
        date_label = gtk_label_new( format_time(message->m_date));
        gtk_widget_set_margin_start(name_label, 5);
        gtk_widget_set_margin_end(name_label, 10);
        gtk_widget_set_margin_start(date_label, 10);
        gtk_widget_set_margin_end(date_label, 5);
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        gtk_widget_set_halign(date_label, GTK_ALIGN_END);

        gtk_box_pack_start( GTK_BOX(name_date_box), name_label, FALSE, FALSE, 0 ); // date, name to box
        gtk_box_pack_end( GTK_BOX(name_date_box), date_label, FALSE, FALSE, 0 );

        gtk_box_pack_end( GTK_BOX(message_box), name_date_box, FALSE, FALSE, 0 ); // date/name box, message to box
        gtk_box_pack_end( GTK_BOX(message_box), message_text_label, TRUE, TRUE, 0 );

        //gtk_widget_set_halign(profile_button, GTK_ALIGN_START);
        //gtk_widget_set_valign(profile_button, GTK_ALIGN_END);
        //profile_image = gtk_image_new_from_file( "./resources/profile_icon_40.png" );

        //gtk_container_add( GTK_CONTAINER(profile_button), profile_image );
        gtk_container_add( GTK_CONTAINER(message_button), message_box );

        //gtk_box_pack_start( GTK_BOX(main_box), profile_button, FALSE, FALSE, 0 ); // date, name to box
        //gtk_box_pack_end( GTK_BOX(main_box), message_button, TRUE, TRUE, 0 );
        //
        GtkStyleContext *context;
        context = gtk_widget_get_style_context( GTK_WIDGET( name_date_box) );
        gtk_style_context_add_class(context,"ClearButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( date_label) );
        gtk_style_context_add_class(context,"ChatMessageDate");
        context = gtk_widget_get_style_context( GTK_WIDGET( name_label) );
        gtk_style_context_add_class(context,"ChatMessageName");
        context = gtk_widget_get_style_context( GTK_WIDGET( message_text_label) );
        gtk_style_context_add_class(context,"ChatMessageText");
        context = gtk_widget_get_style_context( GTK_WIDGET( message_button) );
        gtk_style_context_add_class(context,"ChatMessageButton");
        context = gtk_widget_get_style_context( GTK_WIDGET( newitem) );
        gtk_style_context_add_class(context,"ClearButton");
        //
        g_signal_connect( G_OBJECT(message_button), "button-press-event", G_CALLBACK(mx_message_button_4_signal), NULL );
        //g_signal_connect( G_OBJECT(profile_button), "clicked", G_CALLBACK(mx_profile_button_4_signal), &(message->u_id) );
        gtk_container_add( GTK_CONTAINER(newitem), message_button );
        gtk_widget_set_halign( GTK_WIDGET(newitem), GTK_ALIGN_START );
    }
    gtk_widget_set_hexpand( GTK_WIDGET(newitem), FALSE );
    /*for (; ; ) { // add attacments

        gtk_box_pack_start( GTK_BOX(message_box), , TRUE, TRUE, 0 );
    }*/

    t_message_widget* mes_widget = (t_message_widget*)malloc( sizeof(t_message_widget) );
    mes_widget->list_item = newitem;
    //g_object_ref(chat_widget->list_item);
    mes_widget->type = type;
    mes_widget->message = message;
    mes_widget->main_box = main_box;
    mes_widget->profile_button = profile_button;
    mes_widget->message_button = message_button;
    mes_widget->message_box = message_box;
    mes_widget->name_date_box = name_date_box;
    mes_widget->message_label = message_text_label;
    //g_object_ref(chat_widget->count_label);
    mes_widget->sort = message->m_date;


    if ( app->chat_page->messagearr == NULL ) { // add chat to arr
        app->chat_page->messagearr_size = 51;
        app->chat_page->messagearr_count = 0;
        app->chat_page->messagearr = (t_message_widget **)malloc( sizeof(t_message_widget*) * app->chat_page->messagearr_size );
        app->chat_page->messagearr[app->chat_page->messagearr_count] = mes_widget;
        app->chat_page->messagearr_count++;
    }
    else if ( app->chat_page->messagearr_count < app->chat_page->messagearr_size  ) {

        app->chat_page->messagearr[app->chat_page->messagearr_count] = mes_widget;
        app->chat_page->messagearr_count++;
    }
    else {
        app->chat_page->messagearr_size += 50;
        t_message_widget ** newmessagearr = (t_message_widget **)malloc( sizeof(t_message_widget*) * app->chat_page->messagearr_size );
        for (; newmessagearr == NULL ; ) { // copy all items to new arr
            newmessagearr = NULL;
        }
//if (newmessagearr == NULL) { }
        app->chat_page->messagearr[app->chat_page->messagearr_count] = mes_widget;
        app->chat_page->messagearr_count++;
    }
    gtk_list_box_prepend( 
        GTK_LIST_BOX(gtk_builder_get_object(app->builder, "main_box_4")), 
        newitem);
    gtk_widget_show_all( GTK_WIDGET(gtk_builder_get_object(app->builder, "main_box_4")) );
    return 0;
}
int mx_del_chat_message( GtkListBox *lbox, GtkWidget *item, int mid ) { // only gtk struct 
if ( lbox == NULL || item == NULL || mid == 0) {}
    
    // gtk_container_remove( GTK_CONTAINER(lbox), item );
    gtk_widget_destroy( item );
    return 0;
}
int mx_change_chat_message( GtkListBox *lbox, GtkWidget *item, int mid ) { // only gtk struct
if ( item == NULL || lbox == NULL || mid == 0) {}
    // find
    // change
    int i = 0;

    for ( ; i < app->chat_page->messagearr_count; i++) {
        if ( app->chat_page->messagearr[i]->message->m_id == mid ) {
            // change 
            gtk_label_set_text(
                GTK_LABEL( app->chat_page->messagearr[i]->message_label ), 
                app->chat_page->messagearr[i]->message->m_text);
            // 
        }
    }

    for (int i = 0; i < user->u_chatarr_count; i++) {
        if (user->u_chatarr[i]->c_id == app->chat_page->messagearr[i]->message->c_id) {

            for (int j = 0; j < user->u_chatarr[i]->c_messagearr_count; j++) {
                if (user->u_chatarr[i]->c_messagearr[j]->m_id == app->chat_page->messagearr[i]->message->m_id) {
                    
                    user->u_chatarr[i]->c_messagearr[j]->m_text = app->chat_page->messagearr[i]->message->m_text;
                }
            }
        }
    }

    /*for (int j = 0; j < app->chat_page->chat->c_messagearr_count; j++) {
        if (app->chat_page->chat->c_messagearr[j]->m_id == app->chat_page->messagearr[i]->message->m_id) {
            
            app->chat_page->chat->c_messagearr[j]->m_text = app->chat_page->messagearr[i]->message->m_text;
        }
    }*/
    return 0;
}


// chats list 
int mx_load_chat_list( t_user *user  ) {        
if (user == NULL) {}
    // 
    app->chats_list_page->user = user;
    for (int i = 0; i < user->u_chatarr_count; i++) {
        mx_add_chat( user->u_chatarr[i] );
    }
    return 0;
}
int mx_add_chat( t_chat *chat ) {
if ( chat == NULL) {}
    // create 

    GtkWidget *newitem = gtk_list_box_row_new();

    //GtkWidget *newitem = gtk_list_box_row_new();


    GtkWidget *main_button = gtk_button_new(  );
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);


    GtkWidget *profile_image = NULL;
    if ( chat->ct_type == 2 ) {

        profile_image = gtk_image_new_from_file( "./resources/chat_icon_50.png" );
    }
    else if ( chat->ct_type == 1 ) {

        profile_image = gtk_image_new_from_file( "./resources/profile_icon_50.png" );
    }
        // gtk_image_get_pixbuf();
        // gdk_pixbuf_from
    //GtkWidget *profile_image = gtk_image_new(  );
    GtkWidget *name_count_box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0);


    gtk_box_pack_start( GTK_BOX(main_box), GTK_WIDGET(profile_image), FALSE, TRUE, 0 );
    gtk_box_pack_start( GTK_BOX(main_box), GTK_WIDGET(name_count_box), TRUE, TRUE, 0 );

    gtk_widget_set_margin_bottom( GTK_WIDGET(profile_image), 5);
    gtk_widget_set_margin_top( GTK_WIDGET(profile_image), 5);
    gtk_widget_set_margin_start( GTK_WIDGET(profile_image), 5);
    gtk_widget_set_margin_end( GTK_WIDGET(profile_image), 5);

    GtkWidget *name_label = gtk_label_new( chat->c_name ) ;
    GtkWidget *count_label = gtk_label_new( "" ) ; // non read count
    gtk_widget_set_margin_start( GTK_WIDGET(name_label), 5);
    gtk_widget_set_margin_end( GTK_WIDGET(count_label), 5);

    gtk_widget_set_margin_top( GTK_WIDGET(name_label), 5);
    gtk_widget_set_margin_bottom( GTK_WIDGET(count_label), 5);

    gtk_widget_set_halign( GTK_WIDGET(name_label), GTK_ALIGN_START);
    gtk_widget_set_halign( GTK_WIDGET(count_label), GTK_ALIGN_END);


    //
    GtkStyleContext *context;
    context = gtk_widget_get_style_context( GTK_WIDGET(newitem) );
    gtk_style_context_add_class(context,"ClearButton");
    context = gtk_widget_get_style_context( GTK_WIDGET(main_button) );
    gtk_style_context_add_class(context,"ChatsListItem");
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box) );
    gtk_style_context_add_class(context,"ClearButton");
    context = gtk_widget_get_style_context( GTK_WIDGET(name_label) );
    gtk_style_context_add_class(context,"ChatsListItemLabel");
  //  context = gtk_widget_get_style_context( GTK_WIDGET(count_label) );
 //   gtk_style_context_add_class(context,"ChatsListItemCountLabel");
    //
    gtk_box_pack_start( GTK_BOX(name_count_box), GTK_WIDGET(name_label), TRUE, TRUE, 0 );
    gtk_box_pack_start( GTK_BOX(name_count_box), GTK_WIDGET(count_label), TRUE, TRUE, 0 );
    //gtk_box_set
    g_signal_connect( G_OBJECT(main_button), "button-press-event", G_CALLBACK(mx_chat_button_3_signal), NULL );

//write(1, " list item\n", 12);
    gtk_container_add( GTK_CONTAINER(main_button), GTK_WIDGET(main_box) );
    gtk_container_add( GTK_CONTAINER(newitem), GTK_WIDGET(main_button) );
    //gtk_list_box_row_set_header( GTK_LIST_BOX_ROW(newitem), main_button );
    gtk_widget_set_margin_top( GTK_WIDGET(newitem), 5);
    gtk_widget_set_margin_start( GTK_WIDGET(newitem), 5);
    gtk_widget_set_margin_end( GTK_WIDGET(newitem), 5);
    //GtkWidget *newitem2 = gtk_list_box_row_new();
    //gtk_list_box_insert( (GtkListBox*)(app->chats_list_page->main_box), newitem2, position);
//sleep(1);

    gtk_list_box_row_changed( GTK_LIST_BOX_ROW(newitem) );
    // add to struct
    t_chat_widget* chat_widget = (t_chat_widget*)malloc( sizeof(t_chat_widget) );
    chat_widget->list_item = newitem;
    //g_object_ref(chat_widget->list_item);
    chat_widget->chat = chat;
    chat_widget->main_button = main_button;
    //g_object_ref(chat_widget->main_button);
    chat_widget->main_box = main_box;
    //g_object_ref(chat_widget->main_box);
    chat_widget->profile_image = profile_image;
    //g_object_ref(chat_widget->profile_image);
    chat_widget->name_count_box = name_count_box;
    //g_object_ref(chat_widget->name_count_box);
    chat_widget->name_label = name_label;
    //g_object_ref(chat_widget->name_label);
    chat_widget->count_label = count_label;
    //g_object_ref(chat_widget->count_label);
    chat_widget->sort = 0;
    if ( app->chats_list_page->chatarr == NULL ) { // add chat to arr
        app->chats_list_page->chatarr_size = 21;
        app->chats_list_page->chatarr_count = 0;
        app->chats_list_page->chatarr = (t_chat_widget **)malloc( sizeof(t_chat_widget*) * app->chats_list_page->chatarr_size );
        app->chats_list_page->chatarr[app->chats_list_page->chatarr_count] = chat_widget;
        app->chats_list_page->chatarr_count++;
    }
    else if ( app->chats_list_page->chatarr_count < app->chats_list_page->chatarr_size  ) {

        app->chats_list_page->chatarr[app->chats_list_page->chatarr_count] = chat_widget;
        app->chats_list_page->chatarr_count++;
    }
    else {
        app->chats_list_page->chatarr_size += 20;
        t_chat_widget ** newchatarr = (t_chat_widget **)malloc( sizeof(t_chat_widget*) * app->chats_list_page->chatarr_size );
        for (; false ; ) { // copy all items to new arr
            newchatarr = NULL;
        }

if (newchatarr == NULL) { }

        app->chats_list_page->chatarr[app->chats_list_page->chatarr_count] = chat_widget;
        app->chats_list_page->chatarr_count++;
    }
    gtk_list_box_prepend( 
        GTK_LIST_BOX(gtk_builder_get_object(app->builder, "chat_list_box_3")), 
        GTK_WIDGET(newitem));
    gtk_widget_show_all( GTK_WIDGET(gtk_builder_get_object(app->builder, "chat_list_box_3")) );
    return 0;
}
int mx_del_chat( GtkListBox *lbox, GtkListBoxRow *item, int cid ) { // only gtk struct 
    // find item 
    if (item == NULL) {
        for (int i = 0; i < app->chats_list_page->chatarr_count; i++) {
            if ( cid == app->chats_list_page->chatarr[i]->chat->c_id ) {
                item = GTK_LIST_BOX_ROW(app->chats_list_page->chatarr[i]->list_item);
            }
        }
    }
    // del 
    for (int i = 0; i < app->chats_list_page->chatarr_count; i++) {

        if ( item == GTK_LIST_BOX_ROW(app->chats_list_page->chatarr[i]->list_item) ) {
            
            gtk_widget_destroy( GTK_WIDGET(item) );
            // free ? 
            app->chats_list_page->chatarr[i] = NULL;
            for (int j = i; j < app->chats_list_page->chatarr_count - 1; j++) { 

                app->chats_list_page->chatarr[j] = app->chats_list_page->chatarr[j+1];
            }
            app->chats_list_page->chatarr_count--;
        }
    }
    gtk_list_box_invalidate_filter(lbox);
    return 0;
}
int mx_change_chat( GtkListBox *lbox, GtkListBoxRow *item, int cid ) { // only gtk struct 
    // find item 
    if (item == NULL) {
        for (int i = 0; i < app->chats_list_page->chatarr_count; i++) {
            if ( cid == app->chats_list_page->chatarr[i]->chat->c_id ) {
                item = GTK_LIST_BOX_ROW(app->chats_list_page->chatarr[i]->list_item);
            }
        }
    }
    // change
    for (int i = 0; i < app->chats_list_page->chatarr_count; i++) {

        if ( item == GTK_LIST_BOX_ROW(app->chats_list_page->chatarr[i]->list_item) ) {
            // 

            // hide count 

        }
    }
    gtk_list_box_invalidate_sort(lbox);
    return 0;
}


int mx_show_search_list( t_chat **chat ) {

    for (int i = 0; (chat)[i] != NULL ; i++) {

        mx_add_search( (chat)[i] );
    }
    return 0;
}
int mx_add_search( t_chat *chat ) {
if ( chat == NULL) {}
    GtkWidget *newitem = gtk_list_box_row_new();

    GtkWidget *main_button = gtk_button_new(  );
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *profile_image = NULL;
    if ( (chat)->ct_type == 2 ) {
        profile_image = gtk_image_new_from_file( "./resources/chat_icon_40.png" );
    }
    else if ( (chat)->ct_type == 1 ) {
        profile_image = gtk_image_new_from_file( "./resources/profile_icon_40.png" );
    }
    gtk_widget_set_margin_bottom( GTK_WIDGET(profile_image), 5);
    gtk_widget_set_margin_top( GTK_WIDGET(profile_image), 5);
    gtk_widget_set_margin_start( GTK_WIDGET(profile_image), 5);
    gtk_widget_set_margin_end( GTK_WIDGET(profile_image), 5);

    GtkWidget *name_count_box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start( GTK_BOX(main_box), GTK_WIDGET(profile_image), FALSE, TRUE, 0 );
    gtk_box_pack_start( GTK_BOX(main_box), GTK_WIDGET(name_count_box), TRUE, TRUE, 0 );
    GtkWidget *search_add_button = gtk_button_new(  );
    GtkWidget *search_add_image = gtk_image_new_from_file( "./resources/add_icon.png" );
    gtk_container_add( GTK_CONTAINER(search_add_button), search_add_image );
    gtk_box_pack_start( GTK_BOX(main_box), GTK_WIDGET(search_add_button), FALSE, TRUE, 0 );

    GtkWidget *count_label = NULL; //
    GtkWidget *name_label = gtk_label_new( (chat)->c_name ) ;
    gtk_widget_set_margin_start( GTK_WIDGET(name_label), 5);
    gtk_widget_set_margin_end( GTK_WIDGET(name_label), 5);
    gtk_widget_set_margin_top( GTK_WIDGET(name_label), 5);
    gtk_widget_set_margin_bottom( GTK_WIDGET(name_label), 5);

    gtk_widget_set_halign( GTK_WIDGET(name_label), GTK_ALIGN_START);
    //
    GtkStyleContext *context;
    context = gtk_widget_get_style_context( GTK_WIDGET(newitem) );
    gtk_style_context_add_class(context,"ChatsListBox");
    // context = gtk_widget_get_style_context( GTK_WIDGET(main_button) );
    // gtk_style_context_add_class(context,"ChatsListItem");
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box) );
    gtk_style_context_add_class(context,"ClearButton");
    context = gtk_widget_get_style_context( GTK_WIDGET(name_label) );
    gtk_style_context_add_class(context,"ChatsListItemLabel");
    context = gtk_widget_get_style_context( GTK_WIDGET(search_add_button) );
    gtk_style_context_add_class(context,"CLCButton");
    //
    gtk_box_pack_start( GTK_BOX(name_count_box), GTK_WIDGET(name_label), TRUE, TRUE, 0 );
    //gtk_box_set
    
//write(1, " list item\n", 12);
    // gtk_container_add( GTK_CONTAINER(main_button), GTK_WIDGET(main_box) );
    // gtk_container_add( GTK_CONTAINER(newitem), GTK_WIDGET(main_button) );
    gtk_container_add( GTK_CONTAINER(newitem), GTK_WIDGET(main_box) );
    //gtk_list_box_row_set_header( GTK_LIST_BOX_ROW(newitem), main_button );
    g_signal_connect( G_OBJECT(search_add_button), "button-press-event", G_CALLBACK(mx_add_search_button_3_signal), NULL );

    gtk_widget_set_margin_top( GTK_WIDGET(newitem), 5);
    gtk_widget_set_margin_start( GTK_WIDGET(newitem), 5);
    gtk_widget_set_margin_end( GTK_WIDGET(newitem), 5);
    //GtkWidget *newitem2 = gtk_list_box_row_new();
    //gtk_list_box_insert( (GtkListBox*)(app->chats_list_page->main_box), newitem2, position);
//sleep(1);

    gtk_list_box_row_changed( GTK_LIST_BOX_ROW(newitem) );
    // add to struct
    t_chat_widget* chat_widget = (t_chat_widget*)malloc( sizeof(t_chat_widget) );
    chat_widget->list_item = newitem;
    //g_object_ref(chat_widget->list_item);
    chat_widget->chat = chat;
    chat_widget->main_button = main_button;
    //g_object_ref(chat_widget->main_button);
    chat_widget->main_box = main_box;
    //g_object_ref(chat_widget->main_box);
    chat_widget->profile_image = profile_image;
    //g_object_ref(chat_widget->profile_image);
    chat_widget->name_count_box = name_count_box;
    //g_object_ref(chat_widget->name_count_box);
    chat_widget->name_label = name_label;
    //g_object_ref(chat_widget->name_label);
    chat_widget->count_label = count_label;
    chat_widget->search_add_button = search_add_button;
    chat_widget->search_add_image = search_add_image;
    //g_object_ref(chat_widget->count_label);
    chat_widget->sort = 0;
    if ( app->chats_list_page->searcharr == NULL ) { // add chat to arr
        app->chats_list_page->searcharr_size = 51;
        app->chats_list_page->searcharr_count = 0;
        app->chats_list_page->searcharr = (t_chat_widget **)malloc( sizeof(t_chat_widget*) * app->chats_list_page->searcharr_size );
        app->chats_list_page->searcharr[app->chats_list_page->searcharr_count] = chat_widget;
        app->chats_list_page->searcharr_count++;
    }
    else if ( app->chats_list_page->searcharr_count < app->chats_list_page->searcharr_size  ) {

        app->chats_list_page->searcharr[app->chats_list_page->searcharr_count] = chat_widget;
        app->chats_list_page->searcharr_count++;
    }
    else {
        app->chats_list_page->searcharr_size += 50;
        t_chat_widget ** newsearcharr = (t_chat_widget **)malloc( sizeof(t_chat_widget*) * app->chats_list_page->searcharr_size );
        for (; false ; ) { // copy all items to new arr
            newsearcharr = NULL;
        }
    if (newsearcharr == NULL) { }
        app->chats_list_page->searcharr[app->chats_list_page->searcharr_count] = chat_widget;
        app->chats_list_page->searcharr_count++;
    }
    gtk_list_box_prepend( 
        GTK_LIST_BOX(gtk_builder_get_object(app->builder, "search_list_3")), 
        GTK_WIDGET(newitem));
    gtk_widget_show_all( GTK_WIDGET(gtk_builder_get_object(app->builder, "search_list_3")) );
    return 0;
}



// notifications
gboolean mx_show_notification( char* notificationstr, int seconds ) {
    if (seconds != 0) {
        // timeout? 
        gdk_threads_add_timeout( seconds * 1000, (GSourceFunc)(mx_hide_notification), NULL );
        //gdk_threads_add_timeout_full();
    }
    //  check entrys
    gtk_widget_show( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_notification_box")) ); 
    // request 
    gtk_label_set_label( GTK_LABEL(gtk_builder_get_object(app->builder, "o_notification")), (const gchar*)notificationstr );


    return TRUE;
}
gboolean mx_hide_notification( void ) {
    
    gtk_widget_hide( GTK_WIDGET(gtk_builder_get_object(app->builder, "o_notification_box")) ); 
    //  check entrys


    // request 


    return FALSE;
}


