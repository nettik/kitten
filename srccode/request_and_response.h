#ifndef REQUEST_AND_RESPONSE_H
#define REQUEST_AND_RESPONSE_H
 
#include "init.h"
#include "epoll.h"
#include "parse_http_request.h"
#include "status_code.h"

int accept_connection(int epollfd, int listenfd);                                         //接受连接

void do_request(struct task_para* arg);                                                   //处理请求

char* receive_request_from_client(int connfd, int epollfd);                               //从套接字中接受请求报文

void serve_static_text_file(char* path, int connfd, struct http_request_info* keyinfo);   //发送静态文件

int get_header_field_from_buffer(int start, char* buf, char* tempbuf);                    //从缓冲区中获取首部字段

bool is_request_legal(int connfd, struct http_request_info* keyinfo);                     //判断请求是否合法

#endif
