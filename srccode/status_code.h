#ifndef STATUS_CODE_H
#define STATUS_CODE_H

#include "init.h"
#include "parse_http_request.h"

void header_200_OK(int connfd, struct http_request_info* keyinfo);  //返回状态码200

void header_404_Not_Found(int connfd);                              //返回状态码404

void header_400_Bad_Request(int connfd);                            //返回状态码400

void header_501_Not_Implemented(int connfd);                        //返回状态码501

#endif
