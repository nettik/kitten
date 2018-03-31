#ifndef EPOLL_H
#define EPOLL_H

#include "init.h"
#include "thread.h"

using namespace std;


void do_epoll(int listenfd, unordered_map<int, struct task_queue>* task);

void handle_event(int epollfd, struct epoll_event* events, int num, int listenfd, unordered_map<int, struct task_queue>* task);

void epoll_add(int epollfd, int fd, int state);

void epoll_del(int epollfd, int fd, int state);

void epoll_mod(int epollfd, int fd, int state);

#endif
