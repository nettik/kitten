#ifndef PARSE_HTTP_REQUEST_H
#define PARSE_HTTP_REQUEST_H

#include "init.h"


struct http_request_info
{
	char method[10];
	char version[10];
	char* url;
	char path[MAX_SIZE];
	char connect_status[20];
	http_request_info()
	{
		memset(method, 0, sizeof(method));
		memset(version, 0, sizeof(version));
		memset(connect_status, 0, sizeof(connect_status));
		if (sprintf(path, "%s", DEFAULT_PATH) < 0)
			perror("sprintf");
	}
};


int parse_request_line(char* buffer, struct http_request_info* keyinfo);


#endif
