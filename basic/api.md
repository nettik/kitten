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

```
#include <unistd.h>
int close(int sockfd);
```
返回：若成功则为0，若出错则为-1
close一个TCP套接字的默认行为是把该套接字标记成已关闭
```
#include <sys/socket.h>
int shutdown(int sockfd, int howto);
```
close函数有两个限制，可以使用shutdown函数来避免  
1）close把描述符的引用计数减1，仅在该计数变为0时才关闭套接字；使用shutdown函数可以不管引用计数就激发TCP的正常连接终止序列  
2）close终止读和写两个方向的数据传送

shutdown函数的行为依赖于howto参数的值  
1）SHUT_RD：关闭连接的读这一半，套接字中不再有数据可接收，而且套接字接收缓冲区中的现有数据都被丢弃，进程不能再对这个的套接字调用任何读函数，对一个TCP套接字这样调用shutdown函数后，由该套接字接收的来自对端的任何数据都被确认，然后悄然丢弃  
2）SHUT_WR：关闭连接的写这一半，对于TCP套接字，这称为半关闭，当前留在套接字发送缓冲区中的数据将被发送掉，后跟TCP的正常连接终止序列，进程不能再对这样的套接字调用任何写函数  
3) SHUT_RDWR：连接的读半部和写半部都关闭


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


### 互斥锁
```
#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t* mptr);
int pthread_mutex_unlock(pthread_mutex_t* mptr);
```
返回：若成功则为0，若出错则为正的Exxx值  
可以通过互斥锁保护共享变量；访问该变量的前提条件是持有该互斥锁  
如果试图上锁已被另外某个线程锁住的一个互斥锁，本线程将被阻塞，直到该互斥锁被解锁为止  
如果某个互斥锁变量是静态分配的，就必须把它初始化为常值PTHREAD_MUTEX_INITIALIZER  


### 条件变量
互斥锁提供互斥机制，条件变量提供信号机制
```
#include <pthread.h>
int pthread_cond_wait(pthread_cond_t* cptr, pthread_mutex_t* mptr);
int pthread_cond_signal(pthread_cond_t* cptr);
```
返回：若成功则为0，若出错则为正的Exxx值  
pthread_cond_wait函数把调用线程投入睡眠并释放调用线程持有的互斥锁；当调用线程从pthread_cond_wait返回时（其他某个线程发送信号），该线程再次持有该互斥锁


### IO函数
#### read、write
```
#include <unistd.h>
ssize_t read(int fd, void* buf, size_t nbytes)
```
返回值：若成功则返回实际读到的字节数，若已到文件尾则返回0，出错返回-1  
从fd中读取nbytes字节的数据到buf中  
ssize_t提供带符号的返回值，size_t不带符号
```
#include <unistd.h>
ssize_t write(int fd, const void* buf, size_t nbytes);
```
返回值：若成功则返回写出的字节数，若失败返回-1
#### recv、send
```
#include <sys/socket.h>
ssize_t recv(int sockfd, void* buff, size_t nbytes, int flags);
ssize_t send(int sockfd, const void* buff, size_t nbytes, int flags);
```
返回：若成功则为读入或写出的字节数，若出错则为-1  
flags参数的值或为0或为一个或多个常值的逻辑或

flags参数取值  
MSG_DONTROUTE：绕过路由表查找  
MSG_DONTWAIT：仅本操作非阻塞  
MSG_OOB：发送或接收带外数据  
MSG_PEEK：窥看外来消息  
MSG_WAITALL：等待所有数据  

### fcntl函数
```
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* int arg */);
```
返回：若成功则取决于cmd，若出错则为-1

设置某个文件标志的唯一正确的方法是：先取得当前标志，与新标志逻辑或后再设置标志
```
int flags;
if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
	err_sys("FGETFL error");
flags |= O_NONBLOCK;
if (fcntl(fd, F_SETFL, flags) < 0)
	err_sys("F_SETFL error");
```
以下代码关闭非阻塞标志,flags是由上面代码所示的fcntl调用设置来的
```
flags &= ~O_NONBLOCK;
if (fcntl(fd, F_SETFL, flags) < 0)
	err_sys("E_SETFL error");
```

### getsockname和getpeername函数
```
#include <sys/socket.h>
int getsockname(int sockfd, struct sockaddr* localaddr, socklen_t* addrlen);
int getpeername(int sockfd, struct sockaddr* peeraddr, socklen_t addrlen);
```
返回：若成功则为0，若出错则为-1   
这两个函数的最后一个参数都是值-结果参数。这就是说，这两个函数都得装填由localaddr或peeraddr指针所指的套接字地址结构  

需要这两个函数的理由如下：  
1、在一个没有调用bind的TCP客户上，connect成功返回后，getsockname用于返回由内核赋予该连接的本地IP地址和本地端口号  
2、在以端口号0调用bind(告知内核去选择本地端口号)后，getsockname用于返回由内核赋予的本地端口号    
3、在一个以通配IP地址调用bind的TCP服务器上，与某个客户的连接一旦建立(accept成功返回)，getsockname就可以用于返回由内核赋予该连接的本地IP地址。在这样的调用中，套接字描述符参数必须是已连接套接字的描述符，而不是监听套接字的描述符  
4、当一个服务器是由调用过accept的某个进程通过调用exec执行程序时，它能够获取客户身份的唯一途径便是调用getpeername  
