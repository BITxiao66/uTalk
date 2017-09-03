#ifndef __com_with_server_H
#define __com_with_server_H

extern int send_msg_to_server (const char *chat_friend_name, const char *msg);
extern int request_friend_msg_to_server (const char *friend_name, int *msg_num, int from[], const char *msg[]);
extern int request_friends_list_to_server (char *username, int *friends_num, char *friends_list[], char *msg_list[]);
extern int search_friends_to_server (const char *search_string, int *friends_num, const char *friends_name[]);

#endif