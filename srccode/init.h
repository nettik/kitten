#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>   //struct sockaddr_in, htonl(), htons()
#include <string.h>       //bzero()
#include <sys/socket.h>   //socket(), bind(), listen()
#include <pthread.h>      //pthread_create(), pthread_detach()
#include <stdio.h>        //printf()
#include <arpa/inet.h>    //inet_ntop()
#include <unistd.h>       //close()
#include <string.h>
#include <unordered_map>
#include <sys/epoll.h>
#include <malloc.h>       //free()

#define PORT 4000
#define IPADDR INADDR_ANY
#define LIS_QUEUE_SIZE 5
#define EPOLL_LIS_SIZE 1000
#define EPOLLEVENTS 200
#define MAX_SIZE 1024

static pthread_mutex_t lock_task = PTHREAD_MUTEX_INITIALIZER;

typedef sockaddr SA;


int socket_bind_listen();


#endif
