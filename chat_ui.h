#ifndef __CHAT_UI_H
#define __CHAT_UI_H

extern void chat_ui (const char *username);

extern void receive_msg_from_server (const char *friend_name, const char *msg);
extern void receive_friend_request_response_from_server (const char *friend_name, const int result);
extern int receive_friend_request_from_server (const char *friend_name);

void add_friend (const char *friend_name, const char *msg, int position);

#endif