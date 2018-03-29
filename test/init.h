#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>   //struct sockaddr_in, htonl(), htons()
#include <string.h>       //bzero()
#include <sys/socket.h>   //socket(), bind(), listen()

#define PORT 4000
#define IPADDR INADDR_ANY
#define LIS_QUEUE_SIZE 5
typedef sockaddr SA;


int socket_bind_listen();


#endif
