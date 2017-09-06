/********************************************************************************
 * Files         : client.c
 * Description   : the client part of network.
 * Author        : xinchengxin
 * Last Modified : 2017.09.06, 09:15:31
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "chat.h"


#define SERVER_PORT 8000
#define DEFAULT_PORT 8000
#define MAXN 4000

/********************************************************************************
 * Description : Declaration of all global variables in client.c. (add all global variables appearing in client.c here)
 * Author      : xinchengxin
 * Date        : 2017.09.06
 * Time        : 09:18:12
 ********************************************************************************/
char currect_name[21]; // username of currect login user
int socket_fd, n; // socket id, n is used to transmit data between two threads
char friend[1000][21];// array to transmit data between two thread


 /********************************************************************************
  * Description : Declaration of all functions in client.c. (add all function appearing in client.c here)
  * Author      : xinchengxin
  * Date        : 2017.09.06
  * Time        : 09:15:45
  ********************************************************************************/
int init_net(const char *server_ip);
int signin(const char* username, const char* password);
int signup(const char* username, const char* password);
int send_msg(const char* friendname, const char* massage);
int load_friends_list(char friendname[][21]);
int search_friends(const char* text, char friendname[][21]);
int add_friend_request(const char* friendname);
int inform_net();
int agree_add_friend(const char* friendname);
int refuse_add_friend(const char* friendname);
void *recving (void *p_null);
int request_chathistory (const char *friendname, char namelist[][21], char msglist[][21]);
int agree_recv_file ();
int refuse_recv_file ();


/********************************************************************************
 * Description : request chat history to server
 * Prameter    : friend name, namelist[][21], msglist[][21]
 * Return      : int : 1 success, 0 failed 
 * Side effect : not finashed
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int request_chathistory (const char *friendname, char namelist[][21], char msglist[][21])
{
    sprintf (namelist[0], "DaDa"); sprintf (msglist[0], "Hello");
    sprintf (namelist[1], "xuda"); sprintf (msglist[1], "Hi");
    sprintf (namelist[2], "DaDa"); sprintf (msglist[2], "How are you");
    return 3;
}

/********************************************************************************
 * Description : not finashed
 * Prameter    : not finashed
 * Return      : not finashed
 * Side effect : not finashed
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int agree_recv_file ()
{
    return 1;
}

/********************************************************************************
 * Description : not finashed
 * Prameter    : not finashed
 * Return      : not finashed
 * Side effect : not finashed
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int refuse_recv_file ()
{
    return 1;
}

/********************************************************************************
 * Description : init network
 * Prameter    : server ip
 * Return      : int : 1 success, 0 failed
 * Side effect : send connect massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int init_net(const char *server_ip)
{
    int server_sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in _2_server_sockaddr;
    struct sockaddr_in _2_client_sockaddr;
    int listen_sockfd;
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    listen_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    char buf[MAXN];
    int recvbytes;
    int got_port;
    int len;
    char msg[MAXN];
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    memset(&(server_addr.sin_zero),0,sizeof(server_addr.sin_zero)); 
    if (connect(listen_sockfd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) == -1) 
    {
        perror("connect");
        return 0;
    }

    memset(buf,0,MAXN);
    if ((recvbytes=recv(listen_sockfd, buf, MAXN, 0)) == -1)        
    {
        perror("recv");
        return 0;
    }
    else if (recvbytes > 0)
    {
        buf[recvbytes] = '\0';
        got_port = atoi(buf);//端口号
        printf("%d\n",got_port);
    }

    _2_server_sockaddr.sin_family = AF_INET;
    _2_server_sockaddr.sin_port = htons(got_port);//改成传输来的端口号
    _2_server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    int server_len = sizeof(_2_server_sockaddr);

    int on = 1;
    setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    if(bind(server_sockfd,(struct sockaddr *)&_2_server_sockaddr,server_len)==-1)
    {
        perror("bind");
        return 0;
    }   

    if(listen(server_sockfd,5) == -1)
    {
        perror("listen");
        return 0;
    }

    int client_len = sizeof(_2_client_sockaddr);

    if((socket_fd=accept(server_sockfd,(struct sockaddr *)&_2_client_sockaddr,&client_len))==-1)
    {
        perror("accept error");
        return 0;
    }
    close(listen_sockfd); 
    return 1;
}

/********************************************************************************
 * Description : sign in to server
 * Prameter    : username and password
 * Return      : int : 1 success, 0 failed
 * Side effect : send sign in massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int signin(const char* username, const char* password)
{
    char send_massage[MAXN];
    char recv_massage[MAXN];
    int slen, rlen;
    sprintf(send_massage, "/0*username:%s;password:%s;",username, password);
    slen = strlen(send_massage);
    if(send(socket_fd, send_massage, slen + 1, 0) == -1)
    {
        puts("signin send failed");
        return 0;
    }

    if ((rlen = recv(socket_fd, recv_massage, MAXN, 0)) == -1) 
    { 
        puts("signin back failed");
        return 0;
    }

    if(recv_massage[0] == '0')
    {
        strcpy(currect_name, username);
        return 1;
    }
    return 0;
}

/********************************************************************************
 * Description : sign up to server
 * Prameter    : username and password
 * Return      : int : 1 success, 0 failed
 * Side effect : send sign up massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int signup(const char* username, const char* password)
{
    char send_massage[MAXN];
    char recv_massage[MAXN];
    int slen, rlen;
    sprintf(send_massage, "/1*username:%s;password:%s;",username, password);
    slen = strlen(send_massage);
    if(send(socket_fd, send_massage, slen + 1, 0) == -1)
    {
        puts("signup send failed");
        return 0;
    }

    if ((rlen = recv(socket_fd, recv_massage, MAXN, 0)) == -1) 
    { 
        puts("signup back failed");
        return 0;
    }

    if(recv_massage[0] == '0')
    {
        return 1;
    }
    return 0;
}

/********************************************************************************
 * Description : request friendlist to server
 * Prameter    : friendname[][21] to store friendnames
 * Return      : friendname[][21] storing friendnames
 * Side effect : change the values of friendname[][21]
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int load_friends_list(char friendname[][21])
{
    char send_massage[MAXN];
    sprintf(send_massage, "/2:%s*", currect_name);

    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);

    usleep(1000000);

    for(int i = 0; i < n; i++)
    {
        strcpy(friendname[i], friend[i]);
    }
    return n;
}

/********************************************************************************
 * Description : dispart the socket to two threads, sending and recieving
 * Prameter    : none
 * Return      : int : 1 success, 0 failed
 * Side effect : create a new thread
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int inform_net()
{
    pthread_t pthid;
    pthread_create(&pthid, NULL, (void *)recving, NULL);
    return 1;
}

/********************************************************************************
 * Description : send a massage to a friend
 * Prameter    : friendname, massage
 * Return      : int : 1 success, 0 failed
 * Side effect : send a massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int send_msg(const char* friendname, const char* massage)
{
    char send_massage[MAXN];
    sprintf(send_massage, "%s:%s", friendname, massage);
    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);
    return 1;
}

/********************************************************************************
 * Description : send a friend request to a friend
 * Prameter    : name of friend you want to add 
 * Return      : int : 1 success, 0 failed
 * Side effect : send a friend request massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int add_friend_request(const char* friendname)
{
    char send_massage[MAXN];
    sprintf(send_massage, "/3:%s*", friendname);
    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);
    return 1;
}

/********************************************************************************
 * Description : send a agree response to a friend request from friend
 * Prameter    : name of friend who want to add you
 * Return      : int : 1 success, 0 failed
 * Side effect : send a massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int agree_add_friend(const char* friendname)
{
    char send_massage[MAXN];
    sprintf(send_massage, "/3:1%s*", friendname);
    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);
    return 1;
}

/********************************************************************************
 * Description : send a refuse response to a friend request from friend
 * Prameter    : name of friend who want to add you
 * Return      : int : 1 success, 0 failed
 * Side effect : send a massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int refuse_add_friend(const char* friendname)
{
    char send_massage[MAXN];
    sprintf(send_massage, "/3:0%s*", friendname);
    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);
    return 1;
}

/********************************************************************************
 * Description : 
 * Prameter    : 
 * Return      : 
 * Side effect : 
 * Author      : 
 * Date        : 2017.09.05
 ********************************************************************************/
int search_friends(const char* text, char friendname[][21])
{
    char send_massage[MAXN];
    sprintf(send_massage, "/5:%s*", text);

    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);

    usleep(1000000);

    for(int i = 0; i < n; i++)
    {
        strcpy(friendname[i], friend[i]);
    }
    return n;
}

/********************************************************************************
 * Description : a thread function of recieving thread
 * Prameter    : void *p_pull (no use)
 * Return      : void *
 * Side effect : a thread function
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
void *recving (void *p_null)
{
    int rlen, i;
    char recv_massage[MAXN];
    char friendname[MAXN];
    char massage[MAXN];
    while(1)
    {
        rlen = recv(socket_fd, recv_massage, MAXN, 0);
        puts(recv_massage);

        //part 1, not massage
        if(recv_massage[0] == '/')
        {
            //friend list or searchname list
            if(recv_massage[1] == '2' || recv_massage[1] == '5')
            {
                FILE *fp = fopen("temp", "w");
                fprintf(fp, "%s", recv_massage + 3);
                fclose(fp);
                FILE *rp = fopen("temp", "r");
                fscanf(rp, "%d", &n);
                for(i = 0; i < n; i++)
                {
                    fscanf(rp, "%s", friend[i]);
                }
                fclose(rp);
            }
            //abort add friend request
            else if(recv_massage[1] == '3')
            {
                for(i = 0; i >= 0; i++)
                {
                    if(receive_message[i] == '*')
                    {
                        receive_message[i] = 0;
                        break;
                    }
                }
                //agree
                if(recv_massage[3] == '3')
                {
                    sscanf(recv_massage, "/3:3%s", friendname);
                    friend_request_response(friendname, 1);
                }
                //refuse
                else if(recv_massage[3] == '2')
                {
                    sscanf(recv_massage, "/3:2%s", friendname);
                    friend_request_response(friendname, 0);
                }
                //request
                else 
                {
                    sscanf(recv_massage, "/3:%s", friendname);
                    recv_friend_request(friendname);
                }
            }
        }
        //part 2 it is massage
        else
        {
            for(i = 0; i >= 0; i++)
            {
                if(receive_message[i] == ':')
                {
                    receive_message[i] = ' ';
                    break;
                }
            }
            sscanf(recv_massage, "%s %s", friendname, massage);
            recv_msg(friendname, massage);
        }
    }
}

/********************************************************************************
 * Description : main function to test this mudular
 * Prameter    : prameter of main function
 * Return      : return value of main function
 * Side effect : main function
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
// int main()
// {

//     if(init_net("192.168.43.235"))
//     {
//         puts("yes");
//     }
    
//     if(signin("xuda1","123456"))
//     {
//         puts("yes");
//     }

//     inform_net();

//     char num[MAXN][21];
//     int cnt = load_friends_list (num);

//     int i;
//     for (int i = 0;i < cnt; i++)
//     {
//         printf("%s\n", num[i]);
//     }

//     agree_add_friend ("shaw");

//     while(1);

//     return 0;
// }

