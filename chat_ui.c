#include <gtk/gtk.h>
#include <string.h>
#include "com_with_server.h"
#include "error_dialog_ui.h"
#include "chat_ui.h"
// #include "msg_server.h"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *friends_listbox;
gchar username[20];
gchar cur_chat_friend_name[20];

GList *friends_list;
typedef struct{
    GtkWidget *avator_image, *name_label, *msg_label;
} FriendItem;

void update_msg_textview(const gchar *name, const gchar *msg){
    GtkWidget * msg_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_textview"));
    GtkTextBuffer *msg_textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (msg_textview));

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(msg_textbuffer, &end);

    gtk_text_buffer_insert (msg_textbuffer, &end, name, -1);
    gtk_text_buffer_insert (msg_textbuffer, &end, ":\n", -1);
    gtk_text_buffer_insert (msg_textbuffer, &end, msg, -1);
    gtk_text_buffer_insert (msg_textbuffer, &end, "\n\n", -1);
}

void update_friends_list (const gchar *friend_name, const gchar *msg){
    for (GList *l = friends_list; l != NULL; l = l->next){
        GtkWidget *name_label = ((FriendItem *)(l->data))->name_label;
        const gchar *name = gtk_label_get_text ((GtkLabel *)name_label);
        if (strcmp(name, friend_name) == 0){
            GtkWidget *msg_label = ((FriendItem *)(l->data))->msg_label;
            gtk_label_set_text ((GtkLabel *)msg_label, msg);
            break;
        }
    }
}

void receive_msg_from_server (const char *friend_name, const char *msg){
    update_friends_list (friend_name, msg);

    if (strcmp(friend_name, cur_chat_friend_name) == 0){
        update_msg_textview(friend_name, msg);
    }
}

gchar *get_type_textview_msg(){
    GtkWidget * type_textview = GTK_WIDGET (gtk_builder_get_object(builder, "type_textview"));
    GtkTextBuffer * type_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (type_textview));
    
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter (type_textbuffer, &start);
    gtk_text_buffer_get_end_iter (type_textbuffer, &end);

    gchar * msg = gtk_text_buffer_get_text(type_textbuffer, &start, &end, TRUE);
    gtk_text_buffer_set_text (type_textbuffer, "", -1);
    return msg;
}

void send_button_press(GtkWidget *widget, gpointer *data){
    // 1. Get cur_chat_friend_name and the message
    if (strcmp(cur_chat_friend_name, "") == 0){
        return;
    }
    gchar *msg = get_type_textview_msg();
    if (strlen(msg) <= 0){
        return;
    }

    // 2. Send to server
    gboolean send_msg_to_server_succeed = send_msg_to_server(cur_chat_friend_name, msg);
    if (send_msg_to_server_succeed == FALSE){
        error_dialog (window, "Connet faild!");
        return;
    }

    // 3. Set the text buffer in UI
    update_msg_textview(username, msg);
}

void uTalk_friend_selected (GtkWidget *name_label, GtkWidget *widget){
    // Get chatwith label
    GtkWidget *chatwith_label = GTK_WIDGET (gtk_builder_get_object(builder, "chatwith_label"));
    const gchar *name = gtk_label_get_text ((GtkLabel *)name_label);
    
    // If the friend is selected, do nothing
    if (strcmp(name, cur_chat_friend_name) == 0) return;
    
    // Update chatwith label
    strcpy(cur_chat_friend_name, name);
    gtk_label_set_text ((GtkLabel *)chatwith_label, cur_chat_friend_name);

    // Clear msg_textview
    GtkWidget *msg_textview = GTK_WIDGET (gtk_builder_get_object(builder, "msg_textview"));
    GtkTextBuffer *msg_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (msg_textview));
    gtk_text_buffer_set_text (msg_textbuffer, "", -1);

    // Send cur_chat_friend_name to server
    gboolean succeed = build_chat_with_server(cur_chat_friend_name);
    if (succeed == FALSE){
        error_dialog (window, "Connet faild!");
        return;
    }
}

GtkWidget *uTalk_friend_new (gchar *avator_path, gchar *friend_name, gchar *msg, FriendItem *friend_item){
    // Get avator image
    GtkWidget *avator_image = gtk_image_new_from_file (avator_path);
    
    // Create friend name label
    GtkWidget *name_label = gtk_label_new (friend_name);
    PangoAttrList *name_attr_list = pango_attr_list_new ();
    PangoAttribute *name_attr_scale = pango_attr_scale_new (1.2);
    pango_attr_list_insert (name_attr_list, name_attr_scale);
    gtk_label_set_attributes ((GtkLabel *)name_label, name_attr_list);
    pango_attr_list_unref (name_attr_list);
    
    // Create message label
    GtkWidget *msg_label = gtk_label_new (msg);
    
    // Combine name_label and msg_label
    GtkWidget *name_and_msg_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_homogeneous (GTK_BOX(name_and_msg_box), TRUE);
    gtk_box_pack_start (GTK_BOX(name_and_msg_box), name_label, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(name_and_msg_box), msg_label, FALSE, TRUE, 0);
    
    // Combine with avator
    GtkWidget *friend_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX(friend_box), avator_image, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(friend_box), name_and_msg_box, FALSE, TRUE, 0);

    GtkWidget *friend_box_out = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *up_separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *down_separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start (GTK_BOX(friend_box_out), up_separator, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(friend_box_out), friend_box, FALSE, TRUE, 0);
    gtk_box_pack_end (GTK_BOX(friend_box_out), down_separator, FALSE, TRUE, 0);

    // Wrapped with an event box for responsing click
    GtkWidget *friend_eventbox = gtk_event_box_new ();
    gtk_widget_set_events (friend_eventbox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect_swapped (G_OBJECT(friend_eventbox), "button-press-event", G_CALLBACK(uTalk_friend_selected), name_label);
    gtk_container_add (GTK_CONTAINER(friend_eventbox), friend_box_out);

    // Set the addition arguement
    friend_item->avator_image = avator_image;
    friend_item->name_label = name_label;
    friend_item->msg_label = msg_label;

    return friend_eventbox;
}

void add_friend (GtkWidget *widget, gpointer *data){
    FriendItem friend_item;
    GtkWidget *friend = uTalk_friend_new("dada.jpg", "DaDax", "offline msg", &friend_item);
    gtk_list_box_insert ((GtkListBox *)friends_listbox, friend, 0);
    friends_list = g_list_insert ((GList *)friends_list, &friend_item, 0);
    gtk_widget_show_all (friends_listbox);
}

void load_friends_list(){
    gint friends_num;
    gchar *friends_name_list[20], *friends_msg_list[20];
    gboolean succeed = request_friends_list_from_server (username, &friends_num, friends_name_list, friends_msg_list);
    if (succeed == FALSE){
        error_dialog (window, "Connet faild!");
        return;
    }
    GtkWidget *friends_listbox = GTK_WIDGET(gtk_builder_get_object(builder, "friends_listbox"));
    for (int i = 0; i < friends_num; i++){
        FriendItem friend_item;
        GtkWidget *friend = uTalk_friend_new("dada.jpg", friends_name_list[i], friends_msg_list[i], &friend_item);
        gtk_list_box_insert ((GtkListBox *)friends_listbox, friend, -1);
        friends_list = g_list_insert ((GList *)friends_list, &friend_item, -1);
    }
}

void chat_ui_init (){
    gtk_init (NULL, NULL);
    builder = gtk_builder_new_from_file ("chat_ui.glade");

    window = GTK_WIDGET (gtk_builder_get_object(builder, "window"));
    g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    friends_listbox = GTK_WIDGET(gtk_builder_get_object(builder, "friends_listbox"));

    GtkWidget *type_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "type_overlay"));
    GtkWidget *send_button = gtk_button_new_with_label ("发送");
    g_signal_connect (G_OBJECT(send_button), "clicked", G_CALLBACK(send_button_press), NULL);

    gtk_widget_set_halign (send_button, GTK_ALIGN_END);
    gtk_widget_set_valign (send_button, GTK_ALIGN_END);
    gtk_widget_set_margin_end (send_button, 5);
    gtk_widget_set_margin_bottom (send_button, 5);
    gtk_widget_set_size_request (send_button, 90, 43);
    gtk_overlay_add_overlay ((GtkOverlay *)type_overlay, send_button);

    GtkWidget *quit_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "quit_menuitem"));
    g_signal_connect (G_OBJECT(quit_menuitem), "activate", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *v_add_friend_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_add_friend_menuitem"));
    g_signal_connect (G_OBJECT(v_add_friend_menuitem), "activate", G_CALLBACK(add_friend), NULL);
    
    GtkWidget *username_label = GTK_WIDGET(gtk_builder_get_object(builder, "username_label"));
    gtk_label_set_text ((GtkLabel *)username_label, username);
}

void chat_ui (const char *rev_username){
    strcpy (username, rev_username);
    memset (cur_chat_friend_name, 0, sizeof(cur_chat_friend_name));

    chat_ui_init ();

    load_friends_list ();

    gtk_widget_show_all (window);
    gtk_main ();
}
