#ifndef __chat_ui_H
#define __chat_ui_H

extern void chat_ui (const char *username);

extern void receive_msg_from_server (const char *friend_name, const char *msg);
extern void receive_friend_request_from_server (const char *friend_name);
extern void receive_friend_request_response_from_server (const char *friend_name, const int result);

void add_friend (const gchar *friend_name);

#endif