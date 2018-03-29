#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "response.h"
#include "thread.h"

#define EPOLL_LIS_SIZE 1000
#define EPOLLEVENTS 200

void do_epoll(int listenfd);

void handle_event(int epollfd, struct epoll_event* events, int num, int listenfd);

void epoll_add(int epollfd, int fd, int state);

void epoll_del(int epollfd, int fd, int state);

void epoll_mod(int epollfd, int fd, int state);

#endif
