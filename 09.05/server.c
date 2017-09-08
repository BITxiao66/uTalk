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

 #define MYPORT    8000                          //server's listen port
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
    while (line[i]&&line[i]!=';')
    {
	user_name[j]=line[i];
	i++; j++;
    }
    user_name[j]=0;
    i+=10; j=0;
    while (line[i]&&line[i]!=';')
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
        char comm2[1024]="\0";
        sprintf(comm2,"insert into avator values ('%s',0)",user_name);
        if (mysql_query(conn,comm2));
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
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
  
    //get name and password
    char user_name[20];
    char user_pw[20];
    int i=9,j=0;
    while (line[i]&&line[i]!=';')
    {
	user_name[j]=line[i];
	i++; j++;
    }
    user_name[j]=0;
    i+=10; j=0;
    while (line[i]&&line[i]!=';')
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
        ///fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
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
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
   
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"select * from alluser where username = '%s';",line);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
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
int search_avator_from_db(const char* username)
{
    //making connection 
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
   
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"select number from avator where username = '%s';",username);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    int ans;
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
	    ans = atoi(row[0]);
        //printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}

void change_avator_in_db(const char* username,int id)
{
    //making connection 
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
   
    //start deletion
    char comm[1024] ="\0";
    sprintf(comm,"update avator set number = %d where username = '%s';",id,username);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    mysql_close(conn);
    return;
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
       // fprintf(stderr, "%s\n", mysql_error(conn));
       // exit(1);
    }

    //check if the username exit
    char comm[1024];
    int ans=0;
    sprintf(comm,"select * from friend_list where p1 = '%s' and p2 = '%s';",name1,name2);
    if (mysql_query(conn,comm))
    {
       // fprintf(stderr, "%s\n", mysql_error(conn));
       // exit(1);
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
            //fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
        char comm2[1024] ="\0";
        sprintf(comm2,"insert into friend_list values ('%s','%s');",name2,name1);
        if (mysql_query(conn,comm2))
        {
           // fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
    }
    mysql_free_result(res);
    mysql_close(conn);
    return;
}


/********************************************************************************
Description : get friend list
Prameter    : Function done
Return      : int (number of friends)
Side effect : none
Author      : sunjinda
Date        : 2017.09.04
Time        : 10:43:00
********************************************************************************/
/*
int get_friend_list_from_db(const char* name,char* list)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
       // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }

    //check if the username exit
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select p2 from friend_list where p1 = '%s';",name);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s\n", row[0]); //remember the output number!!
        ans++; 
        strcat(line,row[0]);
        strcat(line,"\n");
    }
    //strcat(line,0);
    strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}*/

int get_friend_list_from_db(const char* name,char* list)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
       // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }

    //check if the username exit
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select friend_list.p2,avator.number from friend_list,avator where friend_list.p2=avator.username and p1 = '%s';",name);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0]); //remember the output number!!
        ans++; 
        strcat(line,row[0]);
        strcat(line,"\n");
        strcat(line,row[1]);
        strcat(line,"\n");
    }
    //strcat(line,0);
    strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
}
int search_user_from_db(const char* name,char* list)
{
    //making connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
       // fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }

    //check if the username exit
    char comm[1024]="\0",line[2000]="\0";
    //memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select * from avator where username like '%%%s%%';",name);
    if (mysql_query(conn,comm))
    {
        //fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //printf("%s %s\n", row[0],row[0]); //remember the output number!!
        ans++; 
        strcat(line,row[0]);
        strcat(line,"\n");
        strcat(line,row[1]);
        strcat(line,"\n");
    }
    //strcat(line,0);
    strcpy(list,line);
    mysql_free_result(res);
    mysql_close(conn);
    return ans;
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
 * Return         : 
  
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
    char temp1[100];                             //  using in case '2'
    int i;
    int opp_userID;
    int len;
    int friend_num;
    int trash;
    int avatorID;
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
    else if(original_msg[1]=='2')                //get friend list
    {
        *p_to_sockfd=0;
        //char *friend_list[10];
        strcpy(temp,original_msg+3);
        len=strlen(temp);
        temp[len-1]=0;
        friend_num = get_friend_list_from_db(temp,temp1);
        
        
        strcpy(p_msg_to_slef,"/2* ");

        itoa(friend_num,temp);
        strcat(p_msg_to_slef,temp);

        strcat(p_msg_to_slef," ");
        strcat(p_msg_to_slef,temp1);

        printf("%s",temp1);
    }
    else if(original_msg[1]=='3')                //add friend
    {
        if(original_msg[3]=='0')
        {
            strcpy(opp_name,original_msg+4);
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
                
                strcat(p_msg_to_opp,"*");
            }
        }
        else if(original_msg[3]=='1')
        {
            strcpy(opp_name,original_msg+4);
            len = strlen(opp_name);
            opp_name[len-1]=0;

            insert_friend_into_database(online_user[current_userID],opp_name);
            
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
        {

        }
        else if(original_msg[3]=='3')
        {}
        else
        {
            strcpy(opp_name,original_msg+3);
            len=strlen(opp_name);
            opp_name[len-1]=0;
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
    else if(original_msg[1]=='4')                //get offline msg
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
    else if(original_msg[1]=='5')                //if friend exist
    {
        *p_to_sockfd=0;
        strcpy(temp,original_msg+3);
        len=strlen(temp);
        temp[len-1]=0;

        friend_num = search_user_from_db(temp,temp1);
        
        
        strcpy(p_msg_to_slef,"/5* ");

        itoa(friend_num,temp);
        strcat(p_msg_to_slef,temp);

        strcat(p_msg_to_slef," ");
        strcat(p_msg_to_slef,temp1);
    }
    else if(original_msg[1]=='6')                //get history msg from server
    {
        
    }
    else if(original_msg[1]=='7')                //set my avator in DB
    {
        *p_to_sockfd=0;
        avatorID=original_msg[3]-'0';
        change_avator_in_db(online_user[current_userID],avatorID);
        strcpy(p_msg_to_slef,"/7*");
    
    }
    else if(original_msg[1]=='8')                //search avator from DB
    {
        *p_to_sockfd=0;
        avatorID=search_avator_from_db(online_user[current_userID]);
        strcpy(p_msg_to_slef,"/8:0*");
        p_msg_to_slef[3]=avatorID+'0';        

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
            len=strlen(opp_name)+1;
            strcat(p_msg_to_opp,original_msg+len);

            strcpy(p_msg_to_slef,"get\n");
        }
    }
    return 1;
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
int process_file(int* p_to_sockfd,int current_userID,char *p_msg_to_opp,const char*original_msg,int len)
{
    char str[100];
    char temp[100];
    char opp_name[100];
    char file_name[100];
    int i;
    int len1;
    int opp_userID;
    
    /*for(i=0;i<90&&original_msg[i+9];i++)
        opp_name[i]=original_msg[i+9];
    opp_name[i]=0;

    for(i=0;i<90&&original_msg[i+1];i++)
        file_name[i]=original_msg[i+1];
    file_name[7]=0;*/

    sscanf(original_msg+1,"%s %s",file_name,opp_name);

    opp_userID=if_user_online(opp_name);

    *p_to_sockfd=ary_sockfd[opp_userID];

    
    memset(p_msg_to_opp,' ',15);

    p_msg_to_opp[0]='#';
    strcpy(p_msg_to_opp+1,file_name);
    len1=strlen(p_msg_to_opp);
    p_msg_to_opp[len1]=' ';

    strcpy(p_msg_to_opp+9,online_user[current_userID]);
    len1=strlen(p_msg_to_opp);
    p_msg_to_opp[len1]=' ';
    p_msg_to_opp[14]=0;

    for(i=0;i<len;i++)
        memcpy(p_msg_to_opp+i+15,original_msg+i+15,1);
    
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
        if(recvbytes<=0)
        {
            userNum--;
            memset(online_user[current_user_ID],0,10);
            ary_sockfd[current_user_ID]=0;
            close(current_user_sockfd);
            return NULL;
        }
        else
        {
            if(buf[0]=='/')
            {
                process_command(&opposite_user_sockfd,current_user_ID,msg_to_self,msg_to_opp,buf);
                send(current_user_sockfd,msg_to_self,strlen(msg_to_self)+1,0);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,strlen(msg_to_opp)+1,0);
                }
            }
            else if(buf[0]=='#')
            {
                process_file(&opposite_user_sockfd,current_user_ID,msg_to_opp,buf,recvbytes);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,recvbytes,0);
                }
            }
            else
            {
                process_msg(&opposite_user_sockfd,current_user_ID,msg_to_self,msg_to_opp,buf);
                //send(current_user_sockfd,msg_to_self,strlen(msg_to_self)+1,0);
                if(opposite_user_sockfd>0)
                {
                    send(opposite_user_sockfd,msg_to_opp,strlen(msg_to_opp)+1,0);
                }

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
    int i;
 
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
            //userNum++;
        }
        for(i=0;i<MAX_USER_NUM;i++)
            if(ary_sockfd[i]==0)
                break;
        //ary_sockfd[i]=client_sockfd;
        //global_current_sockfd=client_sockfd;

        //pthread_create(&ptid[i],NULL,_pthread_entrance,NULL);

        _2_server_sockfd = socket(AF_INET,SOCK_STREAM, 0); // 定义套接字类型   
		server_sockaddr.sin_family = AF_INET;
		server_sockaddr.sin_port = htons(PORT+userNum);
		server_sockaddr.sin_addr=sin_addr; 
		server_len = sizeof(server_sockaddr);
		bzero(&(server_sockaddr.sin_zero),sizeof(server_sockaddr.sin_zero)); 
        sleep(1);
        for(i=0;i<MAX_USER_NUM;i++)
            if(ary_sockfd[i]==0)
                break;
        ary_sockfd[i]=_2_server_sockfd;
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
