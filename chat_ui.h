#ifndef __chat_ui_H
#define __chat_ui_H

extern gboolean chat_ui (const gchar *username);
extern void receive_msg_from_server(gchar *friend_name, gchar * msg);

#endif