# Kitten Http Server
This is a tiny http server.
![](https://github.com/nettik/kitten/blob/master/basic/picture/%E4%BB%A3%E7%A0%81%E9%87%8F%E7%BB%9F%E8%AE%A1.png)
## 开发环境
+ 操作系统：Ubuntu 17.10

+ 编辑器：Vim

+ 编译器：gcc 7.2.0

+ 版本控制：git

+ 压测工具：webbench 1.5
## 关键技术
+ 线程池threadpool：  
预先建立一个包含4个工作线程的线程池，当已连接套接字可读时，由主线程将任务添加到线程池，由线程池中的工作线程完成HTTP请求的解析以及HTTP响应的发送
+ 事件驱动模型(epoll模型)：  
首先向epoll注册监听事件；之后当系统监听到连接请求时，会通知主线程建立连接并创建已连接描述符，该已连接描述符被注册到读事件；当用户数据到达时，系统感知到读事件，建立任务并放入线程池，唤醒工作线程工作
+ 非阻塞I/O  
系统调用recv在数据未准备好时并不会阻塞线程，而是通过轮询方式判断I/O是否准备就绪(检查errno是否为EAGAIN)
