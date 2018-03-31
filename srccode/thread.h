#ifndef THREAD_H
#define THREAD_H

#include "init.h"
#include "response.h"

using namespace std;

struct thread_parameter
{
	int epollfd;
	int sockfd;
	unordered_map<int, struct task_queue>* taskptr;
};


void* thread_work_recv(void* arg);

void* thread_work_send(void* arg);

#endif
