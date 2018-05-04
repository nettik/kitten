#include "init.h"

int socket_bind_listen()
{
	int listenfd;
	struct sockaddr_in seraddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}

	int optval = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt");
		return -1;
	}

	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_addr.s_addr = htonl(IPADDR);
	seraddr.sin_port = htons(PORT);
	seraddr.sin_family = AF_INET;

	if ((bind(listenfd, (SA*)&seraddr, sizeof(seraddr))) < 0)
	{
		perror("bind");
		return -1;
	}
	
	if (listen(listenfd, LIS_QUEUE_SIZE) < 0)
	{
		perror("listen");
		return -1;
	}

	return listenfd;
}

int make_socket_nonblock(int sockfd)
{
	int flag;
	if ((flag = fcntl(sockfd, F_GETFL, 0)) < 0)
	{
		perror("F_GETFL");
		return -1;
	}
	flag |= O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, flag) < 0)
	{
		perror("F_SETFL");
		return -1;
	}
	return 0;
}

void handle_sigpipe()
{
	struct sigaction action;
	action.sa_handler = SIG_IGN;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGPIPE, &action, NULL);
}
