#ifndef __CLIENT_H
#define __CLIENT_H

#include "chat.h"

extern int init_net (const char *server_ip);
extern int signin (const char *username, const char *password);
extern int signup (const char *username, const char *password);

extern int load_friends_list (const char *username, char friendlist[][MAX_LENGTH]);
extern int send_msg (const char *friendname, const char *msg);

extern int search_friends (const char *text, char friendlist[][MAX_LENGTH]);
extern int add_friend_request (const char *friendname);

extern int request_chathistory (const char *friendname, char namelist[][MAX_LENGTH], char msglist[][MAX_LENGTH]);

#endif
