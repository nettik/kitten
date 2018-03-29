#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>   //pthread_detach(), pthread_create()
#include "response.h"  //string_echo()
#include <malloc.h>    //free()

struct thread_parameter
{
	int epollfd;
	int sockfd;
};


void* thread_work(void* arg);



#endif
