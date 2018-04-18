#include "epoll.h"

void epoll_add(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_del(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void epoll_mod(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void do_epoll(int listenfd, struct thread_pool_info* pool)
{
	int epollfd, nums;
	struct epoll_event events[EPOLLEVENTS];
	epollfd = epoll_create(EPOLL_LIS_SIZE);
	epoll_add(epollfd, listenfd, EPOLLIN | EPOLLET);

	for (;;)
	{
		nums = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
		handle_event(epollfd, events, nums, listenfd, pool);
	}
}

void handle_event(int epollfd, struct epoll_event* events, int nums, int listenfd, struct thread_pool_info* pool)
{
	int fd;
	//pthread_t tid;
	for (int i = 0; i < nums; i++)
	{
		fd = events[i].data.fd;
		if ((fd == listenfd) && (events[i].events & EPOLLIN))
			accept_connection(epollfd, listenfd);
		else if (events[i].events & EPOLLIN)
			thread_pool_add_task(pool, (void*)&fd, &do_request);
		/*else if (events[i].events & EPOLLIN)
		{
			struct thread_parameter* para = new thread_parameter();
			para->epollfd = epollfd;
			para->sockfd = events[i].data.fd;
			para->taskptr = task;

			pthread_t tid;

			pthread_create(&tid, NULL, &thread_work_recv, para);
			
		}
		else if (events[i].events & EPOLLOUT)
		{	
			struct thread_parameter* para = new thread_parameter();
			para->epollfd = epollfd;
			para->sockfd = events[i].data.fd;
			para->taskptr = task;

			pthread_t tid;
			pthread_create(&tid, NULL, &thread_work_send, para);
			//do_send(epollfd, fd, task);
		}*/
	}
}
