#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include<unistd.h> 
#include <arpa/inet.h> 
#include<fcntl.h>
#include<sys/shm.h>

#define SERVER_PORT 8080 /* 客户机连接远程主机的端口 */ 
#define MAXDATASIZE 100 /* 每次可以接收的最大字节 */ 
#define SERVER_IP "192.168.43.235" /* 服务器的IP地址 */
#define MYKEY   12345
#define SIZE    10240
#define CHAT_PORT 8079
 /********************************************************************************
 * Description : Connect server to login
 * Prameter    : (1)server_addr:指向listen_socket地址信息的指针
                 (2)sockfd:int型指针，指向listen_sockfd
 * Return      : 0:connect server success    -1:else
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan,xuda
 * Date        : 2017.09.01
 * Time        : 15:02:00
 ********************************************************************************/
int online(struct sockaddr_in* server_addr,int* sockfd)
{
    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket"); 
        return -1; 
    } 
    server_addr->sin_family = AF_INET; 
    server_addr->sin_port = htons(SERVER_PORT);
    server_addr->sin_addr.s_addr = inet_addr(SERVER_IP);
    bzero(&(server_addr->sin_zero),sizeof(server_addr->sin_zero)); 
    if (connect(*sockfd, (struct sockaddr *)server_addr,sizeof(struct sockaddr_in)) == -1) 
    {
        perror("connect");
        return -1;
    }
    return 0;
}
 /********************************************************************************
 * Description : Send login information to server
 * Prameter    : (1)user_name:A char*,point to user_name
                 (2)password:A char*,point to password
                 (2)sockfd:assigment using listen_sockfd
 * Return      : A port number:log in success    -1:else
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan,xuda
 * Date        : 2017.09.01
 * Time        : 15:18:00
 ********************************************************************************/
int send_login(char *user_name,char *password,int sockfd)
{
    int len;
    char msg[100];
    char buf[100];
    strcpy(msg,"/0*username:");
    strcat(msg,user_name);
    strcat(msg,";password:");
    strcat(msg,password);
    strcat(msg,";");
    len = strlen(msg);
    int numbytes;
    if(send(sockfd,msg,len,0) == -1)
    {
        perror("send error");
    }
    else
    {
        bzero(buf,MAXDATASIZE);
        if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1)        
        { 
            perror("recv");
            exit(1); 
        }
        else if (numbytes > 0)        
        {
            buf[numbytes] = '\0';
            int port=atoi(buf);//端口号
            return port;
        }
    }
}

 /********************************************************************************
 * Description : Send login information to server
 * Prameter    : (1)user_name:A char*,point to user_name
                 (2)password:A char*,point to password
                 (2)sockfd:assigment using listen_sockfd
 * Return      : 0：sign up success   -1:username had been used
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan,xuda
 * Date        : 2017.09.01
 * Time        : 15:18:00
 ********************************************************************************/
int send_signup(char *user_name,char *password,int sockfd)
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

    if(send(sockfd,msg,len,0) == -1)
    {
        perror("send error");
    }
    else
    {
        bzero(buf,MAXDATASIZE);
        if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1)        
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

int main()
{
    struct sockaddr_in listen_sock_addr;
    int listen_sockfd;
    return 0;
}