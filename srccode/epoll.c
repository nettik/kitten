#include "epoll.h"

int epoll_add(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
		return -1;
	return 0;
}

int epoll_del(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev) < 0)
		return -1;
	return 0;
}

int epoll_mod(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) < 0)
		return -1;
	return 0;
}

void do_epoll(int listenfd, struct thread_pool_info* pool)
{
	int epollfd, nums;
	struct epoll_event events[EPOLLEVENTS];
	if ((epollfd = epoll_create(EPOLL_LIS_SIZE)) < 0)
		perror("epoll_create");
	if (epoll_add(epollfd, listenfd, EPOLLIN | EPOLLET) < 0)
		perror("epoll_cnt_add_listenfd");

	for (;;)
	{
		if ((nums = epoll_wait(epollfd, events, EPOLLEVENTS, -1)) < 0)
		{
			perror("epoll_wait");
			if (errno == EINTR)
				continue;
		}
		handle_event(epollfd, events, nums, listenfd, pool);
	}
	close(epollfd);
}

void handle_event(int epollfd, struct epoll_event* events, int nums, int listenfd, struct thread_pool_info* pool)
{
	int fd;
	for (int i = 0; i < nums; i++)
	{
		fd = events[i].data.fd;
		if ((fd == listenfd) && (events[i].events & EPOLLIN))
		{
			if (accept_connection(epollfd, listenfd) < 0)
				perror("accept_connection");
		}
		else if (events[i].events & EPOLLIN)
		{
			struct task_para* p = (struct task_para*)malloc(sizeof(task_para));
			p->epollfd = epollfd;
			p->connfd = fd;
			
			thread_pool_add_task(pool, p, &do_request);
		}
	}
}
