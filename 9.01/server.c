/********************************************************************************
 * Files         : server.c
 * Description   : the server mode of the uTalk.
 * Author        : xiaoziyuan,sunjinda
 * Last Modified : 2017.09.01
 ********************************************************************************/
 #include <sys/types.h>
 #include <mysql/mysql.h>
 #include <sys/socket.h>                         
 #include <stdio.h>
 #include <netinet/in.h>                         
 #include <arpa/inet.h>                         
 #include <unistd.h>
 #include<string.h>
 #include<stdlib.h>
 #include<fcntl.h>
 #include<pthread.h>
 #include<sys/shm.h>
 #include<time.h>
 #include"database.h"

 #define MYPORT    8080                          //server's listen port
 #define PORT      8081                          //the begin of srever's chat port
 #define CLPORT    8079                          //client's chat port
 #define MYKEY   12345
 #define SIZE    10240
 /********************************************************************************
Description : sign up in database
Prameter    : Function is not completed.
Return      : int (1 == success, 0 == failed)
Side effect : 
Author      : sunjinda
Date        : 2017.09.02
Time        : 09:55:00
********************************************************************************/
int check_signup(const char* line)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    //making connection
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    
    //get name and password
    char user_name[20];
    char user_pw[20];
    int i =9,j=0;
    while (line[i]!=';')
    {
	user_name[j]=line[i];
	i++; j++;
    }
    user_name[++j]=0;
    i+=10; j=0;
    while (line[i]!=';')
    {
	user_pw[j]=line[i];
	i++; j++;
    }
    user_pw[++j]=0;

    //check if the username exit
    char comm[1024];
    int ans=0;
    sprintf(comm,"select * from alluser where username = '%s';",user_name);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;                            //remember the output number!!
    } 
    if(ans!=0)                            //if user_name exist!
    {
	//printf("username exist!\n");
	mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    else  //start insertion
    {
        char comm1[1024] ="\0";
        sprintf(comm1,"insert into alluser values ('%s','%s');",user_name,user_pw);
        if (mysql_query(conn,comm1))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
	mysql_free_result(res);
        mysql_close(conn);
	return 1;
    }
}

/********************************************************************************
Description : login in database
Prameter    : Function is not completed.
Return      : int (1 == success, 0 == failed)
Side effect : 
Author      : sunjinda
Date        : 2017.09.02
Time        : 09:56:00
********************************************************************************/
int check_login(const char* line)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
  
    //get name and password
    char user_name[20];
    char user_pw[20];
    int i=9,j=0;
    while (line[i]!=';')
    {
	user_name[j]=line[i];
	i++; j++;
    }
    user_name[j]=0;
    i+=10; j=0;
    while (line[i]!=';')
    {
	user_pw[j]=line[i];
	i++; j++;
    }
    user_pw[j]=0;

    //select data
    char comm[1024]="\0";
    int ans=0;
    sprintf(comm,"select * from alluser where username = '%s' and password = '%s';",
            user_name,user_pw);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
	ans++;
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    if(ans==0)
    {
        mysql_free_result(res);
        mysql_close(conn);
	return 0;
    }
   
    //ending
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

 /********************************************************************************
 * Description : int to string
 * Prameter    : int n:a in number   const char* str:destination
 * Return      : void
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 2017.09.01
 * Time        : 11:03:00
 ********************************************************************************/
void itoa(int n,char*str)
{
	str[0]=(n/1000)+'0';
	n%=1000;
	str[1]=(n/100)+'0';
	n%=100;
	str[2]=(n/10)+'0';
	str[3]=n%10+'0';
	str[4]=0;
}

 /********************************************************************************
 * Description : After user clicked the signin button in signin window, signin.
 * Prameter    : Function is not completed.
 * Return      : void
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 2017.09.01
 * Time        : 15:44:06
 ********************************************************************************/
int main()
{
    int  ary_sockfd[10];                         //arrar for clients' sockfd
    char buf[100];
    int listen_sockfd;                           //server's listen socketfd
    int client_sockfd;                           //client's listen socketfd
    int server_sockfd;                           //chat socketfd
    socklen_t listen_len;
    socklen_t client_len;
    socklen_t server_len;
    struct sockaddr_in listen_sockaddr;
    struct sockaddr_in client_sockaddr;
    struct sockaddr_in server_sockaddr;
    int userNum;                                 //the number of current online user 
    struct in_addr client_ip;                    //the client's address information
    printf("\n======================server initialization======================\n");
    while(1)
    {
        listen_sockfd = socket(AF_INET,SOCK_STREAM, 0);                        // 定义套接字类型   
		listen_sockaddr.sin_family = AF_INET;
		listen_sockaddr.sin_port = htons(MYPORT);
		listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
        listen_len = sizeof(listen_sockaddr);
               		                                 
		int on = 1;                              //允许重复使用本地地址和套接字绑定
		setsockopt(listen_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
		if(bind(listen_sockfd,(struct sockaddr *)&listen_sockaddr,listen_len)==-1)                
		{
			perror("bind");
			exit(1);
        }
        else
        {
            printf("A new socket bind success\n");
        }   
	
		if(listen(listen_sockfd,5) == -1)
		{
			perror("listen");
			exit(1);   
		}
		
        client_len = sizeof(client_sockaddr);
        //listen_len = sizeof(listen_sockaddr);

        if((client_sockfd=accept(listen_sockfd,(struct sockaddr *)&client_sockaddr,&client_len))==-1)
		{
			 perror("accept error");
			 exit(1);
        }
        else                                     //connect success
        {
            recv(client_sockfd, buf, 100, 0);    //reserve log request

            if(buf[1]=='0')                      //log in
            {
                if(check_login(buf+3))           //log in success
                {
                    client_ip=client_sockaddr.sin_addr;
                    itoa(PORT+userNum,buf);
                    send(client_sockfd,buf,strlen(buf),0);
                }
                else                             
                {
                    send(client_sockfd,"-1",strlen("-1"),0);
                }
            }
            else if(buf[1]=='1')                 //sign up
            {
                if(check_signup(buf+3))          //sign up success
                {
                    send(client_sockfd,"0",strlen("0"),0);
                }
                else
                {
                    send(client_sockfd,"-1",strlen("-1"),0);
                }
            }
            else                                 //invalid 
            {
                send(client_sockfd,"Invalid command",strlen("Invalid command"),0);
            }
        }
        close(listen_sockfd);
    }

}