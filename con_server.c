#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <errno.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h>


#define SERVER_PORT 8080 /* 客户机连接远程主机的端口 */ 
#define MAXDATASIZE 100 /* 每次可以接收的最大字节 */ 
#define SERVER_IP "192.168.1.108" /* 服务器的IP地址 */
#define MYKEY   12345
#define SIZE    10240
#define CHAT_PORT 8079


int client_sockfd;


 /********************************************************************************
 * Description : Connect server to login
 * Prameter    : 
 * Return      : 0:connect server success    -1:else
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan,xuda
 * Date        : 2017.09.02
 * Time        : 20:44
 *Vesion       :const IP addrress
 ********************************************************************************/
 int online()
 {
    int server_sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in _2_server_sockaddr;
    struct sockaddr_in _2_client_sockaddr;
    int listen_sockfd;
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    //int server_sockfd;
    listen_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    char buf[100];
    int recvbytes;
    int got_port;
    int len;
    char msg[100];
   
   /* if ((listen_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket"); 
        return -1;
    } */
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&(server_addr.sin_zero),0,sizeof(server_addr.sin_zero)); 
    if (connect(listen_sockfd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) == -1) 
    {
        perror("connect");
        return -1;
    }

    memset(buf,0,MAXDATASIZE);
    if ((recvbytes=recv(listen_sockfd, buf, MAXDATASIZE, 0)) == -1)        
    {
        perror("recv");
        return -1;
    }
    else if (recvbytes > 0)
    {
        buf[recvbytes] = '\0';
        got_port=atoi(buf);//端口号
        printf("%d\n",got_port);
    }

    //second time
    //server_sockfd = socket(AF_INET,SOCK_STREAM, 0); 
    _2_server_sockaddr.sin_family = AF_INET;
    _2_server_sockaddr.sin_port = htons(got_port);//改成传输来的端口号
    _2_server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    int server_len = sizeof(_2_server_sockaddr);

    int on = 1;
    setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    if(bind(server_sockfd,(struct sockaddr *)&_2_server_sockaddr,server_len)==-1)
    {
        perror("bind");
        return -1;
    }   

    if(listen(server_sockfd,5) == -1)
    {
        perror("listen");
        return -1;
    }

    int client_len = sizeof(_2_client_sockaddr);

    //accept
    if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&_2_client_sockaddr,&client_len))==-1)
    {
        perror("accept error");
        return -1;
    }
    //close(listen_sockfd); 
    return 0;
 }

 /********************************************************************************
 * Description : Send login information to server
 * Prameter    : (1)user_name:A char*,point to user_name
                 (2)password:A char*,point to password
 * Return      : 0:log in success    -1:else
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan,xuda
 * Date        : 2017.09.02
 * Time        : 20:44
 ********************************************************************************/
int send_login(const char *user_name,const char *password)
{
    char msg[100];
    char buf[100];
    int len;
    strcpy(msg,"/0*username:");
    strcat(msg,user_name);
    strcat(msg,";password:");
    strcat(msg,password);
    strcat(msg,";");
    len = strlen(msg);
    int numbytes;
    if(send(client_sockfd,msg,len,0) == -1)
    {
        perror("send error");
    }
    else
    {
        memset(buf, 0, MAXDATASIZE);
        if ((numbytes=recv(client_sockfd, buf, MAXDATASIZE, 0)) == -1)        
        { 
            perror("recv");
            exit(1); 
        }
        else if (numbytes > 0)
        {
           if(buf[0]=='0')
                return 0;
            return -1;
        }
    }
}

/********************************************************************************
 * Description : Send signup information to server
 * Prameter    : (1)user_name:A char*,point to user_name
                 (2)password:A char*,point to password
 * Return      : 0：sign up success   -1:username has been used
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan,xuda
 * Date        : 2017.09.02
 * Time        : 20:44
 ********************************************************************************/
 int send_signup(const char *user_name, const char *password)
 {
     int len;
     char msg[100];
     char buf[100];
     strcpy(msg,"/1*username:");
     strcat(msg,user_name);
     strcat(msg,";password:");
     strcat(msg,password);
     strcat(msg,";");
     len = strlen(msg);
     int numbytes;
 
     if(send(client_sockfd,msg,len,0) == -1)
     {
         perror("send error");
     }
     else
     {
         memset(buf,0,MAXDATASIZE);
         if ((numbytes=recv(client_sockfd, buf, MAXDATASIZE, 0)) == -1)        
         {
             perror("recv");
             exit(1); 
         }
         else if (numbytes > 0)        
         {
             buf[numbytes] = '\0';
             int port=atoi(buf);//许达命名的变量，我也不知道为什么这样叫
             return port;
         }
     }
 }
