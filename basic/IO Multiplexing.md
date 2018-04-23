一个输入操作通常包括两个不同的阶段：  
1、等待数据准备好  
2、从内核向进程复制数据  
对于一个套接字上的输入操作，第一步通常涉及等待数据从网络中到达。当所等待分组到达时，它被复制到内核中的某个缓冲区。第二步就是把数据从内核缓冲区复制到应用进程缓冲区。


### 阻塞式I/O模型
默认情况下，所有套接字都是阻塞的   
![image](https://github.com/nettik/kitten/blob/master/basic/picture/1.png)  
进程调用recvfrom，其系统调用直到数据报到达且被复制到应用进程的缓冲区中或者发生错误才返回   
进程在调用recvfrom开始到它返回的整段时间内是被阻塞的  


### 非阻塞式I/O模型
进程把一个套接字设置成非阻塞是在通知内核：当所请求的I/O操作非得把本进程投入睡眠才能完成时，不要把本进程投入睡眠，而是返回一个错误   
![image](https://github.com/nettik/kitten/blob/master/basic/picture/2.png)   
前三次调用recvfrom时没有数据可返回，因此内核转而立即返回一个EWOULDBLOCK错误  
当一个应用进程像这样对一个非阻塞描述符循环调用recvfrom时，我们称之为轮询

对于非阻塞的套接字，如果输入操作不能被满足（对于TCP套接字即至少一个字节的数据可读），相应调用将立即返回一个EWOULDBLOCK错误  
对于非阻塞的TCP套接字，如果其发送缓冲区中根本没有空间，输出函数调用将立即返回一个EWOULDBLOCK错误  

如果对一个阻塞的套接字调用accept函数，并且尚无新的连接到达，调用进程将被投入睡眠  
如果对一个非阻塞的套接字调用accept函数，并且尚无新的连接到达，accept调用将立即返回一个EWOULDBLOCK错误  


### I/O复用模型
有了I/O复用，就可以调用select或poll，阻塞在这两个系统调用中的某一个之上，而不是阻塞在真正的I/O系统调用上   
![image](https://github.com/nettik/kitten/blob/master/basic/picture/3.png)   
进程阻塞于select调用，等待数据报套接字变为可读，当select返回套接字可读这一条件时，我们调用recvfrom把所读数据报复制到应用进程缓冲区  

### 信号驱动式I/O模型
可以用信号，让内核在描述符就绪时发送SIGIO信号通知我们      
![image](https://github.com/nettik/kitten/blob/master/basic/picture/4.png)   
首先开启套接字的信号驱动式I/O功能，并通过sigaction系统调用安装一个信号处理函数，该系统调用将立即返回，进程继续工作   
当数据报准备好读取时，内核就为该进程产生一个SIGIO信号，随后既可以在信号处理函数中调用recvfrom读取数据报，并通知主循环数据已准备好待处理，也可以立即通知主循环，让它读取数据报   
这种模型的优势在于等待数据报到达期间进程不被阻塞


### 异步I/O模型
![image](https://github.com/nettik/kitten/blob/master/basic/picture/5.png)  
信号驱动模式I/O是由内核通知我们何时可以启动一个I/O操作，而异步I/O模型是由内核通知我们I/O操作何时完成   
告知内核启动某个操作，并让内核在整个操作（包括将数据从内核复制到我们自己的缓冲区）完成后通知我们  


### 同步I/O和异步I/O对比
同步I/O操作导致请求进程阻塞，直到I/O操作完成  
异步I/O操作不导致请求进程阻塞  
阻塞式I/O模型、非阻塞式I/O模型、I/O复用模型和信号驱动式I/O模型都是同步I/O模型，因为其中真正的I/O操作（recvfrom）将阻塞进程  
异步I/O模型与POSIX定义的异步I/O相匹配


# select函数
该函数允许进程指示内核等待多个事件中的任何一个发生，并且只在有一个或多个事件发生或经历一段指定的时间后才唤醒
```
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, const struct timeval* timeout);
```
返回：若有就绪描述符则为其数目，若超时则为0，若出错则为-1

timeout参数，告知内核等待所指定描述符中的任何一个就绪可花多长时间  
timeval结构用于指定这段时间的秒数和微秒数
```
struct timeval
{
  long tv_sec;	秒
  long tv_usec;	微秒
}
```
这个参数有三种可能：  
1）永远等待下去：仅在有一个描述符准备好I/O时才返回。为此我们把该参数设置为空指针  
2）等待一段固定时间：在有一个描述符准备好I/O时返回，但是不超过由该参数所指向的timeval结构中指定的秒数和微秒数  
3）根本不等待：检查描述符后立即返回，这称为轮询。为此，该参数必须指向一个timeval结构，而且其中的定时器值（由该结构指定的秒数和微秒数）必须为0

readset、writeset和exceptset指定我们要让内核测试读、写和异常条件的描述符  
select使用描述符集，通常是一个整数数组，其中每个整数中的每一位对应一个描述符  
```
void FD_ZERO(fd_set* fdset);          clear all bits in fdset
void FD_SET(int fd, fd_set* fdset);   turn on the bit for fd in fdset
void FD_CLR(int fd, fd_set* fdset);   turn off the bit for fd in fdset
int FD_ISSET(int fd, fd_set* fdset);  is the bit for fd on in fdset?
```
描述符集初始化非常重要  
select函数的中间三个参数readset、writeset和exceptset中，如果我们对某一个的条件不感兴趣，就可以把它设为空指针  

maxfdp1参数指定待测试的描述符的个数，它的值是待测试的最大描述符加1

头文件<sys/select.h>中定义的FD_SETSIZE常值是数据类型fd_set中的描述符总数，其值通常是1024

1）select函数修改由指针readset、writeset和exceptset所指向的描述符集，因而这三个参数都是值-结果参数  
2）调用该函数时，指定所关心的描述符的值  
3）函数返回时，结果将指示哪些描述符已就绪  
4）该函数返回后，使用FD_ISSET来测试fd_set数据类型中的描述符  
5）描述符集内任何与未就绪描述符对应的位返回时均清成0，为此，每次重新调用select函数时，都得再次把所有描述符集内所关心的位置为1  


### 描述符就绪条件
#### 1、满足下列四个条件中的任何一个时，一个套接字准备好读
1）该套接字接收缓冲区中的数据字节数大于等于套接字接收缓冲区低水位标记的当前大小，对这样的套接字执行读操作不会阻塞并将返回一个大于0的值（也就是返回准备好读入的数据）  
2）该连接的读半部关闭（也就是接收了FIN的TCP连接），对这样的套接字的读操作将不阻塞并返回0（也就是返回EOF）  
3）该套接字是一个监听套接字且已完成的连接数不为0，对这样的套接字的accept通常不会阻塞  
4）其上有一个套接字错误待处理，对这样的套接字的读操作将不阻塞并返回-1（也就是返回一个错误），同时把errno设置成确切的错误条件  
#### 2、满足下列四个条件中的任何一个时，一个套接字准备好写
1）该套接字发送缓冲区中的可用空间字节数大于等于套接字发送缓冲区低水位标记的当前大小，并且或者该套接字已连接，或者该套接字不需要连接（如UDP套接字），这就意味着如果我们把这样的套接字设置成非阻塞，写操作将不阻塞并返回一个正值  
2）该连接的写半部关闭，对这样的套接字的写操作将产生SIGPIPE信号  
3）使用非阻塞式connect的套接字已建立连接，或者connect已经以失败告终  
4）其上有一个套接字错误待处理，对这样的套接字的写操作将不阻塞并返回-1（也就是返回一个错误），同时把errno设置成确切的错误条件；这些待处理的错误也可以通过指定SO_ERROR套接字选项调用getsockopt获取并清除


# poll函数
```
#include <poll.h>
int poll(struct pollfd* fdarray, unsigned long nfds, int timeout);
```
返回：若有就绪描述符则为其数目，若超时则为0，若出错则为-1

第一个参数是指向一个结构数组第一个元素的指针，每个数组元素都是一个pollfd结构，用于指定测试某个给定描述符fd的条件
```
struct pollfd
{
  int fd;         descriptor to check
  short events;   events of interest on fd
  short revents;  events that occurred on fd
};
```
要测试的条件由events成员指定，函数在revents成员中返回该描述符的状态  

结构数组中的元素的个数是由nfds参数指定

timeout参数指定poll函数返回前等待多长时间，它是一个指定应等待毫秒数的正值  
1) INFTIM：永远等待
2) 0：立即返回，不阻塞进程
3) \>0：等待指定数目的毫秒数

如果我们不再关心某个特定描述符，那么可以把与它对应的pollfd结构的fd成员设置成一个负值，poll函数将忽略这样的pollfd结构的events成员，返回时将它的revents成员的值置为0


# epoll函数
```
#include <sys/epoll.h>
int epoll_create(int size);
```
创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大  
创建好epoll句柄后，它就是会占用一个fd值，所以在使用完epoll后，必须调用close关闭，否则可能导致fd被耗尽
```
#include <sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);
```
1、第一个参数是epoll_create的返回值  
2、第二个参数表示动作，用三个宏来表示   
EPOLL_CTL_ADD：注册新的fd到epfd中  
EPOLL_CTL_MOD：修改已经注册的fd的监听事件  
EPOLL_CTL_DEL：从epfd中删除一个fd  
3、第三个参数是需要监听的fd  
4、第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下
```
typedef union epoll_data
{
  void* ptr;
  int fd;
  __uint32_t u32;
  __uint64_ u64;
}epoll_data_t;

struct epoll_event
{
  _uint32_t events;
  epoll_data_t data;
};
```
events可以是以下几个宏的集合  
1) EPOLLIN：表示对应的文件描述符可以读（包括对端socket正常关闭）  
2) EPOLLOUT：表示对应的文件描述符可以写  
3) EPOLLPRI：表示对应的文件描述符有紧急的数据可读  
4) EPOLLERR：表示对应的文件描述符发生错误  
5) EPOLLHUP：表示对应的文件描述符被挂断  
6) EPOLLET：将epoll设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的  
7) EPOLLONSHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入epoll队列中  
```
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
```
等待事件的产生，参数events用来从内核得到事件的集合，maxevents告知内核这个events有多大，这个maxevents的值不能大于创建epoll_create时的size，参数timeout是超时时间  
该函数返回需要处理的事件数目，返回0表示已超时

# Reactor模式
Reactor是这样一种模式，它要求主线程(I/O处理单元)只负责监听文件描述上是否有事件发生，有的话就立即将该事件通知工作线程(逻辑单元)。除此以外，主线程不做任何其他实质性的工作。读写数据，接受新的连接，以及处理客户请求均在工作线程中完成。
使用同步I/O模型(以epoll_wait为例)实现的Reactor模型的工作流程是：  
1) 主线程往epoll内核事件表中注册socket上的读就绪事件  
2) 主线程调用epoll_wait等待socket上有数据可读  
3) 当socket上有数据可读时，epoll_wait通知主线程。主线程则将socket可读事件放入请求队列  
4) 睡眠在请求队列上的某个工作线程被唤醒，它从socket读取数据，并处理客户请求。然后往epoll内核事件表中注册该socket上的写就绪事件  
5) 主线程调用epoll_wait等待socket可写  
6) 当socket可写时，epoll_wait通知主线程。主线程将socket可写事件放入请求队列  
7) 睡眠在请求队列上的某个工作线程被唤醒，它往socket上写入服务器处理客户请求的结果  
![image](https://github.com/nettik/kitten/blob/master/basic/picture/Reactor%E6%A8%A1%E5%BC%8F%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

# 将套接字设置为非阻塞模型
1) 可以利用socket()函数创建socket时指定创建的socket是非阻塞的
```
int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCk, 0);
```
2) 可以使用accept4()函数，可以直接将返回的socket设置为非阻塞模式，只要将accept4()最后一个参数flags设置成SOCK_NONBLOCK即可
```
int accept4(int sockfd, struct sockaddr* addr, socklen_t* addrlen, int flags);
```
3) 调用fcntl()函数
```
fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
```
