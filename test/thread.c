#include "thread.h"

void* thread_work(void* arg)
{
	int connfd = *((int*)arg);
	free(arg);
	pthread_detach(pthread_self());
	string_echo(connfd);
	return NULL;
}
