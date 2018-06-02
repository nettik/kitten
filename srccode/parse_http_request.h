#ifndef PARSE_HTTP_REQUEST_H
#define PARSE_HTTP_REQUEST_H

#include "init.h"
#include "request_and_response.h"

struct mime_type
{
	const char* type;
	const char* value;
};


struct http_request_info
{
	char method[10];

	char path[MAX_SIZE];
	char GET_para[MAX_SIZE];
	//bool excute_cgi;

	char version[10];

	char connect_status[20];
	char file_type[20];


	http_request_info()
	{
		memset(method, 0, sizeof(method));
		memset(path, 0, sizeof(path));
		memset(GET_para, 0, sizeof(GET_para));
		memset(version, 0, sizeof(version));
		memset(connect_status, 0, sizeof(connect_status));
		memset(file_type, 0, sizeof(file_type));
		if (sprintf(path, "%s", DEFAULT_PATH) < 0)
			perror("sprintf_path");
	}
};

void set_file_type(char* path, struct http_request_info* keyinfo);                     //设置文件类型

void parse_url(char* url, struct http_request_info* keyinfo);                          //解析url

int parse_request_line(char* buffer, struct http_request_info* keyinfo);               //解析请求行

void parse_header_field(char* buffer, int start, struct http_request_info* keyinfo);   //解析首部字段

//void destroy_http_request_info(struct http_request_info* keyinfo);

#endif
