#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "friendbox_struct.h"
#include "msg_queue.h"
#include "client.h"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *addfriend_window;
GtkWidget *friends_listbox;

gchar username[20];
gchar cur_chat_friend_name[20];

GList *friends_list;
typedef struct FriendItem{
	gchar friend_name[MAX_LENGTH];
	GtkWidget *friendbox;
	Msg_queue *msg_queue;
} FriendItem;

FriendItem *utalk_frienditem_new (const gchar *friend_name, GtkWidget *friendbox, Msg_queue *msg_queue){
	FriendItem *frienditem = (FriendItem *)malloc (sizeof(FriendItem));
	strcpy (frienditem->friend_name, friend_name);
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

void add_friend (const char *friend_name, const char *msg, int position){
	GtkWidget *friendbox_with_msg = utalk_friendbox_with_msg_new ("dada.jpg", friend_name, msg);
	gtk_list_box_insert ((GtkListBox *)friends_listbox, friendbox_with_msg, position);

	Msg_queue *msg_queue = Msg_queue_new ();
	Message *message = Message_new (friend_name, msg);
	FriendItem *frienditem = utalk_frienditem_new (friend_name, friendbox_with_msg, msg_queue);
	friends_list = g_list_insert (friends_list, frienditem, position);
}

// Transfer file module

void choose_file (GtkWidget *widget, gpointer data){
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("打开文件", (GtkWindow *)window, GTK_FILE_CHOOSER_ACTION_OPEN,
										  "取消", GTK_RESPONSE_CANCEL, "打开", GTK_RESPONSE_ACCEPT, NULL);
	gint res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT){
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		gchar *filename = gtk_file_chooser_get_filename (chooser);
		g_print ("%s\n", filename);
	}
	gtk_widget_destroy (dialog);
}

// Chat history module

void load_chathistory_ui (GtkWidget *widget, gpointer data){
	if (strcmp (cur_chat_friend_name, "") == 0) return;

	gchar namelist[MAX_NUM][MAX_LENGTH], msglist[MAX_NUM][MAX_LENGTH];
	gint msg_num = request_chathistory (cur_chat_friend_name, namelist, msglist);
	GtkWidget *chat_history_window = GTK_WIDGET (gtk_builder_get_object(builder, "chat_history_window"));

	GtkWidget *history_close_button = GTK_WIDGET(gtk_builder_get_object(builder, "history_close_button"));
	g_signal_connect (G_OBJECT(history_close_button), "clicked", G_CALLBACK(gtk_window_close), NULL);

	GtkWidget *chathistory_label = GTK_WIDGET(gtk_builder_get_object(builder, "chathistory_label"));
	char text[20];
	sprintf (text, "与 %s 的历史消息", cur_chat_friend_name);
	gtk_label_set_text ((GtkLabel *)chathistory_label, text);

	GtkTextView *text_view = (GtkTextView *)gtk_builder_get_object(builder, "chat_history_textview");
	gtk_text_view_set_editable (text_view,	FALSE);
	gtk_text_view_set_cursor_visible (text_view, FALSE);

	GtkTextBuffer *chat_history_textbuffer = gtk_text_view_get_buffer(text_view);
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(chat_history_textbuffer, &end);

	char msg[MAX_LENGTH];
	for (int i = 0; i < msg_num; i++){
		sprintf (msg, "\n%s:\n%s\n", namelist[i], msglist[i]);
		gtk_text_buffer_insert (chat_history_textbuffer, &end, msg, -1);
	}

	gtk_widget_show_all ((GtkWidget *)chat_history_window);
}

// Search and add friend module

void friend_request_response (const char *name, int response){
	const gchar *msg = response ? "同意了您的好友请求" : "拒绝了您的好友请求";
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s %s", name, msg);
	gtk_window_set_title (GTK_WINDOW(dialog), "好友请求");
	gtk_dialog_run (GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	if (response == 0) return;
	add_friend (name, "Hello, let's talk!", 0);
	gtk_widget_show_all (friends_listbox);
}

int recv_friend_request (const char *name){
	gchar *msg = "有用户请求添加您为好友";
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "%s", msg);
	gtk_window_set_title (GTK_WINDOW(dialog), "Friend Request");
	gint result = gtk_dialog_run (GTK_DIALOG(dialog));
	gtk_widget_destroy (dialog);

	if (result == GTK_RESPONSE_NO) return 0;
	add_friend (name, "Hello, let's talk!", 0);
	gtk_widget_show_all (friends_listbox);
	return 1;
}

void add_friend_button_clicked (GtkWidget *widget, gpointer data){
	GtkWidget *found_friends_listbox = GTK_WIDGET(gtk_builder_get_object (builder, "found_friends_listbox"));
	GList *list	= gtk_list_box_get_selected_rows ((GtkListBox *)found_friends_listbox);
	if (list == NULL) return;
	GtkListBoxRow *row = (GtkListBoxRow *)(list->data);
	GtkWidget *friendbox = (GtkWidget *)(gtk_container_get_children ((GtkContainer *)row)->data);
	const char *name = utalk_friendbox_get_name (friendbox);

	gint result = add_friend_request (name);

	gchar msg[30];
	if (result == -1) sprintf (msg, "%s 已经是您的好友了。", name);
	else sprintf (msg, "向 %s 的好友申请已发送。", name);
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(addfriend_window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", msg);
	gtk_window_set_title(GTK_WINDOW(dialog), "好友请求");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void search_button_clicked (GtkWidget *widget, gpointer data){
	GtkWidget *searchentry = GTK_WIDGET(gtk_builder_get_object(builder, "searchentry"));
	const gchar *text = gtk_entry_get_text ((GtkEntry *)searchentry);
	g_print ("Search text = %s\n", text);

	char friendlist[20][MAX_LENGTH];
	gint friends_num = search_friends (text, friendlist);
	
	GtkWidget *found_friends_listbox = GTK_WIDGET(gtk_builder_get_object (builder, "found_friends_listbox"));
	GList *list = gtk_container_get_children ((GtkContainer *)found_friends_listbox);
	for (GList *l = list; l != NULL; l = l->next){
		gtk_container_remove ((GtkContainer *)found_friends_listbox, (GtkWidget *)(l->data));
	}
	for (int i = 0; i < friends_num; i++){
		GtkWidget *friendbox = utalk_friendbox_new ("dada.jpg", friendlist[i]);
		gtk_list_box_insert ((GtkListBox *)found_friends_listbox, friendbox, -1);
	}
	gtk_widget_show_all (found_friends_listbox);
}

void load_addfriend_ui (){
	addfriend_window = GTK_WIDGET (gtk_builder_get_object(builder, "addfriend_window"));

	GtkWidget *search_button = GTK_WIDGET(gtk_builder_get_object(builder, "search_button"));
	g_signal_connect (G_OBJECT(search_button), "clicked", G_CALLBACK(search_button_clicked), NULL);

	GtkWidget *add_friend_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_friend_button"));
	g_signal_connect (G_OBJECT(add_friend_button), "clicked", G_CALLBACK(add_friend_button_clicked), NULL);

	GtkWidget *chathistory_close_button = GTK_WIDGET(gtk_builder_get_object(builder, "chathistory_close_button"));
	g_signal_connect (G_OBJECT(chathistory_close_button), "clicked", G_CALLBACK(gtk_window_close), NULL);

	gtk_widget_show_all (addfriend_window);
}

// Chat module

void update_msg_textview (const gchar *name, const gchar *msg){
	GtkWidget *msg_textview = GTK_WIDGET(gtk_builder_get_object (builder, "msg_textview"));
	GtkTextBuffer *msg_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (msg_textview));

	GtkTextIter end;
	gtk_text_buffer_get_end_iter(msg_textbuffer, &end);
	gchar content[100];
	sprintf (content, "\n%s:\n%s\n", name, msg);
	gtk_text_buffer_insert (msg_textbuffer, &end, content, -1);
}

void friend_selected (GtkListBox *box, GtkListBoxRow *row, gpointer user_data){
	// Get the friend selected
	GtkWidget *friendbox = (GtkWidget *)(gtk_container_get_children ((GtkContainer *)row)->data);
	const gchar *name = utalk_friendbox_with_msg_get_name (friendbox);
	
	// Set chatwith_label and cur_chat_friend_name
	GtkWidget *chatwith_label = GTK_WIDGET (gtk_builder_get_object(builder, "chatwith_label"));
	gtk_label_set_text ((GtkLabel *)chatwith_label, name);
	strcpy (cur_chat_friend_name, name);
	
	// Clear msg_textview
	GtkWidget *msg_textview = GTK_WIDGET (gtk_builder_get_object(builder, "msg_textview"));
	GtkTextBuffer *msg_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (msg_textview));
	gtk_text_buffer_set_text (msg_textbuffer, "", -1);

	// Find the frienditem
	FriendItem *frienditem = NULL;
	for (GList *l = friends_list; l != NULL; l = l->next){
		frienditem = (FriendItem *)(l->data);
		if (strcmp(frienditem->friend_name, name) == 0) break;
	}

	// Clear the Msg_queue
	while (!Msg_queue_is_empty (frienditem->msg_queue)){
		Message msg = Msg_queue_front (frienditem->msg_queue);
		Msg_queue_pop (frienditem->msg_queue);
		update_msg_textview(msg.name, msg.text);
	}
}

void recv_msg (const char *friendname, const char *msg){
	FriendItem *frienditem = NULL;
	for (GList *l = friends_list; l != NULL; l = l->next){
		frienditem = (FriendItem *)(l->data);
		if (strcmp(frienditem->friend_name, friendname) == 0) break;
	}
	if (frienditem == NULL){
		server_error_dialog (window);
		return;
	}

	utalk_friendbox_with_msg_set_msg (frienditem->friendbox, msg);

	if (strcmp(friendname, cur_chat_friend_name) == 0){
		update_msg_textview(friendname, msg);
	}
	else{
		Msg_queue_push (frienditem->msg_queue, Message_new (friendname, msg));
	}
}

gchar *get_type_textview_msg (){
	GtkWidget *type_textview = GTK_WIDGET (gtk_builder_get_object(builder, "type_textview"));
	GtkTextBuffer *type_textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (type_textview));
	
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter (type_textbuffer, &start);
	gtk_text_buffer_get_end_iter (type_textbuffer, &end);

	gchar *msg = gtk_text_buffer_get_text (type_textbuffer, &start, &end, TRUE);
	gtk_text_buffer_set_text (type_textbuffer, "", -1);
	return msg;
}

void send_button_clicked (GtkWidget *widget, gpointer data){
	if (strcmp (cur_chat_friend_name, "") == 0) return;
	gchar *msg = get_type_textview_msg ();
	if (strlen (msg) <= 0) return;

	int succeed = send_msg (cur_chat_friend_name, msg);
	if (succeed == 0){
		server_error_dialog (window);
		return;
	}
	else{
		g_print ("Send message successfully. Message: %s\n", msg);
	}

	update_msg_textview (username, msg);

	FriendItem *frienditem = NULL;
	for (GList *l = friends_list; l != NULL; l = l->next){
		frienditem = (FriendItem *)(l->data);
		if (strcmp(frienditem->friend_name, cur_chat_friend_name) == 0) break;
	}
	utalk_friendbox_with_msg_set_msg (frienditem->friendbox, msg);
}

// Init module

void load_friends_list_from_server (){
	char friends_name_list[MAX_NUM][MAX_LENGTH];
	int friends_num = load_friends_list (username, friends_name_list);
	g_print ("Load friends list done. friends counting %d.\n", friends_num);

	for (int i = 0; i < friends_num; i++){
		add_friend (friends_name_list[i], "", -1);
	}
	gtk_widget_show_all (friends_listbox);
}

void test (GtkWidget *widget, gpointer data){
	int num = 2;
	const char *name[3], *msg[3];
	name[0] = "shaw";
	msg[0] = "hello";
	name[1] = "xuda";
	msg[1] = "hehe";
}

void set_test_menu (){
	// GtkWidget *quit_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "quit_menuitem"));
	// g_signal_connect (G_OBJECT(quit_menuitem), "activate", G_CALLBACK(gtk_main_quit), NULL);

	// GtkWidget *v_rev_msg_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_rev_msg_menuitem"));
	// g_signal_connect (G_OBJECT(v_rev_msg_menuitem), "activate", G_CALLBACK(v_rev_msg), NULL);

	// GtkWidget *v_rev_friend_request_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_rev_friend_request_menuitem"));
	// g_signal_connect (G_OBJECT(v_rev_friend_request_menuitem), "activate", G_CALLBACK(v_rev_friend_request), NULL);

	// GtkWidget *v_friend_request_ac_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_friend_request_ac_menuitem"));
	// g_signal_connect (G_OBJECT(v_friend_request_ac_menuitem), "activate", G_CALLBACK(v_friend_request_ac), NULL);

	// GtkWidget *v_friend_request_ref_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "v_friend_request_ref_menuitem"));
	// g_signal_connect (G_OBJECT(v_friend_request_ref_menuitem), "activate", G_CALLBACK(v_friend_request_ref), NULL);

	// GtkWidget *test_button = GTK_WIDGET (gtk_builder_get_object(builder, "test_button"));
	// g_signal_connect (G_OBJECT(test_button), "activate", G_CALLBACK(test), NULL);
}

void chat_init (){
	friends_listbox = GTK_WIDGET(gtk_builder_get_object(builder, "friends_listbox"));
	g_signal_connect (G_OBJECT(friends_listbox), "row-selected", G_CALLBACK(friend_selected), NULL);

	GtkWidget *username_label = GTK_WIDGET(gtk_builder_get_object(builder, "username_label"));
	gtk_label_set_text ((GtkLabel *)username_label, username);

	// Add Send button
	GtkWidget *type_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "type_overlay"));
	GtkWidget *send_button = gtk_button_new_with_label ("发送");
	g_signal_connect (G_OBJECT(send_button), "clicked", G_CALLBACK(send_button_clicked), NULL);

	gtk_widget_set_halign (send_button, GTK_ALIGN_END);
	gtk_widget_set_valign (send_button, GTK_ALIGN_END);
	gtk_widget_set_margin_end (send_button, 5);
	gtk_widget_set_margin_bottom (send_button, 5);
	gtk_widget_set_size_request (send_button, 90, 43);
	gtk_overlay_add_overlay ((GtkOverlay *)type_overlay, send_button);

	// Search and add friend
	GtkWidget *add_friend_menuitem = GTK_WIDGET (gtk_builder_get_object(builder, "add_friend_menuitem"));
	g_signal_connect (G_OBJECT(add_friend_menuitem), "activate", G_CALLBACK(load_addfriend_ui), NULL);

	// Choose file
	GtkWidget *choose_file_button = GTK_WIDGET(gtk_builder_get_object(builder, "choose_file_button"));
	g_signal_connect (G_OBJECT(choose_file_button), "clicked", G_CALLBACK(choose_file), NULL);

	// Chat history
	GtkWidget *chat_history_button = GTK_WIDGET(gtk_builder_get_object(builder, "chat_history_button"));
	g_signal_connect (G_OBJECT(chat_history_button), "clicked", G_CALLBACK(load_chathistory_ui), NULL);

	set_test_menu ();
}

GtkWidget *load_chat_window (const char *recv_username){
	builder = gtk_builder_new_from_file ("chat.glade");
	if(builder == NULL) return NULL;
	
	window = GTK_WIDGET (gtk_builder_get_object(builder, "window"));
	g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	strcpy (username, recv_username);
	memset (cur_chat_friend_name, 0, sizeof(cur_chat_friend_name));

	chat_init ();
	load_friends_list_from_server ();

	return window;
}
