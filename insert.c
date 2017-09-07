#ifndef SJD_HEAD
#define SJD_HEAD

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
char server[] = "0";       //server ID (you can search it in mysql)
char user[] = "root";      //USER name (nevermind)
char password[] = "961009";//password (sunjinda's birthday)
char database[] = "utalk"; //database name (this is a default database)
#endif


void insert_db(const char user_name[],const char user_pw[]);

void insert_db(const char user_name[],const char user_pw[])
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
    sprintf(comm,"select * from alluser where username = '%s';",user_name);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s %s\n", row[0],row[1]); //remember the output number!!
        ans++;        
    }
    if(ans!=0)                            //if user_name exist!
    {
	    printf("username exist!\n");
        //mysql_free_result(res);
        //mysql_close(conn);
        //return ;
    }
    else  //start insertion
    {
        char comm1[1024] ="\0";
        sprintf(comm1,"insert into alluser values ('%s','%s');",
	    user_name,user_pw);
        if (mysql_query(conn,comm1))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            //exit(1);
        }
    }
    
    //timer
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    char now_time[20];
    sprintf(now_time,"%d-%d-%d-%d-%d-%d",
	    timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday,
	    timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
    //printf("%s\n",time);

    //insert time
    char comm2[1024] ="\0";
    sprintf(comm2,"insert into talk values ('%s','%s');",user_name,now_time);
    if (mysql_query(conn,comm2))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }

    //ending
    mysql_free_result(res);
    mysql_close(conn);
    printf("finish! \n");
}

int main() 
{
    char line1[20]="daddy";
    char line2[20]="654321";
    insert_db(line1,line2);
    return 0;
}
