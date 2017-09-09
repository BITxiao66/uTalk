/********************************************************************************
 * Files         : client.c
 * Description   : the client part of network.
 * Author        : xinchengxin
 * Last Modified : 2017.09.06, 23:58:31
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
char n_filename[100];
int n_photo[100];
char n_friendname[100];


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
int load_friends_list(char friendname[][21], int *photo);
int search_friends(const char* text, char friendname[][21], int *);
int add_friend_request(const char* friendname);
int inform_net();
int agree_add_friend(const char* friendname);
int refuse_add_friend(const char* friendname);
void *recving (void *p_null);
int request_chathistory (const char *friendname, char namelist[][21], char msglist[][21]);
int agree_recv_file ();
int refuse_recv_file ();
int send_useravator (int avator_id);
int request_useravator ();
int send_a_file (const char *path, const char *friendname);
void *send_files(void *ip);
void *recv_files(void *ip);


/********************************************************************************
 * Description : send a file to friend
 * Prameter    : filepath, friendname
 * Return      : int : 1 success, 0 failed 
 * Side effect : send massage to server, create a new thread
 * Author      : xinchengxin
 * Date        : 2017.09.06
 ********************************************************************************/
int send_a_file (const char *path, const char *friendname)
{
    char filename[100];
    char send_massage[100];
    char ip[] = "127.0.0.1";//to be changed
    int plen = strlen(path);
    pthread_t thread_id;
    int i;
    for(i = plen - 1; i >= 0; i--)
    {
        if(path[i] == '/')
        {
            break;
        }
    }
    strcpy(filename, path + i + 1);
    strcpy(n_filename, path);

    printf("file: %s filename :%s\n", path, filename);

    pthread_create(&thread_id, NULL, (void *)send_files, (void *)ip);
    
    strcpy(n_friendname, friendname);

    sprintf(send_massage, "正在后台向对方发送文件\"%s\"！！！", filename);
    recv_msg(friendname, send_massage);

    sprintf(send_massage, "%s:sendfile %s %s", friendname, filename, ip);
    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);

    return 1;
}


/********************************************************************************
 * Description : change my avator
 * Prameter    : avator id
 * Return      : int : 1 success, 0 failed 
 * Side effect : send massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int send_useravator (int avator_id)
{
    char send_massage[MAXN];
    sprintf(send_massage, "/7:%d*", avator_id);
    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);
    return 1;
}

/********************************************************************************
 * Description : request my avator from server
 * Prameter    : none.
 * Return      : int : 1 success, 0 failed 
 * Side effect : send massage to server
 * Author      : xinchengxin
 * Date        : 2017.09.06
 ********************************************************************************/
int request_useravator ()
{
    send(socket_fd, "/8*", 4, 0);
    return 1;
}


/********************************************************************************
 * Description : send a file to friend
 * Prameter    : relative filepath, friendname
 * Return      : int : 1 success, 0 failed 
 * Side effect : send massage to server, create a new thread
 * Author      : xinchengxin
 * Date        : 2017.09.06
 ********************************************************************************/
int sendfile(const char* filename, const char* friendname)
{
    char send_message[200];
    puts(filename);
    FILE *fp = fopen(filename, "r");
    puts(filename);
    memset(send_message, ' ', sizeof(send_message));
    send_message[0] = '#';
    int flen = strlen(filename);
    int nlen = strlen(friendname);
    int i;
    for(i = 0; i < flen; i++)
    {
        send_message[i + 1] = filename[i];
    }
    for(i = 0; i < nlen; i++)
    {
        send_message[i + 9] = friendname[i];
    }
    send_message[14] = 0;
    int len;

    while((len = fread(send_message + 15, sizeof(char), 85, fp)) > 0)
    {
        send(socket_fd, send_message, len + 15, 0);
        usleep(1000);
    }
    fclose(fp);
    
    return 0;
}

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
 * Description : agree to recieve file
 * Prameter    : none.
 * Return      : int : 1 success, 0 failed 
 * Side effect : none.
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int agree_recv_file ()
{
    return 1;
}

/********************************************************************************
 * Description : refuse to recieve file
 * Prameter    : none.
 * Return      : int : 1 success, 0 failed 
 * Side effect : none.
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
        got_port = atoi(buf);//port
        printf("%d\n",got_port);
    }

    _2_server_sockaddr.sin_family = AF_INET;
    _2_server_sockaddr.sin_port = htons(got_port);//change to port recieved
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

    if((socket_fd = accept(server_sockfd,(struct sockaddr *)&_2_client_sockaddr,&client_len))==-1)
    {
        perror("accept error");
        return 0;
    }
    close(listen_sockfd); 
    return 1;
}

/********************************************************************************
 * Description : sign in to server
 * Author      : xinchengxin
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
        puts(recv_massage);
        strcpy(currect_name, username);
        puts("login success");
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
int load_friends_list(char friendname[][21], int *photo)
{
    char send_massage[MAXN];
    sprintf(send_massage, "/2:%s*", currect_name);

    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);

    usleep(1000000);

    for(int i = 0; i < n; i++)
    {
        strcpy(friendname[i], friend[i]);
        photo[i] = n_photo[i];
        puts(friendname[i]);
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
 * Description : a thread function of sending file thread
 * Prameter    : void *p_pull (no use)
 * Return      : void *
 * Side effect : a thread function
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
void *send_files(void *ip)
{
    puts(ip);
    int socket_fd, connect_fd;
    struct sockaddr_in server_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        puts("failed to create socket!");
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr (ip);
    server_addr.sin_port = htons(4000);

    printf("%d\n", server_addr.sin_addr.s_addr);

    bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(socket_fd, 10);

    puts("accepting");
    connect_fd = accept(socket_fd, (struct sockaddr*) NULL, NULL);
    
    close(socket_fd);

    int slen, rlen;
    char send_massage[MAXN + 10] = "I have recieved it.";
    char recv_massage[MAXN + 10];

    socket_fd = connect_fd;

    FILE *fp = fopen(n_filename, "r");
    puts("strart send!");
    //每次读一段传输
    while((slen = fread(send_massage, sizeof(char), 4000, fp)) > 0)
    {
        write(socket_fd, send_massage, slen);
        
        //例行回复
        read(socket_fd, recv_massage, MAXN);
    }

    //关闭文件
    fclose(fp);
    
    //通知传输完成
    write(socket_fd, "finash", 7);
    read(socket_fd, recv_massage, MAXN);
    
    close(socket_fd);
    //通知用户传输完成
    printf("finashed!");

    sprintf(send_massage, "向对方发送文件\"%s\"完成！！！", n_filename);
    recv_msg(n_friendname, send_massage);
    
    pthread_exit(NULL);
}

/********************************************************************************
 * Description : a thread function of recieving file thread
 * Prameter    : void *p_pull (no use)
 * Return      : void *
 * Side effect : a thread function
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
void *recv_files(void *ip)
{
    puts(ip);
    if(strcmp(ip, "127.0.0.1") != 0)
    {
        strcpy(ip, "127.0.0.1");
    }
    int socket_fd, slen, rlen;
    char send_massage[MAXN + 10] = "I have recieved it.";
    char recv_massage[MAXN + 10];
    struct sockaddr_in server_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        puts("failed to create socket!");
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    printf("%d\n", server_addr.sin_addr.s_addr);

    if(connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
        puts("failed to connect server!");
        return 0;
    }

    int connect_fd = socket_fd;
    char path[100];
    sprintf(path, "recv_files/%s", n_filename);
    FILE *fp = fopen(path, "w");
    while((rlen = read(connect_fd, recv_massage, MAXN)) > 0)
    {
        //收到结束信号，退出
        if(strcmp(recv_massage, "finash") == 0)
        {
            //告知对方收到
            write(connect_fd, "ok", 3);
            break;
        }
        fwrite(recv_massage, sizeof(char), rlen, fp);

        //例行回复
        write(connect_fd, "ok", 3);
    }
    //关闭文件，接收完成
    fclose(fp);
    sprintf(send_massage, "文件\"%s\"接收完毕，保存在文件安装目录的recv_files文件夹下！！！\n", n_filename);
    recv_msg(n_friendname, send_massage);
    close(connect_fd);
    pthread_exit(NULL);
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
    char judge[100], filename[100];
    char ip[100];
    pthread_t thread_id;
    sscanf(massage, "%s", judge);
    if(strcmp(judge, "send") == 0)
    {
        sscanf(massage, "%s%s", judge, filename);
        sendfile(filename, friendname);
        puts(massage);
        return 1;
    }
    else if (strcmp(judge, "sendfile") == 0)
    {
        sscanf(massage, "%s%s%s", judge, n_filename, ip);

        printf("ip:%s\n", ip);

        //sprintf("")
        pthread_create(&thread_id, NULL, (void *)send_files, (void *)ip);

        puts(massage);
    }
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
 * Description : request friendlist to server
 * Prameter    : friendname[][21] to store friendnames
 * Return      : friendname[][21] storing friendnames
 * Side effect : change the values of friendname[][21]
 * Author      : xinchengxin
 * Date        : 2017.09.05
 ********************************************************************************/
int search_friends(const char* text, char friendname[][21], int *photo)
{
    char send_massage[MAXN];
    sprintf(send_massage, "/5:%s*", text);

    send(socket_fd, send_massage, strlen(send_massage) + 1, 0);

    usleep(1000000);

    for(int i = 0; i < n; i++)
    {
        strcpy(friendname[i], friend[i]);
        photo[i] = n_photo[i];
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
    char filename[100];
    pthread_t thread_id;
    char ip[100];
    char judge[100];
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
                    fscanf(rp, "%s %d", friend[i], n_photo + i);
                }
                fclose(rp);
            }
            //abort add friend request
            else if(recv_massage[1] == '3')
            {
                for(i = 0; i >= 0; i++)
                {
                    if(recv_massage[i] == '*')
                    {
                        recv_massage[i] = 0;
                        break;
                    }
                }
                //agree
                if(recv_massage[3] == '3')
                {
                    sscanf(recv_massage, "/3:3%s", friendname);
                    //friend_request_response(friendname, 1);
                }
                //refuse
                else if(recv_massage[3] == '2')
                {
                    sscanf(recv_massage, "/3:2%s", friendname);
                    //friend_request_response(friendname, 0);
                }
                //request
                else
                {
                    sscanf(recv_massage, "/3:%s", friendname);
                    sprintf(recv_massage, "/3:1%s*", friendname);
                    send(socket_fd, recv_massage, strlen(recv_massage) + 1, 0);
                    //recv_friend_request(friendname);
                }
            }
            else if (recv_massage[1] == '7')
            {
                ;
            }
            else if (recv_massage[1] == '8')
            {
                int photo_id;
                sscanf(recv_massage, "/8:%d*", &photo_id);
                recv_useravator (photo_id);
            }
        }
        //part 2 it is massage
        else if(recv_massage[0] == '#')
        {
            puts(filename);
            sscanf (recv_massage + 1, "%s%s", filename, friendname);
            FILE *fp;
            if((fp = fopen(filename, "r")) == NULL)
            {
                sprintf(massage, "对方向你发送了文件\"%s\",正在后台接收！！！", filename);
                recv_msg(friendname, massage);
            }
            else
            {
                fclose(fp);
            }

            fp = fopen(filename, "a");
            fwrite(recv_massage + 15, sizeof(char), rlen - 15, fp);
            fclose(fp);
            if(rlen < 100)
            {
                sprintf(massage, "文件\"%s\"接收完毕，保存在文件安装目录下！！！\n", filename);
                recv_msg(friendname, massage);
            }
        }
        else
        {
            for(i = 0; i >= 0; i++)
            {
                if(recv_massage[i] == ':')
                {
                    recv_massage[i] = ' ';
                    break;
                }
            }
            sscanf(recv_massage, "%s %s", friendname, massage);

            sscanf(massage, "%s", judge);
            if (strcmp(judge, "sendfile") == 0)
            {
                printf("recv_massage: %s\n", recv_massage);
                sscanf(recv_massage, "%s%s%s%s", n_friendname, judge, n_filename, ip);                

                printf("ip:%s\n", ip);

                pthread_create(&thread_id, NULL, (void *)recv_files, (void *)ip);
        
                puts(massage);
                
                sprintf(massage, "对方向你发送了文件\"%s\",正在后台接收！！！", n_filename);
                recv_msg(friendname, massage);
                
                //通知用户riend_request_response (const char *n
            }
            else
            {
                recv_msg(friendname, massage);
            }
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

