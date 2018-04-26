#ifndef REQUEST_AND_RESPONSE_H
#define REQUEST_AND_RESPONSE_H
 
#include "init.h"
#include "epoll.h"
#include "parse_http_request.h"

int accept_connection(int epollfd, int listenfd);

void do_request(struct task_para* arg);

char* receive_request_from_client(int connfd, int epollfd);

//int get_oneline(int connfd, char* buff, int buffsize, int& clienton);

/*struct task_queue
{
	int sockfd;
	char buffer[MAX_SIZE];
};*/

//void string_echo(int connfd);

/*void do_recv(int epollfd, int connfd, unordered_map<int, struct task_queue>* taskptr);

void do_send(int epollfd, int connfd, unordered_map<int, struct task_queue>* taskptr);*/

#endif
