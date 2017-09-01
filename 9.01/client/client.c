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
#define ser_port 8079

char buf[MAXDATASIZE];
int numbytes;
int sockfd;
int server_sockfd,client_sockfd;   //第二次socket
socklen_t server_len,client_len;   
struct sockaddr_in server_sockaddr,client_sockaddr;  

int send_login(char *user_name,char *password)
{
    //pthread_t pthpid;
    //pthread_create(&pthpid,NULL,function,NULL);
    int len;
    char msg[100];
    strcpy(msg,"/0*username:");
    strcat(msg,user_name);
    strcat(msg,";password:");
    strcat(msg,password);
    strcat(msg,";");
    len = strlen(msg);


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

int send_signup(char *user_name,char *password)
{
    //pthread_t pthpid;
    //pthread_create(&pthpid,NULL,function,NULL);
    int len;
    char msg[100];
    strcpy(msg,"/1*username:");
    strcat(msg,user_name);
    strcat(msg,";password:");
    strcat(msg,password);
    strcat(msg,";");
    len = strlen(msg);

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


/*void *function(void *p)
{
    while(1)
    {
        bzero(buf,MAXDATASIZE);
        if ((numbytes=recv(client_sockfd, buf, MAXDATASIZE, 0)) == -1)        
        { 
            perror("recv");
            exit(1); 
        }
        else if (numbytes > 0)        
        {
            int bytes_sent;
            buf[numbytes] = '\0';
            return buf;
        }
    }
}*/


int main()
{
    int numbytes; 
    struct sockaddr_in server_addr;
    
    printf("\n======================client initialization======================\n");
    char name[100];    

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket"); 
        exit(1); 
    } 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    bzero(&(server_addr.sin_zero),sizeof(server_addr.sin_zero)); 

    if (connect(sockfd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) == -1) 
    {
        perror("connect");
        exit(1); 
    }
    
    send_signup("xuda","123455");
    //close(sockfd); 
}
