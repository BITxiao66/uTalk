#ifndef _CON_SERVER_H
#define _CON_SERVER_H

/********************************************************************************
 * Files         : msg_server.h
 * Description   : the head file of msg_server.c.
 * Author        : xiaoziyuan,xuda
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

 int online();
 int send_msg_to_server(const char* to_who,const char *msg);
 int get_msg_from_server();

/********************************************************************************
 * Description : Declaration of all global variables in msg_server.c
 * Author      : 
 * Date        : 
 * Time        : 
 ********************************************************************************/
 extern int client_sockfd;
 
 
 /********************************************************************************
  * Description : Declaration of all functions in msg_server.c
  * Author      : 
  * Date        : 
  * Time        : 
  ********************************************************************************/
int online();//只供测试版本使用
 
  #endif
