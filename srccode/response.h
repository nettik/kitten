#ifndef RESPONSE_H
#define RESPONSE_H
 
#include "init.h"
#include "epoll.h"


/*struct task_queue
{
	int sockfd;
	char buffer[MAX_SIZE];
};*/

//void string_echo(int connfd);

void accept_connection(int epollfd, int listenfd);

/*void do_recv(int epollfd, int connfd, unordered_map<int, struct task_queue>* taskptr);

void do_send(int epollfd, int connfd, unordered_map<int, struct task_queue>* taskptr);*/

void do_request(struct task_para* arg);
#endif
