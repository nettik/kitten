#include <stdlib.h>   //malloc()
#include "response.h"
#include "init.h"
#include "thread.h"
#define MAX_SIZE 1024

typedef sockaddr SA;

void* pthread_work(void* arg);

int main()
{
	int listenfd;
	int* connfd;
	struct sockaddr_in cliaddr;
	pthread_t tid;

	listenfd = socket_bind_listen();
	
	bzero(&cliaddr, sizeof(cliaddr));
	socklen_t clilen = sizeof(cliaddr);
	
	for (;;)
	{
		connfd = (int*)malloc(sizeof(int));
		*connfd = accept(listenfd, (SA*)&cliaddr, &clilen);
		pthread_create(&tid, NULL, &thread_work, (void*)connfd);
	}
	
	close(listenfd);
}
