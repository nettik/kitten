#include "request_and_response.h"

void do_request(struct task_para* arg)
{
	int connfd = arg->connfd;
	int epollfd = arg->epollfd;
	char* buffer = receive_request_from_client(connfd, epollfd);
	
	struct http_request_info keyinfo;
	
	if (buffer != NULL)
	{
		int start = parse_request_line(buffer, &keyinfo);

		printf("method : %s\n", keyinfo.method);
		printf("url : %s\n", keyinfo.url);
		printf("path : %s\n", keyinfo.path);
		printf("version : %s\n", keyinfo.version);

		server_static_file(keyinfo.path, connfd, epollfd);
	}

	/*if (buffer != NULL)
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

		send(connfd, buffer, strlen(buffer), 0);

		free(buffer);
	}*/
}

void server_static_file(char* path, int connfd, int epollfd)
{
	FILE* resource = fopen(path, "r");
	if (resource == NULL)
		perror("openfile");
	else
	{
		header_200OK(connfd);
		char buf[MAX_SIZE];
		memset(buf, 0, sizeof(buf));
		//fgets(buf, sizeof(buf), resource);
		while (!feof(resource))
		{
			//send(connfd, buf, sizeof(buf), 0);
			fgets(buf, sizeof(buf), resource);
			send(connfd, buf, strlen(buf), 0);
		}
	}
	fclose(resource);
	//epoll_del(epollfd, connfd, EPOLLIN | EPOLLET);
	//shutdown(connfd, SHUT_WR);
}


char* receive_request_from_client(int connfd, int epollfd)
{
	char* buffer = (char*)malloc(sizeof(char) * BUF_SIZE);
	ssize_t n;
	int buf_length = 1;
	bool clientclosed = false;
	int sum = 0;
	for (;;)
	{
		char tempbuffer[MAX_SIZE];
		n = recv(connfd, tempbuffer, sizeof(tempbuffer), 0);
		if (n == 0)
		{
			clientclosed = true;
			break;
		}
		else if (n < 0)
		{
			if (errno == EINTR)
				continue;
			else if (errno == EAGAIN)
				break;
			else
			{
				clientclosed = true;
				break;
			}
		}
		else if (n > 0)
		{
			while ((sum + n) >= buf_length * BUF_SIZE)
			{
				buffer = (char*)realloc(buffer, buf_length * 2 * BUF_SIZE * sizeof(char));
				buf_length *= 2;
			}
			int i = 0;
			while (i < n && tempbuffer[i] != '\0')
				buffer[sum++] = tempbuffer[i++];
		}
	}
	if (clientclosed == true)
	{
		if (epoll_del(epollfd, connfd, EPOLLIN | EPOLLET) < 0)
			perror("epoll_del_connfd");
		shutdown(connfd, SHUT_WR);
	}
	if (sum == 0)
	{
		free(buffer);
		return NULL;
	}
	return buffer;
}
/*int get_oneline(int connfd, char* buf, int buffsize, int& clienton)
{
	int len = 0;
	char temp = '\0';
	ssize_t n;
	while ((len < buffsize - 1) && (temp != '\n'))
	{
		n = recv(connfd, &temp, sizeof(temp), 0);
		if (n > 0)
		{
			if (temp == '\r')
			{
				n = recv(connfd, &temp, sizeof(temp), MSG_PEEK);
				if (n > 0 && temp == '\n')
					recv(connfd, &temp, sizeof(temp), 0);
				else
					temp = '\n';
			}
			buf[len++] = temp;
		}
		//else if (n == 0)
		//{
		//	clienton = 0;
		//	temp = '\n';
		//}
		else
			temp = '\n';
	}
	buf[len] = '\0';
	return len;
}*/

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

void header_200OK(int connfd)
{
	char buf[MAX_SIZE];
	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Server: Kitten\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
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
