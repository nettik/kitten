#ifndef STATUS_CODE_H
#define STATUS_CODE_H

#include "init.h"
#include "parse_http_request.h"

void header_200_OK(int connfd, struct http_request_info* keyinfo);

void header_404_Not_Found(int connfd);

void header_400_Bad_Request(int connfd);

void header_501_Not_Implemented(int connfd);
#endif
