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
	for (int i = 0; i < strlen(path) + 1; ++i)
		if (path[i] == '.')
		{
			dot_index = i;
			break;
		}
	/*char temp_filetype[20];
	memset(temp_filetype, 0, sizeof(temp_filetype));
	strcpy(temp_filetype, path + dot_index);*/
	int i = 0;
	while (i < sizeof(MIME)/sizeof(struct mime_type) && MIME[i].type != NULL)
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
	while (j < strlen(url) + 1 && url[j] != '?')
		++j;

	strncat(keyinfo->path, url, j);
	if (j != strlen(url) + 1)
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
	//keyinfo->url = url;	

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

int parse_header_field(char* buffer, int start, struct http_request_info* keyinfo)
{
	
}
