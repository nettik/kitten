#ifndef EPOLL_H
#define EPOLL_H

#include "init.h"
#include "thread.h"
#include "request_and_response.h"

void do_epoll(int listenfd, struct thread_pool_info* pool);                                                         

void handle_event(int epollfd, struct epoll_event* events, int num, int listenfd, struct thread_pool_info* pool);

int epoll_add(int epollfd, int fd, int state);

int epoll_del(int epollfd, int fd, int state);

int epoll_mod(int epollfd, int fd, int state);

#endif
