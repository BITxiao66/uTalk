#ifndef SJD_HEAD
#define SJD_HEAD

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
char server[] = "0";       //server ID (you can search it in mysql)
char user[] = "root";      //USER name (nevermind)
char password[] = "961009";//password (sunjinda's birthday)
char database[] = "utalk"; //database name (this is a default database)
#endif

void select_db(const char user_name[]);

void select_db(const char user_name[])
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

    //select data
    char comm[1024]="\0";
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
	    ans++;
        printf("%s %s\n", row[0],row[1]); //remember the output number!!
    }
    if(ans==0)
    {
	printf("message not found!\n");
    }
   
    //ending
    mysql_free_result(res);
    mysql_close(conn);
    printf("finish! \n");
}

int main() 
{
    char line1[20]="daddy";
    select_db(line1);
    return 0;
}
