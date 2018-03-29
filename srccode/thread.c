#include "thread.h"

void* thread_work(void* arg)
{
	int connfd = ((struct thread_parameter*)arg)->sockfd;
	int epollfd = ((struct thread_parameter*)arg)->epollfd;
	//free(arg);
	pthread_detach(pthread_self());
	//string_echo(connfd);
	do_recv(epollfd, connfd);
	return NULL;
}
