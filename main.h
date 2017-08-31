/********************************************************************************
 * Files         : main.h
 * Description   : the head file of main.c.
 * Author        : xinchengxin
 * Last Modified : 2017.08.31, 16:29:31
 ********************************************************************************/


/********************************************************************************
 * Description : Declaration of all global variables in main.c. (add all global variables appearing in main.c here)
 * Author      : xinchengxin
 * Date        : 2017.08.31
 * Time        : 15:38:06
 ********************************************************************************/
 GtkApplication *app;//uTalk application
 
 
 /********************************************************************************
  * Description : Declaration of all functions in main.c. (add all function appearing in main.c here)
  * Author      : xinchengxin
  * Date        : 2017.08.31
  * Time        : 15:38:06
  ********************************************************************************/
 static void user_signin(GtkWidget* widget, gpointer data);
 static void user_signup(GtkWidget* widget, gpointer data);
 static void to_signin(GtkWidget *window);
 static void to_signup(GtkWidget *window);
 static void create_signup_window();
 static void create_signin_window();