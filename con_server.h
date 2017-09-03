#ifndef _CON_SERVER_H
#define _CON_SERVER_H

/********************************************************************************
 * Files         : con_server.h
 * Description   : the head file of con_server.c.
 * Author        : xiaoziyuan
 * Last Modified : 
 ********************************************************************************/

 #include <pthread.h>
 #include <errno.h> 
 #include <netinet/in.h> 
 #include <unistd.h> 
 #include <arpa/inet.h> 
 #include <fcntl.h>

/********************************************************************************
 * Description : Declaration of all global variables in con_server.c
 * Author      : 
 * Date        : 
 * Time        : 
 ********************************************************************************/
 extern int client_sockfd;
 
 
 /********************************************************************************
  * Description : Declaration of all functions in con_server.c
  * Author      : 
  * Date        : 
  * Time        : 
  ********************************************************************************/
  extern int online();
  extern int send_login(const char *user_name,const char *password);
  extern int send_signup(const char *user_name,const char *password);
  
  #endif
