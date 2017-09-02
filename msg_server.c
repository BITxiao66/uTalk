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
#include"chat_ui.h"

#define SERVER_PORT 8080 /* 客户机连接远程主机的端口 */ 
#define MAXDATASIZE 100 /* 每次可以接收的最大字节 */ 
#define SERVER_IP "127.0.0.1" /* 服务器的IP地址 */
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
 * Time        : 
 *Vesion       :const IP addrress
 *Note         :只供测试版使用
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

int send_msg_to_server(const char* to_who,const char *msg)
{
	char strmsg[100];
	char buf[100];
	int recvbytes;
	memset(buf,0,100);

	strcpy(strmsg,to_who);
	strcat(strmsg,":");
	strcat(strmsg,msg);

	int len=strlen(strmsg);
	if(send(client_sockfd,strmsg,len,0) == -1)
    {
        	perror("send error");
		    return 0;
    }
    else
    {
		return 1;
	}
}
void* sub_get_msg(void* p)
{
    int recvbytes;
    char buf[100];
    char temp1[100];
    int i;
    while(1)
    {
        recvbytes=recv(client_sockfd, buf, MAXDATASIZE, 0);
        if(recvbytes==0)
        {
            printf("Server disconnected\n");
            return NULL;
        }
        for(i=0;i<100&&buf[i]!=':';i++)
            temp1[i]=buf[i];
        temp1[i]=0;
        receive_msg_from_server(temp1, buf+i+1);
    }
    return NULL;
}
int get_msg_from_server()
{
    pthread_t ptid;
    pthread_create(&ptid,NULL,sub_get_msg,NULL);
}

// int main()
// {
//     online();
//     char name[100];
//     char msg[100];
//     scanf("%s",name);
//     scanf("%s",msg);
// 	int a=send_msg_to_server(name,msg);
// 	return 0;
// }







