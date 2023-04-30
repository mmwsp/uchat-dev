#include "../inc/ui.h"


// entrys` signals


// buttons signals 
gboolean mx_change_b_2_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL ||data == NULL) {}
    //
    gtk_entry_set_text( 
        GTK_ENTRY( gtk_builder_get_object(app->builder, "login_entry_1") ), "" );
    gtk_entry_set_text( 
        GTK_ENTRY( gtk_builder_get_object(app->builder, "name_entry_1") ), "" );
    gtk_entry_set_text( 
        GTK_ENTRY( gtk_builder_get_object(app->builder, "password_entry_1") ), "" );
    gtk_entry_set_text( 
        GTK_ENTRY( gtk_builder_get_object(app->builder, "repeat_password_entry_1") ), "" );
    // 
    gtk_stack_set_visible_child_full( 
        GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
        "sign_up_page", GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT );
    return true;
}
gboolean mx_enter_b_2_signal( GtkWidget *widget, gpointer data ) {
if (widget == NULL ||data == NULL) {}
    //  check entrys
    const gchar* text1 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "login_entry_2")) );
    const gchar* text2 = gtk_entry_get_text( 
        GTK_ENTRY(gtk_builder_get_object(app->builder, "password_entry_2")) );

    if (text1 != NULL && text2 != NULL
        && strlen(text1) > 0 && strlen(text2) > 0) {

        user = mx_sign_in_up_request((char*)text1, NULL, (char*)text2, NULL);
        app->chats_list_page->user = user;
        if (user != NULL && user->u_id != 0) {

            // request (return t_user or null)
            
            // load page 
            mx_load_chat_list(app->chats_list_page->user);

            // change page 
            gtk_stack_set_visible_child_full( 
                GTK_STACK(gtk_builder_get_object(app->builder, "main_stack")), 
                "chat_list_page", GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT );

        } else {
            mx_show_notification("Error! Wrong data." , 5);
        }
    }
    else {
        mx_show_notification("Error! Wrong data." , 5);
    }

    return true;
}





