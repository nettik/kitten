#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "response.h"
#define MAX_SIZE 1024

typedef sockaddr SA;

void string_echo(int connfd);
static void* pthread_work(void* arg);

int main()
{
	int listenfd;
	int* connfd;
	struct sockaddr_in cliaddr, seraddr;
	pthread_t tid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(4000);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenfd, (SA*)&seraddr, sizeof(seraddr));

	listen(listenfd, 5);
	
	bzero(&cliaddr, sizeof(cliaddr));

	socklen_t clilen = sizeof(cliaddr);
	
	for (;;)
	{
		connfd = (int*)malloc(sizeof(int));
		*connfd = accept(listenfd, (SA*)&cliaddr, &clilen);
		pthread_create(&tid, NULL, &pthread_work, (void*)connfd);
	}
	
	close(listenfd);
}

void* pthread_work(void* arg)
{
	int connfd = *((int*)arg);
	pthread_detach(pthread_self());
	string_echo(connfd);
	return NULL;
}

