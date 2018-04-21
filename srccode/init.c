#include "init.h"

int socket_bind_listen()
{
	int listenfd;
	struct sockaddr_in seraddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	int optval = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(optval));

	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_addr.s_addr = htonl(IPADDR);
	seraddr.sin_port = htons(PORT);
	seraddr.sin_family = AF_INET;

	bind(listenfd, (SA*)&seraddr, sizeof(seraddr));
	
	listen(listenfd, LIS_QUEUE_SIZE);

	return listenfd;
}

void make_socket_nonblock(int sockfd)
{
	int flag = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);
}
