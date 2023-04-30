#include "../inc/ui.h"

static t_sign_page* mx_load_sign_up( /*t_settings* sets,*/ GtkBuilder* builder );
static t_sign_page* mx_load_sign_in( /*t_settings* sets,*/ GtkBuilder* builder );
static t_chats_list_page* mx_load_chats_list( /*t_settings* sets,*/ GtkBuilder* builder );
static t_chat_page* mx_load_chat_page( /*t_settings* sets,*/ GtkBuilder* builder );
static void mx_load_overlays( /*t_settings* sets,*/ GtkBuilder* builder );
gboolean destroy( void /*GtkWidget *widget, gpointer data*/ ) {
    gtk_main_quit();
    return true;
}

t_application* mx_load_ui( int* argc, char*** argv ) {
    t_application* tapp = (t_application*)malloc( sizeof(t_application) );
    gtk_init(argc, argv);

    tapp->builder = gtk_builder_new();
    gtk_builder_add_from_file( tapp->builder, "resources/uchat.glade", NULL);
    g_object_ref(tapp->builder);

    GObject* window = gtk_builder_get_object(tapp->builder, "main_window") ;
    
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL );

    GdkDisplay* disp = gdk_display_get_default();
    GdkScreen* scr = gdk_display_get_default_screen(disp);
    GtkCssProvider* css_provider;
    css_provider = gtk_css_provider_new();
    if (true) {
        gtk_css_provider_load_from_path(css_provider, "./resources/uchat.css", NULL);
    }
    else if (false) {
        gtk_css_provider_load_from_path(css_provider, "./resources/uchat2.css", NULL);
    }

    gtk_style_context_add_provider_for_screen(scr, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    // 1 load sign in page 
    tapp->sign_in_page = mx_load_sign_in(tapp->builder);
    // 2 load sign up page 
    tapp->sign_up_page = mx_load_sign_up(tapp->builder);
    // 3 load chats list page 
    tapp->chats_list_page = mx_load_chats_list(tapp->builder);
    tapp->chat_page = mx_load_chat_page(tapp->builder);

    mx_load_overlays(tapp->builder);

    return tapp;
}

static t_sign_page* mx_load_sign_up(GtkBuilder* builder ) { 
    t_sign_page* tpage = (t_sign_page*)malloc( sizeof(t_sign_page) );

    tpage->type = 1;
    
    GtkWidget* page = GTK_WIDGET(gtk_builder_get_object(builder, "sign_up_page"));

    
    GtkWidget* sign_label = GTK_WIDGET(gtk_builder_get_object(builder, "sign_label_1"));

    
    GtkWidget* change_button = GTK_WIDGET(gtk_builder_get_object(builder, "change_button_1"));
    g_signal_connect( change_button, "clicked", G_CALLBACK(mx_change_b_1_signal), NULL );
    GtkWidget* enter_button = GTK_WIDGET(gtk_builder_get_object(builder, "enter_button_1"));
    g_signal_connect( enter_button, "clicked", G_CALLBACK(mx_enter_b_1_signal), NULL );

    GtkWidget* login_entry = GTK_WIDGET(gtk_builder_get_object(builder, "login_entry_1"));
    GtkWidget* login_label = GTK_WIDGET(gtk_builder_get_object(builder, "login_label_1"));
    GtkWidget* name_entry = GTK_WIDGET(gtk_builder_get_object(builder, "name_entry_1"));
    GtkWidget* name_label = GTK_WIDGET(gtk_builder_get_object(builder, "name_label_1"));
    GtkWidget* password_entry = GTK_WIDGET(gtk_builder_get_object(builder, "password_entry_1"));
    GtkWidget* password_label = GTK_WIDGET(gtk_builder_get_object(builder, "password_label_1"));
    GtkWidget* repeat_password_entry = GTK_WIDGET(gtk_builder_get_object(builder, "repeat_password_entry_1"));
    GtkWidget* repeat_password_label = GTK_WIDGET(gtk_builder_get_object(builder, "repeat_password_label_1"));
  
    GtkStyleContext *context;
    context = gtk_widget_get_style_context( page);
    gtk_style_context_add_class(context,"SignPage");
    context = gtk_widget_get_style_context(sign_label);
    gtk_style_context_add_class(context,"SignMainLabel");

    context = gtk_widget_get_style_context(login_label);
    gtk_style_context_add_class(context,"SignLabel");
    context = gtk_widget_get_style_context( login_entry);
    gtk_style_context_add_class(context,"SignEntry");

    context = gtk_widget_get_style_context(name_label);
    gtk_style_context_add_class(context,"SignLabel");
    context = gtk_widget_get_style_context( name_entry);
    gtk_style_context_add_class(context,"SignEntry");

    context = gtk_widget_get_style_context(password_label);
    gtk_style_context_add_class(context,"SignLabel");
    context = gtk_widget_get_style_context( password_entry);
    gtk_style_context_add_class(context,"SignEntry");

    context = gtk_widget_get_style_context(repeat_password_label);
    gtk_style_context_add_class(context,"SignLabel");
    context = gtk_widget_get_style_context( repeat_password_entry);
    gtk_style_context_add_class(context,"SignEntry");

    context = gtk_widget_get_style_context( change_button);
    gtk_style_context_add_class(context,"SignChange");
    context = gtk_widget_get_style_context( enter_button);
    gtk_style_context_add_class(context,"SignEnter");

    return tpage;
}

static t_sign_page* mx_load_sign_in(GtkBuilder* builder ) {

    t_sign_page* tpage = (t_sign_page*)malloc( sizeof(t_sign_page) );
   

    tpage->type = 2;

    GtkWidget* page = GTK_WIDGET(gtk_builder_get_object(builder, "sign_in_page"));
    GtkWidget* sign_label = GTK_WIDGET(gtk_builder_get_object(builder, "sign_label_2"));

    GtkWidget* change_button = GTK_WIDGET(gtk_builder_get_object(builder, "change_button_2"));
    g_signal_connect( change_button, "clicked", G_CALLBACK(mx_change_b_2_signal), NULL );
    GtkWidget* enter_button = GTK_WIDGET(gtk_builder_get_object(builder, "enter_button_2"));
    g_signal_connect( enter_button, "clicked", G_CALLBACK(mx_enter_b_2_signal), NULL );

    GtkWidget* login_entry = GTK_WIDGET(gtk_builder_get_object(builder, "login_entry_2"));
    GtkWidget* login_label = GTK_WIDGET(gtk_builder_get_object(builder, "login_label_2"));
    GtkWidget* password_entry = GTK_WIDGET(gtk_builder_get_object(builder, "password_entry_2"));
    GtkWidget* password_label = GTK_WIDGET(gtk_builder_get_object(builder, "password_label_2"));


    GtkStyleContext *context;
    context = gtk_widget_get_style_context( page);
    gtk_style_context_add_class(context,"SignPage");
    context = gtk_widget_get_style_context(sign_label);
    gtk_style_context_add_class(context,"SignMainLabel");

    context = gtk_widget_get_style_context(login_label);
    gtk_style_context_add_class(context,"SignLabel");
    context = gtk_widget_get_style_context( login_entry);
    gtk_style_context_add_class(context,"SignEntry");

    context = gtk_widget_get_style_context(password_label);
    gtk_style_context_add_class(context,"SignLabel");
    context = gtk_widget_get_style_context( password_entry);
    gtk_style_context_add_class(context,"SignEntry");

    context = gtk_widget_get_style_context( change_button);
    gtk_style_context_add_class(context,"SignChange");
    context = gtk_widget_get_style_context( enter_button);
    gtk_style_context_add_class(context,"SignEnter");

    return tpage;
}

static t_chats_list_page* mx_load_chats_list(GtkBuilder* builder ) {

    t_chats_list_page* tpage = (t_chats_list_page*)malloc( sizeof(t_chats_list_page) );

    GObject* page = gtk_builder_get_object(builder, "chat_list_page") ;

    GObject* search_revealer = gtk_builder_get_object(builder, "search_revealer_3") ;
    GObject* search_box = gtk_builder_get_object(builder, "search_box_3") ;
    GObject* close_search_button = gtk_builder_get_object(builder, "close_search_button_3") ;
    GObject* search_entry = gtk_builder_get_object(builder, "search_entry_3") ;
    GObject* search_button = gtk_builder_get_object(builder, "search_button_3") ;


    GObject* bottom_box = gtk_builder_get_object(builder, "bottom_box_3") ;
    GObject* add_chat_button = gtk_builder_get_object(builder, "add_chat_b_3") ;
    GObject* profile_button = gtk_builder_get_object(builder, "profile_b_3") ;
    GObject* find_button = gtk_builder_get_object(builder, "find_b_3") ;
    GObject* search_list = gtk_builder_get_object(builder, "search_list_3") ;


    GObject* main_box = gtk_builder_get_object(builder, "chat_list_box_3") ;
    tpage->chatarr = NULL;

    g_signal_connect(  close_search_button, "clicked", G_CALLBACK( mx_close_search_3_signal ), NULL );
    g_signal_connect(  search_button, "clicked", G_CALLBACK( mx_search_3_signal ), NULL );

    g_signal_connect(  add_chat_button, "clicked", G_CALLBACK( mx_add_chat_3_signal ), NULL );
    g_signal_connect(  profile_button, "clicked", G_CALLBACK( mx_show_profile_3_signal ), NULL );
    g_signal_connect( find_button, "clicked", G_CALLBACK( mx_show_search_3_signal ), NULL );

    
    gtk_list_box_set_sort_func( 
        GTK_LIST_BOX(main_box), 
        ( mx_chat_list_sort_3 ), NULL, NULL);

    gtk_list_box_set_sort_func( 
        GTK_LIST_BOX(search_list), 
        ( mx_search_list_sort_3 ), NULL, NULL);



    gtk_widget_show_all( GTK_WIDGET( main_box) );
    gtk_widget_show_all( GTK_WIDGET( search_list) );

    GtkStyleContext *context;
    context = gtk_widget_get_style_context( GTK_WIDGET( page) );
    gtk_style_context_add_class(context,"ChatsListPage");


    context = gtk_widget_get_style_context( GTK_WIDGET( search_revealer) );
    gtk_style_context_add_class(context,"");

    context = gtk_widget_get_style_context( GTK_WIDGET(search_box) );
    gtk_style_context_add_class(context,"ChatsListBox");

    context = gtk_widget_get_style_context( GTK_WIDGET( close_search_button) );
    gtk_style_context_add_class(context,"CLCButton");

    context = gtk_widget_get_style_context( GTK_WIDGET( search_entry) );
    gtk_style_context_add_class(context,"ChatsListItemCountLabel");

    context = gtk_widget_get_style_context( GTK_WIDGET( search_button) );
    gtk_style_context_add_class(context,"ClearButton");


    context = gtk_widget_get_style_context( GTK_WIDGET(bottom_box) );
    gtk_style_context_add_class(context,"ChatsListBox");

    context = gtk_widget_get_style_context( GTK_WIDGET( add_chat_button) );
    gtk_style_context_add_class(context,"CLCButton");

    context = gtk_widget_get_style_context( GTK_WIDGET( profile_button) );
    gtk_style_context_add_class(context,"CLCButton");

    context = gtk_widget_get_style_context( GTK_WIDGET( find_button) );
    gtk_style_context_add_class(context,"CLCButton");


    context = gtk_widget_get_style_context( GTK_WIDGET( main_box) );
    gtk_style_context_add_class(context,"ChatsListMainBox");


    context = gtk_widget_get_style_context( GTK_WIDGET( search_list) ); 
    gtk_style_context_add_class(context,"ChatsListMainBox");

    return tpage;
}

static t_chat_page* mx_load_chat_page(  GtkBuilder* builder ) {

    t_chat_page* tpage = (t_chat_page*)malloc( sizeof(t_chat_page) );
    tpage->send_type = 1;

    GObject* page = gtk_builder_get_object(builder, "chat_page");

    
    GObject* top_box = gtk_builder_get_object(builder, "top_box_4") ;
    GObject* profile_button = gtk_builder_get_object(builder, "profile_button_4") ;
    GObject* exit_chat_button = gtk_builder_get_object(builder, "exit_chat_button_4") ;
    
    GObject* label_box = gtk_builder_get_object(builder, "label_box_4") ;
    GObject* name_label = gtk_builder_get_object(builder, "chat_name_l_4") ;
    GObject* online_label = gtk_builder_get_object(builder, "chat_status_l_4") ;
    

    GObject* bottom_box = gtk_builder_get_object(builder, "send_box_4") ;
    GObject* attach_button = gtk_builder_get_object(builder, "send_attach_button_4") ;
    
    GObject* main_entry = gtk_builder_get_object(builder, "send_entry_4") ;
    GObject* send_button = gtk_builder_get_object(builder, "send_send_button_4") ;
    GObject* message_label = gtk_builder_get_object(builder, "message_label_4") ;
    

    GObject* main_box = gtk_builder_get_object(builder, "main_box_4") ;
    tpage->messagearr = NULL;


    gtk_list_box_set_sort_func( 
        GTK_LIST_BOX(main_box), 
        ( mx_message_list_sort_4 ), NULL, NULL);


    g_signal_connect( exit_chat_button, "clicked", G_CALLBACK( mx_exit_chat_4_signal ), NULL );
    g_signal_connect( profile_button, "clicked", G_CALLBACK( mx_show_chat_profile_4_signal ), NULL );
    g_signal_connect( attach_button, "clicked", G_CALLBACK( mx_attach_4_signal ), NULL );
    g_signal_connect( send_button, "clicked", G_CALLBACK( mx_send_4_signal ), NULL );

    gtk_list_box_set_sort_func( GTK_LIST_BOX( main_box), ( mx_message_list_sort_4 ), NULL, NULL);

    GtkStyleContext *context;

    context = gtk_widget_get_style_context( GTK_WIDGET( page) );
    gtk_style_context_add_class(context,"ChatPage");

    context = gtk_widget_get_style_context( GTK_WIDGET( main_box) );
    gtk_style_context_add_class(context,"ClearButton");

    context = gtk_widget_get_style_context( GTK_WIDGET(top_box) );
    gtk_style_context_add_class(context,"ChatBox");

    context = gtk_widget_get_style_context( GTK_WIDGET( exit_chat_button) );
    gtk_style_context_add_class(context,"CLCButton");

    context = gtk_widget_get_style_context( GTK_WIDGET( profile_button) );
    gtk_style_context_add_class(context,"ClearButton");

    context = gtk_widget_get_style_context( GTK_WIDGET(label_box) );
    gtk_style_context_add_class(context,"ClearButton"); 

    context = gtk_widget_get_style_context( GTK_WIDGET(name_label) );
    gtk_style_context_add_class(context,"ChatNameLabel");

    context = gtk_widget_get_style_context( GTK_WIDGET(online_label) );
    gtk_style_context_add_class(context,"ChatStatusLabel");

    context = gtk_widget_get_style_context( GTK_WIDGET(message_label) );
    gtk_style_context_add_class(context,"ChatRevealedMessageLabel");

    context = gtk_widget_get_style_context( GTK_WIDGET(bottom_box) );
    gtk_style_context_add_class(context,"ChatBox");

    context = gtk_widget_get_style_context( GTK_WIDGET( attach_button) );
    gtk_style_context_add_class(context,"CLCButton");

    context = gtk_widget_get_style_context( GTK_WIDGET( main_entry) );
    gtk_style_context_add_class(context,"ChatMessageEntry");

    context = gtk_widget_get_style_context( GTK_WIDGET( send_button) );
    gtk_style_context_add_class(context,"CLCButton");

    
    return tpage;
}

static void mx_load_overlays(GtkBuilder* builder ) {

    GObject* main_stack = gtk_builder_get_object(builder, "o_overlay");
    GtkStyleContext *context;
    context = gtk_widget_get_style_context( GTK_WIDGET( main_stack) );
    gtk_style_context_add_class(context,"OverlayStack");

    GObject* main_box_1 = gtk_builder_get_object(builder, "profile_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_1) );
    gtk_style_context_add_class(context,"OverlayMainBox");

    GObject* main_label_1 = gtk_builder_get_object(builder, "o_main_label_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_1) );
    gtk_style_context_add_class(context,"OverlayName");

    GObject* edit_button_1 = gtk_builder_get_object(builder, "o_edit_button_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(edit_button_1) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( edit_button_1, "clicked", G_CALLBACK( mx_overlay_1_edit_signal ), NULL );

    GObject* close_button_1 = gtk_builder_get_object(builder, "o_exit_button_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(close_button_1) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( close_button_1, "clicked", G_CALLBACK( mx_overlay_1_exit_signal ), NULL );

    GObject* name_label_1 = gtk_builder_get_object(builder, "o_profile_name_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(name_label_1) );
    gtk_style_context_add_class(context,"OverlayNameLabel");

    GObject* status_label_1 = gtk_builder_get_object(builder, "o_profile_status_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(status_label_1) );
    gtk_style_context_add_class(context,"OverlayStatusLabel");

    GObject* description_label_1 = gtk_builder_get_object(builder, "o_profile_bio_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(description_label_1) );
    gtk_style_context_add_class(context,"OverlayBioLabel");

    GObject* sign_out_button_1 = gtk_builder_get_object(builder, "o_sign_out_button_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(sign_out_button_1) );
    gtk_style_context_add_class(context,"OverlayButton");
    g_signal_connect( sign_out_button_1, "clicked", G_CALLBACK( mx_overlay_1_sign_out_signal ), NULL );

    GObject* del_acc_button_1 = gtk_builder_get_object(builder, "o_del_acc_button_1") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(del_acc_button_1) );
    gtk_style_context_add_class(context,"OverlayRedButton");
    g_signal_connect( del_acc_button_1, "clicked", G_CALLBACK( mx_overlay_1_del_account_signal ), NULL );
// ========================================================================================================= o2 + 

    GObject* main_box_2 = gtk_builder_get_object(builder, "edit_profile_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_2) );
    gtk_style_context_add_class(context,"OverlayMainBox");

    GObject* main_label_2 = gtk_builder_get_object(builder, "o_main_label_2") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_2) );
    gtk_style_context_add_class(context,"OverlayName");

    GObject* save_button_2 = gtk_builder_get_object(builder, "o_save_button_2") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(save_button_2) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( save_button_2, "clicked", G_CALLBACK( mx_overlay_2_save_changes_signal ), NULL );

    GObject* close_button_2 = gtk_builder_get_object(builder, "o_exit_button_2") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(close_button_2) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( close_button_2, "clicked", G_CALLBACK( mx_overlay_2_exit_signal ), NULL );

    GObject* name_label_2 = gtk_builder_get_object(builder, "o_uname_label_2") ; // name 
    context = gtk_widget_get_style_context( GTK_WIDGET(name_label_2) );
    gtk_style_context_add_class(context,"OverlayEntryLabel");

    GObject* name_entry_2 = gtk_builder_get_object(builder, "o_uname_entry_2") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(name_entry_2) );
    gtk_style_context_add_class(context,"OverlayEntry");

    GObject* bio_label_2 = gtk_builder_get_object(builder, "o_ubio_label_2") ; // bio 
    context = gtk_widget_get_style_context( GTK_WIDGET(bio_label_2) );
    gtk_style_context_add_class(context,"OverlayEntryLabel");

    GObject* bio_entry_2 = gtk_builder_get_object(builder, "o_ubio_entry_2") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(bio_entry_2) );
    gtk_style_context_add_class(context,"OverlayEntry");
// ========================================================================================================= o3 + 

    GObject* main_box_3 = gtk_builder_get_object(builder, "add_chat_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_3) );
    gtk_style_context_add_class(context,"OverlayMainBox");

    GObject* main_label_3 = gtk_builder_get_object(builder, "o_main_label_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_3) );
    gtk_style_context_add_class(context,"OverlayName");

    GObject* add_button_3 = gtk_builder_get_object(builder, "o_add_button_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(add_button_3) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( add_button_3, "clicked", G_CALLBACK( mx_overlay_3_add_chat_signal ), NULL );

    GObject* close_button_3 = gtk_builder_get_object(builder, "o_exit_button_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(close_button_3) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( close_button_3, "clicked", G_CALLBACK( mx_overlay_3_exit_signal ), NULL );

    GObject* cname_label_3 = gtk_builder_get_object(builder, "o_cname_label_3") ; // name 
    context = gtk_widget_get_style_context( GTK_WIDGET(cname_label_3) );
    gtk_style_context_add_class(context,"OverlayEntryLabel");

    GObject* cname_entry_3 = gtk_builder_get_object(builder, "o_cname_entry_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(cname_entry_3) );
    gtk_style_context_add_class(context,"OverlayEntry");

    GObject* cdesk_label_3 = gtk_builder_get_object(builder, "o_cdesc_label_3") ; // desk
    context = gtk_widget_get_style_context( GTK_WIDGET(cdesk_label_3) );
    gtk_style_context_add_class(context,"OverlayEntryLabel");

    GObject* cdesc_entry_3 = gtk_builder_get_object(builder, "o_cdesc_entry_3") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(cdesc_entry_3) );
    gtk_style_context_add_class(context,"OverlayEntry");



// ========================================================================================================= o4 + 

    GObject* main_box_4 = gtk_builder_get_object(builder, "chat_profile_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_4) );
    gtk_style_context_add_class(context,"OverlayMainBox");

    GObject* main_label_4 = gtk_builder_get_object(builder, "o_main_label_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_4) );
    gtk_style_context_add_class(context,"OverlayName");

    GObject* save_button_4 = gtk_builder_get_object(builder, "o_edit_button_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(save_button_4) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( save_button_4, "clicked", G_CALLBACK( mx_overlay_4_edit_signal ), NULL );

    GObject* close_button_4 = gtk_builder_get_object(builder, "o_exit_button_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(close_button_4) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( close_button_4, "clicked", G_CALLBACK( mx_overlay_4_exit_signal ), NULL );

    GObject* chat_name_label_4 = gtk_builder_get_object(builder, "o_chat_name_4") ; // name 
    context = gtk_widget_get_style_context( GTK_WIDGET(chat_name_label_4) );
    gtk_style_context_add_class(context,"OverlayNameLabel");

    GObject* chat_status_label_4 = gtk_builder_get_object(builder, "o_chat_status_4") ; // desk
    context = gtk_widget_get_style_context( GTK_WIDGET(chat_status_label_4) );
    gtk_style_context_add_class(context,"OverlayStatusLabel");

    GObject* chat_desk_4 = gtk_builder_get_object(builder, "o_chat_desk_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(chat_desk_4) );
    gtk_style_context_add_class(context,"OverlayBioLabel");

    GObject* leave_button_4 = gtk_builder_get_object(builder, "o_leave_button_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(leave_button_4) );
    gtk_style_context_add_class(context,"OverlayRedButton");
    g_signal_connect( leave_button_4, "clicked", G_CALLBACK( mx_overlay_4_leave_signal ), NULL );

    GObject* del_button_4 = gtk_builder_get_object(builder, "o_del_button_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(del_button_4) );
    gtk_style_context_add_class(context,"OverlayRedButton");
    g_signal_connect( del_button_4, "clicked", G_CALLBACK( mx_overlay_4_delete_signal ), NULL );

// ========================================================================================================= o5

    GObject* main_box_5 = gtk_builder_get_object(builder, "edit_chat_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_5) );
    gtk_style_context_add_class(context,"OverlayMainBox");

    GObject* main_label_5 = gtk_builder_get_object(builder, "o_main_label_5") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_5) );
    gtk_style_context_add_class(context,"OverlayName");

    GObject* save_button_5 = gtk_builder_get_object(builder, "o_save_button_5") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(save_button_5) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( save_button_5, "clicked", G_CALLBACK( mx_overlay_5_save_changes_signal ), NULL );

    GObject* close_button_5 = gtk_builder_get_object(builder, "o_exit_button_5") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(close_button_5) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( close_button_5, "clicked", G_CALLBACK( mx_overlay_5_exit_signal ), NULL );

    GObject* cname_entry_5 = gtk_builder_get_object(builder, "o_cname_entry_5") ; // name 
    context = gtk_widget_get_style_context( GTK_WIDGET(cname_entry_5) );
    gtk_style_context_add_class(context,"OverlayEntry");

    GObject* cname_label_5 = gtk_builder_get_object(builder, "o_cname_label_5") ; // desk
    context = gtk_widget_get_style_context( GTK_WIDGET(cname_label_5) );
    gtk_style_context_add_class(context,"OverlayEntryLabel");

    GObject* cdesk_entry_5 = gtk_builder_get_object(builder, "o_cdesk_entry_5") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(cdesk_entry_5) );
    gtk_style_context_add_class(context,"OverlayEntry");

    GObject* cdesk_label_5 = gtk_builder_get_object(builder, "o_cdesk_label_5") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(cdesk_label_5) );
    gtk_style_context_add_class(context,"OverlayEntryLabel");
// ========================================================================================================= o6

    GObject* main_box_6 = gtk_builder_get_object(builder, "user_profile_4") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_6) );
    gtk_style_context_add_class(context,"OverlayMainBox");

    GObject* main_label_6 = gtk_builder_get_object(builder, "o_main_label_6") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_6) );
    gtk_style_context_add_class(context,"OverlayName");

    GObject* close_button_6 = gtk_builder_get_object(builder, "o_exit_button_6") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(close_button_6) );
    gtk_style_context_add_class(context,"CLCButton");
    g_signal_connect( close_button_6, "clicked", G_CALLBACK( mx_overlay_6_exit_signal ), NULL );

    GObject* user_name_label_6 = gtk_builder_get_object(builder, "o_user_name_6") ; // name 
    context = gtk_widget_get_style_context( GTK_WIDGET(user_name_label_6) );
    gtk_style_context_add_class(context,"OverlayNameLabel");

    GObject* user_status_label_6 = gtk_builder_get_object(builder, "o_user_status_6") ; // bio 
    context = gtk_widget_get_style_context( GTK_WIDGET(user_status_label_6) );
    gtk_style_context_add_class(context,"OverlayStatusLabel");

    GObject* user_bio_6 = gtk_builder_get_object(builder, "o_user_bio_6") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(user_bio_6) );
    gtk_style_context_add_class(context,"OverlayBioLabel");

    GObject* send_mes_button_6 = gtk_builder_get_object(builder, "o_send_mes_button_6") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(send_mes_button_6) );
    gtk_style_context_add_class(context,"OverlayButton");
    g_signal_connect( send_mes_button_6, "clicked", G_CALLBACK( mx_overlay_6_send_message_signal ), NULL );

    GObject* del_button_6 = gtk_builder_get_object(builder, "o_del_button_6") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(del_button_6) );
    gtk_style_context_add_class(context,"OverlayRedButton");
    g_signal_connect( del_button_6, "clicked", G_CALLBACK( mx_overlay_6_del_signal ), NULL );

// ========================================================================================================= o6

    GObject* main_box_n = gtk_builder_get_object(builder, "o_notification_box") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_box_n) );
    gtk_style_context_add_class(context,"OverlayNotificationBox");

    GObject* main_label_n = gtk_builder_get_object(builder, "o_notification") ;
    context = gtk_widget_get_style_context( GTK_WIDGET(main_label_n) );
    gtk_style_context_add_class(context,"OverlayNotificationBox");
}


