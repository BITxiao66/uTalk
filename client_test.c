#include <stdio.h>
#include <string.h>
#include "client.h"
#include "chat.h"

int init_net (const char *server_ip){
    return 1;
}

int signin (const char *username, const char *password){
    return 1;
}

int signup (const char *username, const char *password){
    return 1;
}

int load_friends_list (const char *username, char friendlist[][MAX_LENGTH]){
    strcpy (friendlist[0], "xuda");
    strcpy (friendlist[1], "shaw");
    return 2;
}

int send_msg (const char *friendname, const char *msg){
    return 1;
}

int search_friends (const char *text, char friendlist[][MAX_LENGTH]){
    sprintf (friendlist[0], "fihxc");  
    sprintf (friendlist[1], "fihxc2");
    return 2;  
}

int add_friend_request (const char *friendname){
    return 1;
}

int request_chathistory (const char *friendname, char namelist[][MAX_LENGTH], char msglist[][MAX_LENGTH]){
    sprintf (namelist[0], "DaDa"); sprintf (msglist[0], "Hello");
    sprintf (namelist[1], "xuda"); sprintf (msglist[1], "Hi");
    sprintf (namelist[2], "DaDa"); sprintf (msglist[2], "How are you");
    return 3;
}

int agree_add_friend(const char* friendname)
{
    return 1;
}

int refuse_add_friend(const char* friendname)
{
    return 1;
}

int agree_recv_file (){
    return 1;
}

int refuse_recv_file (){
    return 1;
}
