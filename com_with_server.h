#ifndef __com_with_server_H
#define __com_with_server_H

gboolean send_msg_to_server(const gchar *chat_friend_name, const gchar *msg);
gboolean build_chat_with_server(const gchar *chat_friend_name);

#endif