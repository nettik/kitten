#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
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

static void* pthread_work(void* arg)
{
	int connfd = *((int*)arg);
	pthread_detach(pthread_self());
	string_echo(connfd);
	return NULL;
}

void string_echo(int connfd)
{
	char buffer[MAX_SIZE];
	ssize_t n;
	for (;;)
	{
		n = recv(connfd, buffer, sizeof(buffer), 0);
		buffer[n] = '\0';
		if (n == 0)
		{
			close(connfd);
			break;
		}
		
		struct sockaddr_in cliaddr;
		socklen_t clilen = sizeof(cliaddr);

		getpeername(connfd, (SA*)&cliaddr, &clilen);
		
		char straddr[20];
		inet_ntop(AF_INET, &cliaddr.sin_addr, straddr, sizeof(straddr));
		int port = ntohs(cliaddr.sin_port);

		printf("receive from %s : %d : %s\n",straddr, port, buffer);
		send(connfd, buffer, n, 0);
	}
}
