#include "parse_http_request.h"

struct mime_type MIME[]=
{
	{".html", "text/html"},
	{".pdf", "application/pdf"},
	{".png", "image/png"},
	{".jpg", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".ico", "image/x-icon"},
	{NULL, "text/plain"}
};

void set_file_type(char* path, struct http_request_info* keyinfo)
{
	int dot_index = -1;
	for (unsigned int i = 0; i < strlen(path) + 1; ++i)
		if (path[i] == '.')
		{
			dot_index = i;
			break;
		}
	int i = 0;
	while ((unsigned)i < sizeof(MIME)/sizeof(struct mime_type) && MIME[i].type != NULL)
	{
		if (strcmp(path + dot_index, MIME[i].type) == 0)
		{
			strcpy(keyinfo->file_type, MIME[i].value);
			break;
		}
		++i;
	}
	if (MIME[i].type == NULL)
		strcpy(keyinfo->file_type, "text/plain");
}

void parse_url(char* url, struct http_request_info* keyinfo)
{
	int j = 0;
	while ((unsigned)j < strlen(url) + 1 && url[j] != '?')
		++j;

	strncat(keyinfo->path, url, j);
	if ((unsigned)j != strlen(url) + 1)
		strcpy(keyinfo->GET_para, url + j + 1);
	if (keyinfo->path[strlen(keyinfo->path) - 1] == '/')
		strcat(keyinfo->path, "index.html");
}
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

	parse_url(url, keyinfo);

	set_file_type(keyinfo->path, keyinfo);

	free(url);
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

void parse_header_field(char* buffer, int start, struct http_request_info* keyinfo)
{
	//获得Connection字段
	while ((unsigned)start < strlen(buffer) + 1)
	{
		char tempbuffer[MAX_SIZE];
		memset(tempbuffer, 0, sizeof(tempbuffer));
		start = get_header_field_from_buffer(start, buffer, tempbuffer);
		if (strcasecmp(tempbuffer, "Connection: keep-alive") == 0 || strcasecmp(tempbuffer, "Connection: close") == 0)
		{
			int i = 0;
			while ((unsigned)i < strlen(tempbuffer) + 1 && tempbuffer[i] != ':')
				++i;
			i = i + 2;
			strcpy(keyinfo->connect_status, tempbuffer + i);
			break;
		}
	}

	if (strlen(keyinfo->connect_status) == 0)
		strcpy(keyinfo->connect_status, "close");
}
