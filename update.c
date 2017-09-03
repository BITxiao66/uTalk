#ifndef SJD_HEAD
#define SJD_HEAD

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
char server[] = "0";       //server ID (you can search it in mysql)
char user[] = "root";      //USER name (nevermind)
char password[] = "961009";//password (sunjinda's birthday)
char database[] = "utalk"; //database name (this is a default database)
#endif

void update_db(const char user_name[],const char user_pw[]);

void update_db(const char user_name[],const char user_pw[])
{
    //making connection
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    //check if the username exit
    char comm[1024]="\0";
    int ans=0;
    sprintf(comm,"select * from alluser where username = '%s';", user_name);
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
    if(ans==0)                            //if user_name doesn't exist
    {
	printf("username doesn't exist!\n");
        mysql_free_result(res);
        mysql_close(conn);
        return ;
    }
   
    //start update
    char comm1[1024] ="\0";
    sprintf(comm1,"update alluser set password = '%s' where username = '%s';",
    user_pw,user_name);
    if (mysql_query(conn,comm1))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    //ending
    mysql_free_result(res);
    mysql_close(conn);
    printf("finish! \n");
}

int main() 
{
    char line1[20]="daddy";
    char line2[20]="123456";
    update_db(line1,line2);
    return 0;
}
