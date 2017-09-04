/********************************************************************************
 * Files         : server.c
 * Description   : the server mode of the uTalk.
 * Author        : xiaoziyuan,sunjinda
 * Last Modified : 2017.09.02
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
 #define MAX_USER_NUM 10
 int client_sockfd;
 int _2_server_sockfd;
 int userNum;   
 char online_user[MAX_USER_NUM][10];
 int  ary_sockfd[MAX_USER_NUM];                         //arrar for clients' sockfd
 int global_current_sockfd;
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
    user_name[j]=0;
    i+=10; j=0;
    while (line[i]!=';')
    {
	user_pw[j]=line[i];
	i++; j++;
    }
    user_pw[j]=0;

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
Description : check if the user exist
Prameter    : Function done
Return      : int (0=no,1=exist)
Side effect : none
Author      : sunjinda
Date        : 2017.09.04
Time        : 07:46:00
********************************************************************************/
int if_user_exist(const char* line)
{
    //making connection 
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"select * from alluser where username = '%s';",line);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    int ans=0;
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
Description : add friend in database
Prameter    : Function done
Return      : void
Side effect : none
Author      : sunjinda
Date        : 2017.09.04
Time        : 08:12:00
********************************************************************************/
void insert_friend_into_database(const char* name1,const char* name2)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    //check if the username exit
    char comm[1024];
    int ans=0;
    sprintf(comm,"select * from friend_list where p1 = '%s' and p2 = '%s';",name1,name2);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
        ans++;        
    }
    if(ans!=0)                            //if user_name exist!
    {
        mysql_free_result(res);
        mysql_close(conn);
        return ;
    }
    else  //start insertion
    {
        char comm1[1024] ="\0";
        sprintf(comm1,"insert into friend_list values ('%s','%s');",name1,name2);
        if (mysql_query(conn,comm1))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
        char comm2[1024] ="\0";
        sprintf(comm2,"insert into friend_list values ('%s','%s');",name2,name1);
        if (mysql_query(conn,comm2))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
    }
    return;
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
 * Description : 
 * Prameter    : char*,the name which you want to check
 * Return      : -1               :not online 
                non-negative int  :the position in the array online_user
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 2017.09.02
 * Time        : 
 ********************************************************************************/
int if_user_online(const char *name)
{
    int i;
    for(i=0;i<MAX_USER_NUM;i++)
        if(strcmp(online_user[i],name)==0)
            break;
    if(i==MAX_USER_NUM)
        return -1;
    return i;
}

 /********************************************************************************
 * Description : 
 * Prameter    : 
 * Return      : 
  
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 
 * Time        : 
 ********************************************************************************/
 int process_command(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char*original_msg)
 {
    char opp_name[100];
    char self_name[100];
    char msg_to_opp[100];
    char temp[100];
    int i;
    int opp_userID;
    int len;
    FILE* p_offline_msg=NULL;

    if(original_msg[1]=='0')                     //log in
    {
        if(check_login(original_msg+3))                   //log in success
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"0");

            for(i=0;i<80&&original_msg[i+12]!=';';i++)                         //add name into online_user
                self_name[i]=original_msg[i+12];
            self_name[i]=0;
            strcpy(online_user[current_userID],self_name);

            /*strcpy(temp,"offline_msg/");
            strcat(temp,self_name);
            p_offline_msg=fopen(temp,"r");

            p_msg_to_slef[0]=0;
            while(fgets(temp,99,p_offline_msg))
            {
                strcat(p_msg_to_slef,temp);
            }
            fclose(p_offline_msg);

            strcpy(temp,"offline_msg/");
            strcat(temp,self_name);
            p_offline_msg=fopen(temp,"w");
            fclose(p_offline_msg);*/

            printf("%s had log in\n",self_name);
        }
        else                             
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"-1");
        }
    }
    else if(original_msg[1]=='1')                //sign up
    {
        if(check_signup(original_msg+3))          //sign up success
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"0");
            for(i=0;i<80&&original_msg[i+12]!=';';i++)
                self_name[i]=original_msg[i+12];
             self_name[i]=0;

             strcpy(temp,"offline_msg/");
             strcat(temp,self_name);
             p_offline_msg=fopen(temp,"w");
             fclose(p_offline_msg);

             printf("%s sign up\n",self_name);
        }
        else
        {
            *p_to_sockfd=0;
            strcpy(p_msg_to_slef,"-1");
        }
    }
    else if(original_msg[1]=='2')
    {

    }
    else if(original_msg[1]=='3')
    {
        if(original_msg[3]=='0')
        {
            strcpy(opp_name,original_msg+5);
            len = strlen(opp_name);
            opp_name[len-1]=0;
            
            if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                *p_msg_to_opp=0;
    
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
                p_offline_msg=fopen(temp,"a");

                strcpy(p_msg_to_opp,"/3:2,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
                fprintf(p_offline_msg,"%s\n",p_msg_to_opp);

                fclose(p_offline_msg);
            }
            else
            {
                *p_to_sockfd=ary_sockfd[opp_userID];
    
                strcpy(p_msg_to_opp,"/3:2,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                printf("%s",p_msg_to_opp);
                strcat(p_msg_to_opp,"*");
            }
        }
        else if(original_msg[3]=='1')
        {
            strcpy(opp_name,original_msg+5);
            len = strlen(opp_name);
            opp_name[len-1]=0;
            
            if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                *p_msg_to_opp=0;
    
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
                p_offline_msg=fopen(temp,"a");

                strcpy(p_msg_to_opp,"/3:3,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
                fprintf(p_offline_msg,"%s\n",p_msg_to_opp);

                fclose(p_offline_msg);
            }
            else
            {
                *p_to_sockfd=ary_sockfd[opp_userID];
    
                strcpy(p_msg_to_opp,"/3:3,");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
            }
        }
        else if(original_msg[3]=='2')
        {}
        else if(original_msg[3]=='3')
        {}
        else
        {
            strcpy(opp_name,original_msg+3);
            len = strlen(opp_name);
            opp_name[len-1]=0;
            insert_friend_into_database(online_user[current_userID],opp_name);

            if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
            {
                *p_msg_to_opp=0;
    
                strcpy(temp,"offline_msg/");
                strcat(temp,opp_name);
                p_offline_msg=fopen(temp,"a");

                strcpy(p_msg_to_opp,"/3:");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");

                fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
                fclose(p_offline_msg);
            }
            else
            {
                *p_to_sockfd=ary_sockfd[opp_userID];
    
                strcpy(p_msg_to_opp,"/3:");
                strcat(p_msg_to_opp,online_user[current_userID]);
                strcat(p_msg_to_opp,"*");
            }
        }
    }
    else if(original_msg[1]=='4')
    {
        *p_to_sockfd=0;
        
        strcpy(temp,"offline_msg/");
        strcat(temp,online_user[current_userID]);
        p_offline_msg=fopen(temp,"r");

        p_msg_to_slef[0]=0;
        while(fgets(temp,99,p_offline_msg))
        {
            strcat(p_msg_to_slef,temp);
        }
        fclose(p_offline_msg);
        printf("%s",p_msg_to_slef);

        strcpy(temp,"offline_msg/");
        strcat(temp,online_user[current_userID]);
        p_offline_msg=fopen(temp,"w");
        fclose(p_offline_msg);
    }
 }

  /********************************************************************************
 * Description : 
 * Prameter    :         strcpy(temp,"offline_msg/");
            strcat(temp,self_name);
 * Return      : 
  
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 
 * Time        : 
 ********************************************************************************/
 int process_msg(int* p_to_sockfd,int current_userID,char* p_msg_to_slef,char*p_msg_to_opp,const char*original_msg)
 {
    char opp_name[100];
    char msg_to_opp[100];
    char temp[100];
    int i;
    int opp_userID;
    int len;
    FILE* p_offline_msg=NULL;

    for(i=0;i<100&&original_msg[i]!=':';i++)
        opp_name[i]=original_msg[i];
    opp_name[i]=0;

    if(if_user_exist(opp_name)==0)              
    {
        strcpy(p_msg_to_slef,"User not exist\n");
        *p_msg_to_opp=0;
        return 0;
    }
    else
    {
        for(i=0;i<100&&original_msg[i]!=':';i++)
            opp_name[i]=original_msg[i];
        opp_name[i]=0;

        if((opp_userID=if_user_online(opp_name))==-1)         //Friend not online
        {
            strcpy(p_msg_to_slef,"User not online\n");
            *p_msg_to_opp=0;

            strcpy(temp,"offline_msg/");
            strcat(temp,opp_name);
            p_offline_msg=fopen(temp,"a");

            strcpy(p_msg_to_opp,online_user[current_userID]);
            strcat(p_msg_to_opp,":");
            len=strlen(online_user[current_userID])+1;
            strcat(p_msg_to_opp,original_msg+len);
            //printf("%s\n",p_msg_to_opp);
            fprintf(p_offline_msg,"%s\n",p_msg_to_opp);
            fclose(p_offline_msg);
        }
        else
        {
            *p_to_sockfd=ary_sockfd[opp_userID];

            strcpy(p_msg_to_opp,online_user[current_userID]);
            strcat(p_msg_to_opp,":");
            len=strlen(online_user[current_userID])+1;
            strcat(p_msg_to_opp,original_msg+len);

            strcpy(p_msg_to_slef,"get\n");
        }
    }
    return 1;
 }

 /********************************************************************************
 * Description : int to string
 * Prameter    : int n:a in number   const char* str:destination
 * Return      : void
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 2017.09.02
 * Time        : 11:03:00
 ********************************************************************************/
 void* _pthread_entrance(void* p)
 {
    char buf[100];
    char temp[100];
    char msg_to_self[100];
    char msg_to_opp[100];
    char to_who[100];
    int recvbytes;
    int i;
    int current_user_ID;
    int current_user_sockfd=global_current_sockfd;
    int opposite_user_sockfd;

    for(i=0;i<MAX_USER_NUM;i++)
        if(current_user_sockfd==ary_sockfd[i])
            break;
    current_user_ID=i;

    while(1)
    {
        memset(buf,0,100);
        recvbytes=recv(current_user_sockfd, buf, 100, 0);    //reserve log request
       
        printf("%s\n",buf);
        if(recvbytes==0)
        {
            userNum--;
            memset(online_user[current_user_ID],0,10);
            return NULL;
        }
        else
        {
            if(buf[0]=='/')
            {
                process_command(&opposite_user_sockfd,current_user_ID,msg_to_self,msg_to_opp,buf);
                printf("%s",msg_to_self);
                send(current_user_sockfd,msg_to_self,strlen(msg_to_self),0);
                //send(current_user_sockfd,"0",strlen("0"),0);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,strlen(msg_to_opp),0);
                }
                /*if(buf[1]=='0')                      //log in
                {
                    if(check_login(buf+3))           //log in success
                    {
                        send(current_user_sockfd,"0",strlen("0"),0);
                    }
                    else                             
                    {
                        send(current_user_sockfd,"-1",strlen("-1"),0);
                        while(1)
                        {
                            recv(current_user_sockfd, buf, 100, 0);
                            if(check_login(buf+3))
                            {
                                send(current_user_sockfd,"0",strlen("0"),0);
                                break;
                            }
                            else
                            {
                                send(current_user_sockfd,"-1",strlen("-1"),0);
                            }
                        }
                    }
                }
                else if(buf[1]=='1')                 //sign up
                {
                    if(check_signup(buf+3))          //sign up success
                    {
                        send(current_user_sockfd,"0",strlen("0"),0);
                    }
                    else
                    {
                        send(current_user_sockfd,"-1",strlen("-1"),0);
                    }
                }
                else if(buf[1]=='2')                           //get name
                {
                    for(i=0;i<100&&buf[i+3]!=';';i++)
                        temp[i]=buf[i+3];
                    temp[i]=0;
                    strcpy(online_user[current_user_ID],temp);
                    printf("%s had log in\n",temp);
                }
                else                                 //invalid 
                {
                    send(current_user_sockfd,"Invalid command",strlen("Invalid command"),0);
                }*/
            }
            else
            {
                process_msg(&opposite_user_sockfd,current_user_ID,msg_to_self,msg_to_opp,buf);
                send(current_user_sockfd,msg_to_self,strlen(msg_to_self),0);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,strlen(msg_to_opp),0);
                }
                /*for(i=0;i<100&&buf[i]!=':';i++)
                    to_who[i]=buf[i];
                to_who[i]=0;

                if(if_user_online(to_who)==-1)
                {
                    send(current_user_sockfd,"User not online",strlen("User not online"),0);
                }
                else
                {
                    strcpy(send_msg,online_user[current_user_ID]);
                    strcat(send_msg,":");
                    strcat(send_msg,buf+i);
                    //send(ary_sockfd[if_user_online(to_who)],online_user[current_user_ID],strlen(online_user[current_user_ID]),0);
                    //send(ary_sockfd[if_user_online(to_who)],buf+i,strlen(buf+i),0);
                    send(ary_sockfd[if_user_online(to_who)],send_msg,strlen(send_msg),0);
                    //send(current_user_sockfd,"msg had sent to friend",strlen("msg had sent to friend"),0);
                }*/

            }
        }
        
    }
 }
 /********************************************************************************
 * Description : After user clicked the signin button in signin window, signin.
 * Prameter    : Function is not completed.
 * Return      : void
 * Side effect : Function is not completed.
 * Author      : xiaoziyuan
 * Date        : 2017.09.02
 * Time        : 23:43
 ********************************************************************************/
int main()
{
 
    char buf[100];
    int listen_sockfd;                           //server's listen socketfd
    //int client_sockfd;                           //client's listen socketfd
   // int _2_server_sockfd;                           //chat socketfd
    socklen_t listen_len;
    socklen_t client_len;
    socklen_t server_len;
    struct sockaddr_in listen_sockaddr;
    struct sockaddr_in client_sockaddr;
    struct sockaddr_in server_sockaddr;
                                  //the number of current online user 
    struct in_addr client_ip;                    //the client's address information
    pthread_t ptid[MAX_USER_NUM];
    printf("\n======================server initialization======================\n");
    while(1)
    {
        struct in_addr sin_addr;

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
            //printf("A new socket bind success\n");
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
        {   sin_addr=client_sockaddr.sin_addr;
            itoa(PORT+userNum,buf);
			send(client_sockfd,buf,strlen(buf),0);
        }


        _2_server_sockfd = socket(AF_INET,SOCK_STREAM, 0); // 定义套接字类型   
		server_sockaddr.sin_family = AF_INET;
		server_sockaddr.sin_port = htons(PORT+userNum);
		server_sockaddr.sin_addr=sin_addr; 
		server_len = sizeof(server_sockaddr);
		bzero(&(server_sockaddr.sin_zero),sizeof(server_sockaddr.sin_zero)); 
		sleep(1);
        ary_sockfd[userNum]=_2_server_sockfd;
        global_current_sockfd=_2_server_sockfd;
		
		if (connect(_2_server_sockfd, (struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr_in)) == -1) 
		{ 
			perror("connect");
			exit(1); 
		}
		else
		{
			userNum++;
        }
		pthread_create(&ptid[userNum],NULL,_pthread_entrance,NULL);
        close(listen_sockfd);
    }
}

