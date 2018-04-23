#include "request_and_response.h"

void do_request(struct task_para* arg)
{
	int connfd = arg->connfd;
	int epollfd = arg->epollfd;
	char buffer[MAX_SIZE];
	ssize_t n;
	ssize_t sum = 0;
	bool clientclosed = false;
	for (;;)
	{
		char tempbuffer[MAX_SIZE];
		n = recv(connfd, tempbuffer, sizeof(tempbuffer), 0);
		if (n > 0)
		{
			for (int i = 0; i < n; ++i)
				buffer[sum++] = tempbuffer[i];		
		}
		else if (n < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else if (errno == EINTR)
				continue;
			else
			{
				clientclosed = true;
				break;
			}
		}
		else if (n == 0)
		{
			clientclosed = true;
			break;
		}
	}
	if (clientclosed == true)
	{
		if (epoll_del(epollfd, connfd, EPOLLIN | EPOLLET) < 0)
			perror("epoll_del_connfd");
		shutdown(connfd, SHUT_WR);
		return;
	}
	buffer[sum] = '\0';
	/*n = recv(connfd, buffer, sizeof(buffer), 0);
	if (n <= 0)
	{
		close(connfd);
		epoll_del(epollfd, connfd, EPOLLIN | EPOLLET);
	}*/
	if (sum > 0)
	{
		struct sockaddr_in cliaddr;
		socklen_t clilen = sizeof(cliaddr);
		if (getpeername(connfd, (SA*)&cliaddr, &clilen) < 0)
			perror("getpeername");

		char IPaddr[20];
		if (inet_ntop(AF_INET, &cliaddr.sin_addr, IPaddr, sizeof(IPaddr)) == NULL)
			perror("inet_ntop");
		int port = ntohs(cliaddr.sin_port);

		printf("thread %d receive from %s : %d : %s\n", (int)pthread_self(), IPaddr, port, buffer);

		send(connfd, buffer, sizeof(buffer), 0);
	}
}

int accept_connection(int epollfd, int listenfd)
{
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);

	int connfd;
	if ((connfd = accept(listenfd, (SA*)&cliaddr, &clilen)) < 0)
	{
		perror("accept");
		return -1;
	}

	if (make_socket_nonblock(connfd) < 0)
	{
		perror("make_socket_nonblock_connfd");
		return -1;
	}

	if (epoll_add(epollfd, connfd, EPOLLIN | EPOLLET) < 0)
	{
		perror("epoll_ctl_add_connfd");
		return -1;
	}
	return 0;
}

/*void string_echo(int connfd)
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
}*/

/*void do_recv(int epollfd, int connfd, unordered_map<int, struct task_queue>* task)
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
		buffer[n] = '\0';

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
		
		pthread_mutex_lock(&lock_task);

		(*task)[connfd] = tq;

		pthread_mutex_unlock(&lock_task);

		epoll_mod(epollfd, connfd, EPOLLOUT | EPOLLET);
	}
}


void do_send(int epollfd, int connfd, unordered_map<int, struct task_queue>* task)
{
	char buffer[MAX_SIZE];

	pthread_mutex_lock(&lock_task);

	strcpy(buffer, ((*task)[connfd]).buffer);
	(*task).erase(connfd);

	pthread_mutex_unlock(&lock_task);

	send(connfd, buffer, sizeof(buffer), 0);
	
	epoll_mod(epollfd, connfd, EPOLLIN | EPOLLET);
}*/
