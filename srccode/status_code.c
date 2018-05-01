#include "status_code.h"

void header_200_OK(int connfd, struct http_request_info* keyinfo)
{
	char buf[MAX_SIZE];
	memset(buf, 0, sizeof(buf));
	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Server: Kitten\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: %s\r\n", keyinfo->file_type);
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
}

void header_404_Not_Found(int connfd)
{
	char buf[MAX_SIZE];
	memset(buf, 0, sizeof(buf));
	strcpy(buf, "HTTP/1.0 404 Not Found\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Server: Kitten\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
	
	strcpy(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "<BODY><P>The requested URL was not found on this server.\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(connfd, buf, strlen(buf), 0);
}
void header_400_Bad_Request(int connfd)
{
	char buf[MAX_SIZE];
	memset(buf, 0, sizeof(buf));
	strcpy(buf, "HTTP/1.0 400 Bad Request\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Server: Kitten\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
	
	strcpy(buf, "<HTML><TITLE>Bad Request</TITLE>\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "<BODY><P>There are some syntax mistakes in request datagram.\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(connfd, buf, strlen(buf), 0);
}

void header_501_Not_Implemented(int connfd)
{
	char buf[MAX_SIZE];
	memset(buf, 0, sizeof(buf));
	strcpy(buf, "HTTP/1.0 501 Not Implemented\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Server: Kitten\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);

	strcpy(buf, "<HTML><TITLE>Not Implemented</TITLE>\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "<BODY><P>The method is not implemented in this server.\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(connfd, buf, strlen(buf), 0);
}
