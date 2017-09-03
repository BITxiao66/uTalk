#ifndef __chat_ui_H
#define __chat_ui_H

extern gboolean chat_ui (const char *username);
extern void receive_msg_from_server(const char *friend_name, const char *msg);
void add_friend (const gchar *friend_name);

#endif