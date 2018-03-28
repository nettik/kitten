#ifndef RESPONSE_H
#define RESPONSE_H
 
#include <sys/socket.h>   //send(), recv(), getpeername()
#include <stdio.h>        //printf()
#include <netinet/in.h>   //ntohs(), struct sockaddr_in
#include <arpa/inet.h>    //inet_ntop()
#include <unistd.h>       //close()

#define MAX_SIZE 1024
typedef sockaddr SA;

void string_echo(int connfd);



#endif
