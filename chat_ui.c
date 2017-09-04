#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "chat_ui.h"
#include "friendbox_struct.h"
#include "msg_queue.h"

// #include "test0904.h"
#include "virtual_msg_server.h"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *add_friend_window;
GtkWidget *friends_listbox;

gchar username[20];
gchar cur_chat_friend_name[20];

GList *friends_list;
typedef struct FriendItem{
	const gchar *friend_name;
	GtkWidget *friendbox;
	Msg_queue *msg_queue;
} FriendItem;

FriendItem *utalk_frienditem_new (const gchar *friend_name, GtkWidget *friendbox, Msg_queue *msg_queue){
	FriendItem *frienditem = (FriendItem *)malloc (sizeof(FriendItem));
	frienditem->friend_name = friend_name;
	frienditem->friendbox = friendbox;
	frienditem->msg_queue = msg_queue;
	return frienditem;
}

void server_error_dialog (GtkWidget *window){
	const gchar *msg = "Connect Failed!";
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", msg);
	gtk_window_set_title(GTK_WINDOW(dialog), "Error");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void update_msg_textview (const gchar *name, const gchar *msg){
	GtkWidget *msg_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_textview"));
	GtkTextBuffer *msg_textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (msg_textview));

	GtkTextIter end;
	gtk_text_buffer_get_end_iter(msg_textbuffer, &end);

	gtk_text_buffer_insert (msg_textbuffer, &end, "\n", -1);
	gtk_text_buffer_insert (msg_textbuffer, &end, name, -1);
	gtk_text_buffer_insert (msg_textbuffer, &end, ":\n", -1);
	gtk_text_buffer_insert (msg_textbuffer, &end, msg, -1);
	gtk_text_buffer_insert (msg_textbuffer, &end, "\n", -1);
}

void refresh_friendbox_with_msg (GtkWidget *friendbox, Msg_queue *msg_queue){
	Message *message = Msg_queue_front (msg_queue);
	Msg_queue_pop (msg_queue);
	const gchar *msg = message->text;
	utalk_friendbox_with_msg_set_msg (friendbox, msg);
}

void receive_msg_from_server (const char *friend_name, const char *msg){
	FriendItem *frienditem = NULL;
	for (GList *l = friends_list; l != NULL; l = l->next){
		frienditem = (FriendItem *)(l->data);
		if (strcmp(frienditem->friend_name, friend_name) == 0) break;
	}
	if (frienditem == NULL){
		server_error_dialog (window);
		return;
	}

	Message *message = Message_new (friend_name, msg);
	Msg_queue_push (frienditem->msg_queue, message);

	refresh_friendbox_with_msg (frienditem->friendbox, frienditem->msg_queue);

	if (strcmp(friend_name, cur_chat_friend_name) == 0){
		update_msg_textview(friend_name, msg);
	}
}

void v_rev_msg (GtkWidget *widget, gpointer data){
	receive_msg_from_server ("xuda", "Hello");
}

int receive_friend_request_from_server (const char *friend_name){
	gchar *msg = "有用户请求添加您为好友";
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
	gtk_window_set_title(GTK_WINDOW(dialog), "Friend Request");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	add_friend (friend_name, "Hello, let's talk!", 0);
	gtk_widget_show_all (friends_listbox);

	return 1;
}

void v_rev_friend_request (GtkWidget *widget, gpointer data){
	receive_friend_request_from_server ("xuda");
}

void add_friend (const char *friend_name, const char *msg, int position){
	GtkWidget *friendbox_with_msg = utalk_friendbox_with_msg_new ("dada.jpg", friend_name, msg);
	gtk_list_box_insert ((GtkListBox *)friends_listbox, friendbox_with_msg, position);
	Msg_queue *msg_queue = Msg_queue_new ();
	Message *message = Message_new (friend_name, msg);
	// Msg_queue_push (msg_queue, message);
	FriendItem *frienditem = utalk_frienditem_new (friend_name, friendbox_with_msg, msg_queue);
	friends_list = g_list_insert (friends_list, frienditem, position);
}

void receive_friend_request_response_from_server (const char *friend_name, const int result){
	const gchar *msg = result ? "同意了您的好友请求" : "拒绝了您的好友请求";
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s %s", friend_name, msg);
	gtk_window_set_title(GTK_WINDOW(dialog), "Friend Request Response");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	if (result){
		add_friend (friend_name, "Hello, let's talk!", 0);
		gtk_widget_show_all (friends_listbox);
	}
}

void remove_item_from_container (GtkWidget *widget, gpointer data){
	gtk_container_remove ((GtkContainer *)data, widget);
}

void search_button_press (GtkWidget *widget, gpointer data){
	g_print ("search_button_press\n");

	gint friends_num;
	const char *friends_name[20];
	gboolean succeed = search_friends_to_server ("xuda", &friends_num, friends_name);
	if (succeed == FALSE){
		server_error_dialog (add_friend_window);
		return;
	}

	// Clear found_friends_listbox
	GtkWidget *found_friends_listbox = GTK_WIDGET(gtk_builder_get_object (builder, "found_friends_listbox"));
	gtk_container_foreach ((GtkContainer *)found_friends_listbox, remove_item_from_container, found_friends_listbox);

	// Add
	for (int i = 0; i < friends_num; i++){
		GtkWidget *friendbox = utalk_friendbox_new ("dada.jpg", friends_name[i]);
		gtk_list_box_insert ((GtkListBox *)found_friends_listbox, friendbox, -1);
	}

	gtk_widget_show_all (found_friends_listbox);
}

void add_friend_button_press (GtkWidget *widget, gpointer data){
	g_print ("add_friend_button_press\n");

	GtkWidget *found_friends_listbox = GTK_WIDGET(gtk_builder_get_object (builder, "found_friends_listbox"));
	GList *list	= gtk_list_box_get_selected_rows ((GtkListBox *)found_friends_listbox);
	if (list == NULL) return;
	GtkListBoxRow *row = (GtkListBoxRow *)(list->data);
	GtkWidget *friendbox = (GtkWidget *)(gtk_container_get_children ((GtkContainer *)row)->data);
	const gchar *name = utalk_friendbox_get_name (friendbox);
	g_print ("%s\n", name);

	gboolean succeed = send_friend_request_to_server (name);
	if (succeed == FALSE){
		server_error_dialog (window);
		return;
	}

	gchar *msg1 = "向";
	gchar *msg2 = "的好友申请已发送。";
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(add_friend_window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s %s %s", msg1, name, msg2);
	gtk_window_set_title(GTK_WINDOW(dialog), "好友请求");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	g_print ("Friend request has been sent.\n");
}

void add_friend_ui (){
	g_print ("add friend!\n");
	add_friend_window = GTK_WIDGET (gtk_builder_get_object(builder, "add_friend_window"));

	GtkWidget *search_button = GTK_WIDGET(gtk_builder_get_object(builder, "search_button"));
	g_signal_connect (G_OBJECT(search_button), "clicked", G_CALLBACK(search_button_press), NULL);

	GtkWidget *add_friend_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_friend_button"));
	g_signal_connect (G_OBJECT(add_friend_button), "clicked", G_CALLBACK(add_friend_button_press), NULL);

	// GtkWidget *found_friends_listbox = GTK_WIDGET(gtk_builder_get_object(builder, "found_friends_listbox"));
	// g_signal_connect (G_OBJECT(found_friends_listbox), "row-activated", G_CALLBACK(found_friend_selected), NULL);

	gtk_widget_show_all (add_friend_window);
}

gchar *get_type_textview_msg (){
	GtkWidget * type_textview = GTK_WIDGET (gtk_builder_get_object(builder, "type_textview"));
	GtkTextBuffer * type_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (type_textview));
	
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter (type_textbuffer, &start);
	gtk_text_buffer_get_end_iter (type_textbuffer, &end);

	gchar * msg = gtk_text_buffer_get_text(type_textbuffer, &start, &end, TRUE);
	gtk_text_buffer_set_text (type_textbuffer, "", -1);
	return msg;
}

void send_button_press (GtkWidget *widget, gpointer data){
	// 1. Get cur_chat_friend_name and the message
	if (strcmp(cur_chat_friend_name, "") == 0) return;
	gchar *msg = get_type_textview_msg();
	if (strlen(msg) <= 0) return;

	// 2. Send to server
	gboolean succeed = send_msg_to_server (cur_chat_friend_name, msg);
	if (succeed == FALSE){
		server_error_dialog (add_friend_window);
		return;
	}

	// 3. Set the text buffer in UI
	update_msg_textview(username, msg);
}

void friend_selected (GtkListBox *box, GtkListBoxRow *row, gpointer user_data){
	g_print ("Friend Selected\n");

	GtkWidget *friendbox = (GtkWidget *)(gtk_container_get_children ((GtkContainer *)row)->data);
	const gchar *name = utalk_friendbox_with_msg_get_name (friendbox);
	
	GtkWidget *chatwith_label = GTK_WIDGET (gtk_builder_get_object(builder, "chatwith_label"));
	gtk_label_set_text ((GtkLabel *)chatwith_label, name);

	strcpy (cur_chat_friend_name, name);
	
	// Clear msg_textview
	GtkWidget *msg_textview = GTK_WIDGET (gtk_builder_get_object(builder, "msg_textview"));
	GtkTextBuffer *msg_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (msg_textview));
	gtk_text_buffer_set_text (msg_textbuffer, "", -1);

	// Send cur_chat_friend_name to server
	int msg_num;
	int from[20];
	const char *msg[20];
	gboolean succeed = request_friend_msg_to_server (cur_chat_friend_name, &msg_num, from, msg);
	if (succeed == FALSE){
		server_error_dialog (window);
		return;
	}

	// Show msg
	for (int i = 0; i < msg_num; i++){
		gtk_text_buffer_set_text (msg_textbuffer, msg[i], -1);
		gtk_text_buffer_set_text (msg_textbuffer, "\n\n", -1);
	}
}

void load_friends_list(){
	gint friends_num;
	const gchar *friends_name_list[20], *friends_msg_list[20];
	gboolean succeed = request_friends_list_to_server (username, &friends_num, friends_name_list, friends_msg_list);
	if (succeed == FALSE){
		server_error_dialog (window);
		return;
	}

	for (int i = 0; i < friends_num; i++){
		add_friend (friends_name_list[i], friends_msg_list[i], -1);
	}
}

void v_friend_request_ac (GtkWidget *widget, gpointer data){
	receive_friend_request_response_from_server ("xuda", 1);
}

void v_friend_request_ref (GtkWidget *widget, gpointer data){
	receive_friend_request_response_from_server ("dada", 0);
}

void set_test_menuitem (){
	GtkWidget *quit_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "quit_menuitem"));
	g_signal_connect (G_OBJECT(quit_menuitem), "activate", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *v_rev_msg_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_rev_msg_menuitem"));
	g_signal_connect (G_OBJECT(v_rev_msg_menuitem), "activate", G_CALLBACK(v_rev_msg), NULL);

	GtkWidget *v_rev_friend_request_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_rev_friend_request_menuitem"));
	g_signal_connect (G_OBJECT(v_rev_friend_request_menuitem), "activate", G_CALLBACK(v_rev_friend_request), NULL);

	GtkWidget *v_friend_request_ac_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_friend_request_ac_menuitem"));
	g_signal_connect (G_OBJECT(v_friend_request_ac_menuitem), "activate", G_CALLBACK(v_friend_request_ac), NULL);

	GtkWidget *v_friend_request_ref_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_friend_request_ref_menuitem"));
	g_signal_connect (G_OBJECT(v_friend_request_ref_menuitem), "activate", G_CALLBACK(v_friend_request_ref), NULL);

	GtkWidget *add_friend_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "add_friend_menuitem"));
	g_signal_connect (G_OBJECT(add_friend_menuitem), "activate", G_CALLBACK(add_friend_ui), NULL);
}

void chat_ui_init (){
	gtk_init (NULL, NULL);
	builder = gtk_builder_new_from_file ("chat_ui.glade");

	window = GTK_WIDGET (gtk_builder_get_object(builder, "window"));
	g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	friends_listbox = GTK_WIDGET(gtk_builder_get_object(builder, "friends_listbox"));
	g_signal_connect (G_OBJECT(friends_listbox), "row-selected", G_CALLBACK(friend_selected), NULL);

	GtkWidget *username_label = GTK_WIDGET(gtk_builder_get_object(builder, "username_label"));
	gtk_label_set_text ((GtkLabel *)username_label, username);

	// Add Send button
	GtkWidget *type_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "type_overlay"));
	GtkWidget *send_button = gtk_button_new_with_label ("发送");
	g_signal_connect (G_OBJECT(send_button), "clicked", G_CALLBACK(send_button_press), NULL);

	gtk_widget_set_halign (send_button, GTK_ALIGN_END);
	gtk_widget_set_valign (send_button, GTK_ALIGN_END);
	gtk_widget_set_margin_end (send_button, 5);
	gtk_widget_set_margin_bottom (send_button, 5);
	gtk_widget_set_size_request (send_button, 90, 43);
	gtk_overlay_add_overlay ((GtkOverlay *)type_overlay, send_button);

	set_test_menuitem ();
}

void chat_ui (const char *rev_username){
	strcpy (username, rev_username);
	memset (cur_chat_friend_name, 0, sizeof(cur_chat_friend_name));

	chat_ui_init ();

	load_friends_list ();

	gtk_widget_show_all (window);
	gtk_main ();
}
