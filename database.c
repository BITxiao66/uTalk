
/********************************************************************************
 * Files         : database.c
 * Description   : the entrance of the database
 * Author        : sunjinda
 * Last Modified : 2017.09.02, 10:03:00
 ********************************************************************************/
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "database.h"


/********************************************************************************
Description : create tables
Prameter    : Function done
Return      : void
Side effect : create tables in utalk(a database)
Author      : sunjinda
Date        : 2017.09.02
Time        : 10:35:00
********************************************************************************/
void create_tables()
{
    //ready to connect by using pointer
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   
    //"mysql_query()" sent your command to mysql 
    //if something wrong ,"mysql_error" works
    if (mysql_query(conn,"create table if not exists alluser (username char(20),password char(20));"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    if (mysql_query(conn,"create table if not exists talk (host char(20),time datetime);"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }
    if (mysql_query(conn,"create table if not exists friend_list (p1 char(20),p2 char(20));"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        //exit(1);
    }

    //ending
    mysql_free_result(res);
    mysql_close(conn);
}

/********************************************************************************
Description : sign up in database
Prameter    : Function done
Return      : int (1 == success, 0 == failed)
Side effect : change database
Author      : sunjinda
Date        : 2017.09.02
Time        : 14:20:00
********************************************************************************/
int check_signup(const char* line)
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
    while (linoe[i]!=';')
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
    }
    mysql_free_result(res);
    mysql_close(conn);
	return 1;
}

/********************************************************************************
Description : login in database
Prameter    : Function done
Return      : int (1 == success, 0 == failed)
Side effect : change database
Author      : sunjinda
Date        : 2017.09.02
Time        : 14:21:00
********************************************************************************/
int check_login(const char* line)
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
Description : clean table alluser
Prameter    : Function done
Return      : void
Side effect : clean table alluser in utalk(a database)
Author      : sunjinda
Date        : 2017.09.02
Time        : 10:35:00
********************************************************************************/
void alluser_clean()
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
    char comm1[1024] ="\0";
    sprintf(comm1,"delete from alluser;");
    if (mysql_query(conn,comm1))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    //ending
    mysql_free_result(res);
    mysql_close(conn);
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
Description : get frirnd list
Prameter    : Function done
Return      : int (number of friends)
Side effect : none
Author      : sunjinda
Date        : 2017.09.04
Time        : 10:43:00
********************************************************************************/
int get_friend_list_from_db(const char* name,char* list)
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
    char comm[1024],line[2000];
    memset(line,0,sizeof(line));
    int ans=0;
    sprintf(comm,"select p2 from friend_list where p1 = '%s';",name);
    if (mysql_query(conn,comm))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn); 
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s\n", row[0]); //remember the output number!!
        ans++; 
        strcat(line,row[0]);
        strcat(line,"\n");
    }
    strcat(line,0);
    return ans;
}