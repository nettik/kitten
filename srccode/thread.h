#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>   //pthread_detach(), pthread_create()
#include "response.h"  //string_echo()
#include <malloc.h>    //free()
#include <unordered_map>

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
