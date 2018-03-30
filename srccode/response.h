#ifndef RESPONSE_H
#define RESPONSE_H
 
#include <sys/socket.h>   //send(), recv(), getpeername()
#include <stdio.h>        //printf()
#include <netinet/in.h>   //ntohs(), struct sockaddr_in
#include <arpa/inet.h>    //inet_ntop()
#include <unistd.h>       //close()
#include <string.h>
#include <unordered_map>

using namespace std;

#define MAX_SIZE 1024
typedef sockaddr SA;

struct task_queue
{
	int sockfd;
	char buffer[MAX_SIZE];
};

void string_echo(int connfd);

void accept_connection(int epollfd, int listenfd);

void do_recv(int epollfd, int connfd, unordered_map<int, struct task_queue>* taskptr);

void do_send(int epollfd, int connfd, unordered_map<int, struct task_queue>* taskptr);

#endif
