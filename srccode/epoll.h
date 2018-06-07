#ifndef EPOLL_H
#define EPOLL_H

#include "init.h"
#include "thread.h"
#include "request_and_response.h"

void do_epoll(int listenfd, struct thread_pool_info* pool); //创建epoll描述符，并 

void handle_event(int epollfd, struct epoll_event* events, int num, int listenfd, struct thread_pool_info* pool);  //任务分发函数

int epoll_add(int epollfd, int fd, int state);              //向epoll中增加事件

int epoll_del(int epollfd, int fd, int state);              //向epoll中删除事件

int epoll_mod(int epollfd, int fd, int state);              //修改epoll中的事件

#endif
