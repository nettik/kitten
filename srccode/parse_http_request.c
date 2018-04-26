#include "parse_http_request.h"


int parse_request_line(char* buffer, struct http_request_info* keyinfo)
{
	int i = 0;
	int len = strlen(buffer) + 1;
	//method
	while (i < len && buffer[i] != ' ')
		++i;
	strncpy(keyinfo->method, buffer, i);
	keyinfo->method[i] = '\0';
	
	//url、path
	++i;
	int url_start = i;
	while (i < len && buffer[i] != ' ')
		++i;
	char* url = (char*)malloc(sizeof(char) * (i - url_start + 1));
	strncpy(url, buffer + url_start, i - url_start);
	url[i - url_start] = '\0';
	keyinfo->url = url;	
	
	//version
	++i;
	int ver_start = i;
	while (i < len && buffer[i] != '\r')
		++i;
	strncpy(keyinfo->version, buffer + ver_start, i - ver_start);
	keyinfo->version[i - ver_start] = '\0';

	i = i + 2;
	return i;
}
