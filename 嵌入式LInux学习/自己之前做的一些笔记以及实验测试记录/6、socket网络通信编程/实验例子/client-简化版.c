 //客户端的程序
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
 
#define SERVER_PORT 21006  
#define CLIENT_PORT ((20001+rand())%65536)  

#define BUFFER_SIZE 255
void usage(char *name)
{
	printf("usage: %s IpAddr\n",name);
}
 
int main(int argc, char **argv)
{	
	int servfd,clifd,length = 0;
	struct sockaddr_in servaddr,cliaddr;   
	

	char buf[BUFFER_SIZE];     
	
	if (argc < 2)
	{
		usage(argv[0]);
		exit(1);
	}
	
    clifd = socket(AF_INET,SOCK_STREAM,0);          //创立一个clifd套接字

	cliaddr.sin_family = AF_INET;                   //为cliaddr设置端口号（本机的一个端口）
	cliaddr.sin_port = htons(CLIENT_PORT);  
	cliaddr.sin_addr.s_addr = htons(INADDR_ANY);  
 
	servaddr.sin_family = AF_INET;                   //为servaddr设置端口号（服务器的一个端口）
	inet_aton(argv[1],&servaddr.sin_addr);
	servaddr.sin_port = htons(SERVER_PORT);
 
	bind(clifd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));  //将套接字clifd与cliaddr端口绑定在一起

    socklen_t socklen = sizeof(servaddr);
	connect(clifd,(struct sockaddr*)&servaddr,socklen);  //本进程向servaddr端口发送一个连接请求，另外的一个进程通过accept()函数从这个端口中接收请求同时所创建的一个新的套接字会与本函数中的clifd这个套接字的端口绑定在一起

    recv(clifd,buf,BUFFER_SIZE,0);   //从clifd套接字所绑定的那个端口中读取BUFFER_SIZE字节的数据，将读取到的数据放入到buf中
 
	printf("%s\n",buf);//打印出接收到的信息
 
	close(clifd);  
	return 0;
}

