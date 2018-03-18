### IPv4套接字地址结构  
```
#include <netinet/in.h>  
struct in_addr  
{  
	in_addr_t       s_addr;      32位IPv4地址，网络字节序  
};  
struct sockaddr_in  
{  
	uint8_t         sin_len;     结构体长度  
	sa_family_t     sin_family;  AF_INET  
	in_port_t       sin_port;    16位TCP或UDP端口号，网络字节序  
	struct in_addr  sin_addr;    32位IPv4地址，网络字节序  
	char            sin_zero[8];  
};
```


### 网络字节序和主机字节序转换函数   
```
#include <netinet/in.h>    
uint16_t htons(uint16_t host16bitvalue);  
uint32_t htonl(uint32_t host32bitvalue);     
```
均返回网络字节序值
```   
uint16_t ntohs(uint16_t net16bitvalue);   
uint32_t ntohl(uint32_t net32bitvalue);      
```
均返回主机字节序值


### 字节操纵函数  
```
#include <string.h>   
void bzero(void* dest, size_t nbytes);   
```
bzero把目标字节串中指定数目的字节置为0，经常使用该函数来把一个套接字地址结构初始化为0
```
#include <string.h>  
void* memset(void* dest, int c, size_t len);   
void* memcpy(void* dest, const void* src, size_t nbytes);
```

### 地址转换函数   
```
#include <arpa/inet.h>   
int inet_aton(const char* strptr, struct in_addr* addrptr);    
```
返回：若字符串有效则为1，否则为0    
将strptr所指C字符串转换成一个32位的网络字节序二进制值，并通过指针addrptr来存储
```
in_addr_t inet_addr(const char* strptr);  
```
返回：若字符串有效则为32位二进制网络字节序的IPv4地址，否则为INADDR_NONE
```
char* inet_ntoa(struct in_addr inaddr);  
```
返回：指向一个点分十进制数串的指针   
将一个32位的网络字节序二进制IPv4地址转换成相应的点分十进制数串
```
int inet_pton(int family, const char* strptr, void* addrptr);    
```
返回：若成功返回1，若输入不是有效的表达式格式则为0，若出错则为-1    
转化由strptr指针所指的字符串，并通过addrptr指针存放二进制结果
```
const char* inet_ntop(int family, const void* addrptr, char* strptr, size_t len);    
```
返回：若成功则为指向结果的指针，若出错则为NULL    
从数值格式（addrptr）转换到表达式格式（strptr），len参数是目标存储单元大小


### TCP套接字函数
```
#include <sys/socket.h>
int socket(int family, int type, int protocol);
```
返回：若成功则为非负描述符，若出错则为1   
```
#include <sys/socket.h>
int connect(int fd, const struct sockaddr* servaddr, socklen_t addrlen);
```
返回：若成功则为0，若出错则为-1   
第二个参数，套接字地址结构必须含有服务器的IP地址和端口号   
如果是TCP套接字，调用connect函数将激发TCP三次握手过程，而且仅在连接建立成功或出错时才返回
```
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* myaddr, socklen_t addrlen);
```
返回：若成功则为0，若出错则为-1   
bind函数把本地协议赋予一个套接字
```
#include <sys/socket.h>
int listen(int sockfd, int backlog)
```
返回：若成功则为0，若出错则为-1   
第二个参数规定了内核应该为相应套接字排队的最大连接数（已完成连接队列和未完成连接队列之和）
```
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr* cliaddr, socklen_t* addrlen);
```
返回：若成功则为非负描述符，若出错则为-1   
参数cliaddr和addrlen返回已连接的对端进程（客户）的协议地址   
如果accept成功，那么其返回值是由内核自动生成的一个全新描述符，称其为已连接套接字描述符   


### 基本线程函数
```
#include <pthread.h>
int pthread_create(pthread_t* tid, const pthread_attr_t* attr, void* (*func)(void*), void* arg);
```
返回：若成功则为0，若出错则为正的Exxx值    
一个进程内的每个线程都由一个线程ID（thread ID）标识，其数据类型为pthread_t。如果新的线程成功创建，其ID就通过tid指针返回     
通常情况下，把attr参数指定为空指针     
创建一个线程时，最后指定的参数是由该线程执行的函数及其参数，该线程通过调用这个函数开始执行。该函数的地址由func参数指定，该函数的唯一调用参数是指针arg
```
#include <pthread.h>
int pthread_join(pthread_t* tid, void** status);
```
返回：若成功则为0，若出错则为正的Exxx值  
通过调用pthread_join等待一个给定线程终止  
如果status指针非空，来自所等待线程的返回值（一个指向某个对象的指针）将存入status指向的位置
```
#include <pthread.h>
pthread_t pthread_self(void);
```
返回：调用线程的线程ID  
每个线程使用pthread_self获取自身的线程ID
```
#include <pthread.h>
int pthread_detach(pthread_t tid);
```
pthread_detach函数将把指定的线程转变为脱离状态
```
#include <pthread.h>
void pthread_exit(void* status);
```
线程终止函数

