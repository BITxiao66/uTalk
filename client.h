#ifndef __CLIENT_H
#define __CLIENT_H

#include "chat.h"

extern int init_net (const char *);
extern int signin (const char *, const char *);
extern int signup (const char *, const char *);
extern int inform_net ();

extern int load_friends_list (const char *, char friendlist[][MAX_LENGTH]);
extern int send_msg (const char *, const char *);

extern int search_friends (const char *, char friendname[][MAX_LENGTH]);
extern int add_friend_request (const char *);

extern int agree_add_friend(const char* friendname);
extern int refuse_add_friend(const char* friendname);

extern int request_chathistory (const char *, char namelist[][MAX_LENGTH], char msglist[][MAX_LENGTH]);

extern int agree_recv_file ();
extern int refuse_recv_file ();

#endif
