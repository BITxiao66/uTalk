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

void init_db();

void init_db()
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
    
    //ending
    mysql_free_result(res);
    mysql_close(conn);
}

int main() 
{
    init_db();
    return 0;
}
