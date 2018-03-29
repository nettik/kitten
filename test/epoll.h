#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#define MAX_SIZE 1024

void do_epoll(int listenfd);

void handle_event(int epollfd, struct epoll_event* events, int num, int listenfd, char* buf);

void epoll_add(int epollfd, int fd, int state);

void epoll_del(int epollfd, int fd, int state);

void epoll_mod(int epollfd, int fd, int state);

#endif
