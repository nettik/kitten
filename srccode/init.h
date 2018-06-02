#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>   //struct sockaddr_in, htonl(), htons()
#include <string.h>       //bzero(), strncpy()
#include <sys/socket.h>   //socket(), bind(), listen()
#include <pthread.h>      //pthread_create(), pthread_join()
#include <stdio.h>        //printf()
#include <arpa/inet.h>    //inet_ntop()
#include <unistd.h>       //close()
#include <string.h>
#include <sys/epoll.h>
#include <malloc.h>       //free(), malloc()
#include <fcntl.h>        //fcntl()
#include <errno.h>
#include <stdio.h>        //perror(), sprintf()
#include <string>         //string
#include <signal.h>

#define PORT 4000                                  //默认端口号
#define IPADDR INADDR_ANY                          //绑定到监听套接字上默认的IP地址
#define LIS_QUEUE_SIZE 50                          //listen()函数的第二个参数，表示监听队列中的连接数目
#define EPOLL_LIS_SIZE 1000                        //epoll_create()的参数，epoll可监听的描述符的最大数量
#define EPOLLEVENTS 200                            //epoll_event结构体数组的大小
#define MAX_SIZE 1024                              //缓冲区大小
#define BUF_SIZE 512							   //缓冲区大小
#define THREAD_NUM 4                               //线程池中，线程数目   
#define DEFAULT_PATH "/home/meow/kitten/webpage"   //默认路径

typedef sockaddr SA;

int socket_bind_listen();               //创建监听套接字

int make_socket_nonblock(int sockfd);   //将套接字设置为非阻塞

void handle_sigpipe();                  //处理sigpipe信号

#endif
