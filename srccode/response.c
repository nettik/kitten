#include "response.h"
#include "epoll.h"

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

		char IPaddr[20];
		inet_ntop(AF_INET, &cliaddr.sin_addr, IPaddr, sizeof(IPaddr));
		int port = ntohs(cliaddr.sin_port);

		printf("receive from %s : %d : %s\n", IPaddr, port, buffer);
		send(connfd, buffer, n, 0);
	}
}

void accept_connection(int epollfd, int listenfd)
{
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);

	int connfd = accept(listenfd, (SA*)&cliaddr, &clilen);

	epoll_add(epollfd, connfd, EPOLLIN);
}

void do_recv(int epollfd, int connfd, unordered_map<int, struct task_queue>* task)
{
	char buffer[MAX_SIZE];
	ssize_t n;
	n = recv(connfd, buffer, sizeof(buffer), 0);
	if (n == -1 || n == 0)
	{
		close(connfd);
		epoll_del(epollfd, connfd, EPOLLIN);
	}
	else if (n > 0)
	{
		struct sockaddr_in cliaddr;
		socklen_t clilen = sizeof(cliaddr);
		getpeername(connfd, (SA*)&cliaddr, &clilen);

		char IPaddr[20];
		inet_ntop(AF_INET, &cliaddr.sin_addr, IPaddr, sizeof(IPaddr));
		int port = ntohs(cliaddr.sin_port);

		printf("receive from %s : %d : %s\n", IPaddr, port, buffer);

		task_queue tq;
		tq.sockfd = connfd;
		strcpy(tq.buffer, buffer);
		
		(*task)[connfd] = tq;

		epoll_mod(epollfd, connfd, EPOLLOUT);
	}
}


void do_send(int epollfd, int connfd, unordered_map<int, struct task_queue>* task)
{
	char buffer[MAX_SIZE];
	strcpy(buffer, ((*task)[connfd]).buffer);
	
	send(connfd, buffer, strlen(buffer), 0);
	
	(*task).erase(connfd);

	epoll_mod(epollfd, connfd, EPOLLIN);
}
