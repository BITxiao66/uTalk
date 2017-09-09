/********************************************************************************
 * Files         : client_test.c
 * Description   : The temporary test client.c file for chat.c
 * Author        : DaDaMr_X <dadamrxx@gmail.com>
 * Last Modified : Sep 7, 2017 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "client.h"
#include "chat.h"

int request_useravator ()
{
    return 1;
}

int send_a_file (const char *filename, const char *cur_chat_friend_name){
    return 1;
}

int init_net (const char *server_ip){
    return 1;
}

int signin (const char *username, const char *password){
    return 1;
}

int signup (const char *username, const char *password){
    return 1;
}

int inform_net (){
    return 1;
}

int load_friends_list (char friendlist[][MAX_LENGTH], int *name){
    strcpy (friendlist[0], "DaDa");
    name[0] = 1;
    strcpy (friendlist[1], "xuda");
    name[1] = 2;
    strcpy (friendlist[2], "fihxc");
    name[2] = 3;
    strcpy (friendlist[3], "Shaw");
    name[3] = 4;
    strcpy (friendlist[4], "Yunxin");
    name[4] = 5;
    strcpy (friendlist[5], "Dashu");
    name[5] = 6;
    strcpy (friendlist[6], "Nurali");
    name[6] = 7;
    strcpy (friendlist[7], "Xueying");
    name[7] = 8;
    strcpy (friendlist[8], "Hanqing");
    name[8] = 9;
    return 9;
}

int send_msg (const char *friendname, const char *msg){
    return 1;
}

int search_friends (const char *name, char friendname[][MAX_LENGTH], int *photo_id){
    sprintf (friendname[0], "Lixia");  
    photo_id[0] = 12;
    sprintf (friendname[1], "Shouzhang");
    photo_id[1] = 17;
    return 2;  
}

int add_friend_request (const char *friendname){
    return 1;
}

int request_chathistory (const char *friendname, char namelist[][MAX_LENGTH], char msglist[][MAX_LENGTH]){
    sprintf (namelist[0], "User1"); sprintf (msglist[0], "Hello");
    sprintf (namelist[1], "User2"); sprintf (msglist[1], "Hi");
    sprintf (namelist[2], "User1"); sprintf (msglist[2], "How are you?");
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

int send_useravator (int avator_id){
    return 1;
}
