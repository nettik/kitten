#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>   //struct sockaddr_in, htonl(), htons()
#include <string.h>       //bzero(), strncpy()
#include <sys/socket.h>   //socket(), bind(), listen()
#include <pthread.h>      //pthread_create(), pthread_join()
#include <stdio.h>        //printf()
#include <arpa/inet.h>    //inet_ntop()
#include <unistd.h>       //close()
#include <string.h>
#include <sys/epoll.h>
#include <malloc.h>       //free(), malloc()
#include <fcntl.h>        //fcntl()
#include <errno.h>
#include <stdio.h>        //perror(), sprintf()
#include <string>         //string

#define PORT 4000
#define IPADDR INADDR_ANY
#define LIS_QUEUE_SIZE 5
#define EPOLL_LIS_SIZE 1000
#define EPOLLEVENTS 200
#define MAX_SIZE 1024
#define BUF_SIZE 512
#define THREAD_NUM 4
#define DEFAULT_PATH "/home/meow/kitten/webpage"

typedef sockaddr SA;


int socket_bind_listen();

int make_socket_nonblock(int sockfd);

#endif
