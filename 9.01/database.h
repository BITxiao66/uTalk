/********************************************************************************
 * Files         : database.h
 * Description   : the head file of database.c.
 * Author        : sunjinda
 * Last Modified : 2017.09.02, 10:00:00
 ********************************************************************************/


/********************************************************************************
 * Description : Declaration of all global variables in database.c. (add all global variables appearing in database.c here)
 * Author      : sunjinda
 * Date        : 2017.09.02
 * Time        : 10:01:01
 ********************************************************************************/

char server[] = "0";       //server ID (you can search it in mysql)
char user[] = "root";      //USER name (nevermind)
char password[] = "123456";//password (sunjinda's birthday)(not anymore)
char database[] = "utalk"; //database name (this is NOT a default database)

/********************************************************************************
Description : Declarations of all functions in databases.c (add all function appearing in database.c here)
Author      : sunjinda
Date        : 2017-09-02
Time        : 09:51
********************************************************************************/
int check_signup(const char* line);
int check_login(const char* line);
