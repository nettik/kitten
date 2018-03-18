IPv4套接字地址结构  
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


网络字节序和主机字节序转换函数   
#include <netinet/in.h>    
uint16_t htons(uint16_t host16bitvalue);  
uint32_t htonl(uint32_t host32bitvalue);     
均返回网络字节序值
   
uint16_t ntohs(uint16_t net16bitvalue);   
uint32_t ntohl(uint32_t net32bitvalue);      
均返回主机字节序值


字节操纵函数  
#include <string.h>   
void bzero(void* dest, size_t nbytes);   
bzero把目标字节串中指定数目的字节置为0，经常使用该函数来把一个套接字地址结构初始化为0

#include <string.h>  
void* memset(void* dest, int c, size_t len);   
void* memcpy(void* dest, const void* src, size_t nbytes);


地址转换函数   
#include <arpa/inet.h>   
int inet_aton(const char* strptr, struct in_addr* addrptr);    
返回：若字符串有效则为1，否则为0    
将strptr所指C字符串转换成一个32位的网络字节序二进制值，并通过指针addrptr来存储

in_addr_t inet_addr(const char* strptr);  
返回：若字符串有效则为32位二进制网络字节序的IPv4地址，否则为INADDR_NONE

char* inet_ntoa(struct in_addr inaddr);  
返回：指向一个点分十进制数串的指针   
将一个32位的网络字节序二进制IPv4地址转换成相应的点分十进制数串

int inet_pton(int family, const char* strptr, void* addrptr);    
返回：若成功返回1，若输入不是有效的表达式格式则为0，若出错则为-1    
转化由strptr指针所指的字符串，并通过addrptr指针存放二进制结果

const char* inet_ntop(int family, const void* addrptr, char* strptr, size_t len);   
返回：若成功则为指向结果的指针，若出错则为NULL    
从数值格式（addrptr）转换到表达式格式（strptr），len参数是目标存储单元大小
