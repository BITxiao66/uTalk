#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/shm.h>
#include <ctype.h>

#define SERVER_PORT 8080 /* 客户机连接远程主机的端口 */ 
#define MAXDATASIZE 100 /* 每次可以接收的最大字节 */ 
#define SERVER_IP "192.168.43.235" /* 服务器的IP地址 */
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

 int send_signup(char *user_name,char *password)
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

 int send_login(char *user_name,char *password)
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
        memset(buf,0,MAXDATASIZE);
        if ((numbytes=recv(client_sockfd, buf, MAXDATASIZE, 0)) == -1)        
        {
            perror("recv");
        }
        else if (numbytes > 0) 
        {
        if(buf[0]=='0')
                return 0;
        return -1;
        }
     }
 }

int send_msg_to_server(const char* to_who,const char *msg)
{
	char strmsg[100];
	memset(strmsg,0,sizeof(strmsg));

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

/*int recieve_friend_request_from_server(char *name)
{
    return 0;
}*/

int deal_with_request(char *friendname)
{
    char command[100];
    int len;
    if(recieve_friend_request_from_server(friendname)==1)//调用达神的接口函数
    {
        strcpy(command,"/3:0");
        strcat(command,friendname);
        strcat(command,"*");
        int len=strlen(command);
        send(client_sockfd,command,len,0);//接受
        return 1;
    }
    else
    {
        strcpy(command,"/3:1");
        strcat(command,friendname);
        strcat(command,"*");
        int len=strlen(command);
        send(client_sockfd,command,len,0);//拒绝
        return 0;
    }
}

int send_request_to_friend(char *friendname)
{
    char command[100];
    strcpy(command,"/3:");
    strcat(command,friendname);
    strcat(command,"*");
    int len=strlen(command);

    if(send(client_sockfd,command,len,0) == -1)//发送好友请求
    {
            puts("request send error");
            return 0;
    }
    return 0;
}

void* sub_get_msg(void* p)
{
    int recvbytes;
    char buf[100];
    char name[100];
    int t=1;
    while(1)
    {
        recvbytes=recv(client_sockfd, buf, MAXDATASIZE, 0);

        if(buf[0]='/')
        {
            if(buf[1]=='3'&&!isdigit(buf[3]))
            {
                int i=0;
                while(buf[i]!=':') i++;
                i++;
                int j=0;
                while(buf[i]!='*') 
                {
                    name[j]=buf[i];
                    i++;
                    j++;
                }
                name[j]=0;
                int reply=deal_with_request(name);//调用请求答复接口
            }
            else 
            {
                int i=0;
                while(buf[i]!=':') i++;
                i=i+2;
                int j=0;
                while(buf[i]!='*')
                {
                    name[j]=buf[i];
                    i++;
                    j++;
                }
                name[j]=0;
                if(buf[3]=='2') receive_friend_request_response_from_server(name,0);
                if(buf[3]=='3') receive_friend_request_response_from_server(name,0);
            }
        }
        else
            printf("%s\n",buf);//留给达神改//////////////////////////////
    }
    return NULL;
}

int get_msg_from_server()
{
    pthread_t ptid;
    pthread_create(&ptid,NULL,sub_get_msg,NULL);
}

int get_offline_msg_from_server()
{
    char command[100];
    strcpy(command,"/4*");
    int len=strlen(command);
    int a=send(client_sockfd,command,len,0);
}


int main()
{
    online();
    char myname[100];
    char password[100];
    char towho[100];
    char msg;

    int a=send_login("xuda","123456");
    send(client_sockfd,"/3:0,shaw*",strlen("/3:0,shaw*"),0);
    
    
    sleep(1000);
	return 0;
}







