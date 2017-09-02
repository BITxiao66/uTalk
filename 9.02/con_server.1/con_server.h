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
 #include <sys/types.h> 
 #include <netinet/in.h> 
 #include <sys/socket.h> 
 #include <sys/wait.h> 
 #include<unistd.h> 
 #include <arpa/inet.h> 
 #include<fcntl.h>
 #include<sys/shm.h>

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
  int online(const char*server_ip);
  int send_login(char *user_name,char *password);
  int send_signup(char *user_name,char *password);
  
  #endif
