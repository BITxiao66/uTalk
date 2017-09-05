#ifndef __CLIENT_H
#define __CLIENT_H

extern int init_net(const char *server_ip);
extern int signin(const char* username, const char* password);
extern int signup(const char* username, const char* password);
extern int send_msg(const char* friendname, const char* massage);
extern int load_friend_list(char friendname[][21]);
extern int search_friends(const char* text, char friendname[][21]);
extern int add_friend(const char* friendname);
extern int inform_net();
extern int agree_add_friend(const char* friendname);
extern int refuse_add_friend(const char* friendname);
extern void *recving (void *p_null);

#endif