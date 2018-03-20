一个输入操作通常包括两个不同的阶段：  
1、等待数据准备好  
2、从内核向进程复制数据  
对于一个套接字上的输入操作，第一步通常涉及等待数据从网络中到达。当所等待分组到达时，它被复制到内核中的某个缓冲区。第二步就是把数据从内核缓冲区复制到应用进程缓冲区。


## 阻塞式I/O模型
默认情况下，所有套接字都是阻塞的   
进程调用recvfrom，其系统调用直到数据报到达且被复制到应用进程的缓冲区中或者发生错误才返回   
进程在调用recvfrom开始到它返回的整段时间内是被阻塞的  


### 非阻塞式I/O模型
进程把一个套接字设置成非阻塞是在通知内核：当所请求的I/O操作非得把本进程投入睡眠才能完成时，不要把本进程投入睡眠，而是返回一个错误


### I/O复用模型
有了I/O复用，就可以调用select或poll，阻塞在这两个系统调用中的某一个之上，而不是阻塞在真正的I/O系统调用上


### 信号驱动式I/O模型
可以用信号，让内核在描述符就绪时发送SIGIO信号通知我们


### 异步I/O模型
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

select函数修改由指针readset、writeset和exceptset所指向的描述符集，因而这三个参数都是值-结果参数  
调用该函数时，指定所关心的描述符的值  
函数返回时，结果将指示哪些描述符已就绪  
该函数返回后，使用FD_ISSET来测试fd_set数据类型中的描述符  
描述符集内任何与未就绪描述符对应的位返回时均清成0，为此，每次重新调用select函数时，都得再次把所有描述符集内所关心的位置为1  