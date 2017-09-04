#include <gtk/gtk.h>

int send_msg_to_server (const char *chat_friend_name, const char *msg){
	return TRUE;
}

int request_friends_list_to_server (char *username, int *friends_num, const char *friends_list[], const char *msg_list[]){
	*friends_num = 2;
	friends_list[0] = "xuda";
	msg_list[0] = "last msg";
	friends_list[1] = "shaw";
	msg_list[1] = "last msg";
	return TRUE;
}

int request_friend_msg_to_server (const char *friend_name, int *msg_num, int from[], const char *msg[]){
	*msg_num = 3;
	from[0] = 0; msg[0] = "Hello";
	from[1] = 1; msg[1] = "Hi";
	from[2] = 0; msg[2] = "How are you";
}

int search_friends_to_server (const char *search_string, int *friends_num, const char *friends_name[]){
	*friends_num = 2;
	friends_name[0] = "xuda";
	friends_name[1] = "xuda2";
}
