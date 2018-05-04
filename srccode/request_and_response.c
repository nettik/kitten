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
		
		parse_header_field(buffer, start, &keyinfo);
#if 0		
		printf("method : %s\n", keyinfo.method);
		//printf("GET_para : %s\n", keyinfo.GET_para);
		printf("path : %s\n", keyinfo.path);
		printf("file_type : %s\n", keyinfo.file_type);
		//printf("version : %s\n", keyinfo.version);
#endif
		if (is_request_legal(connfd, &keyinfo) == false)
		{	
			if (epoll_del(epollfd, connfd, EPOLLIN | EPOLLET) < 0)
				perror("epoll_del_connfd_in_do_request");
			shutdown(connfd, SHUT_WR);
			close(connfd);
			return;
		}
		
		serve_static_text_file(keyinfo.path, connfd, &keyinfo);
		
		if (strcasecmp("close", keyinfo.connect_status) == 0)
		{
			if (epoll_del(epollfd, connfd, EPOLLIN | EPOLLET) < 0)
				perror("epoll_del_connfd_in_serve_static_text_file_resource_close");
			shutdown(connfd, SHUT_WR);
			close(connfd);
		}
#if 0		
		start = 0;
		while ((unsigned)start < strlen(buffer) + 1)
		{
			char tempbuffer[MAX_SIZE];
			memset(tempbuffer, 0, sizeof(tempbuffer));
			int t = get_header_field_from_buffer(start, buffer, tempbuffer);
			printf("%s\n", tempbuffer);
			start = t;
		}
#endif
		free(buffer);
	}
}

void serve_static_text_file(char* path, int connfd, struct http_request_info* keyinfo)
{
	FILE* resource = fopen(path, "r");
	if (resource == NULL)
	{
		header_404_Not_Found(connfd);	
	}
	else
	{
		header_200_OK(connfd, keyinfo);
		
		char buf[MAX_SIZE];
		memset(buf, 0, sizeof(buf));
		
		while (!feof(resource))
		{
			fgets(buf, sizeof(buf), resource);
			send(connfd, buf, strlen(buf), 0);
		}
		fclose(resource);	
	}
}


char* receive_request_from_client(int connfd, int epollfd)
{
	char* buffer = (char*)malloc(sizeof(char) * BUF_SIZE);
	memset(buffer, 0, sizeof(char) * BUF_SIZE);
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
			perror("epoll_del_connfd_in_receive_request_from_client");
		shutdown(connfd, SHUT_WR);
		close(connfd);
		
		free(buffer);
		return NULL;
	}
	if (sum == 0)
	{
		free(buffer);
		return NULL;
	}
	buffer[sum] = '\0';
	return buffer;
}
int get_header_field_from_buffer(int start, char* buf, char* tempbuf)   //从缓冲区获取首部字段值
{
	int index = start;
	while ((unsigned)start < strlen(buf) + 1 && buf[start] != '\r')
		++start;
	
	strncpy(tempbuf, buf + index, start - index);
	tempbuf[start - index] = '\0';

	start = start + 2;
	return start;
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

bool is_request_legal(int connfd, struct http_request_info* keyinfo)
{
	if (strcasecmp("GET", keyinfo->method) != 0)
	{
		header_501_Not_Implemented(connfd);
		return false;
	}
	/*...*/

	return true;
}
