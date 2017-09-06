#ifndef __CHAT_H
#define __CHAT_H

#include <gtk/gtk.h>

#define MAX_LENGTH 21
#define MAX_NUM 30

extern GtkWidget *load_chat_window (const char *recv_username);

extern void recv_useravator (int avator_id);
extern void recv_msg (const char *friendname, const char *msg);
extern int recv_friend_request (const char *name);
extern void friend_request_response (const char *name, int response);

#endif
