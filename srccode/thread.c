#include "thread.h"

void* thread_work_recv(void* arg)
{
	int connfd = ((struct thread_parameter*)arg)->sockfd;
	int epollfd = ((struct thread_parameter*)arg)->epollfd;
	unordered_map<int, struct task_queue>* taskptr = ((struct thread_parameter*)arg)->taskptr;

	delete((struct thread_parameter*)arg);

	pthread_detach(pthread_self());
	//string_echo(connfd);
	do_recv(epollfd, connfd, taskptr);
	return NULL;
}

void* thread_work_send(void* arg)
{	
	int connfd = ((struct thread_parameter*)arg)->sockfd;
	int epollfd = ((struct thread_parameter*)arg)->epollfd;
	unordered_map<int, struct task_queue>* taskptr = ((struct thread_parameter*)arg)->taskptr;

	delete((struct thread_parameter*)arg);

	pthread_detach(pthread_self());

	do_send(epollfd, connfd, taskptr);
	return NULL;
}
