/* Server.c */
 #include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
 
#define SERVER_PORT 21006  
#define BUFFER_SIZE 255
 
  
int main(int argc, char **argv)
{
	int servfd,clifd;
	struct sockaddr_in servaddr,cliaddr;        
 
	servfd = socket(AF_INET,SOCK_STREAM,0);     //创建了一个套接字servfd
 
	servaddr.sin_family = AF_INET;              //为servaddr设置端口号    
	servaddr.sin_port = htons(SERVER_PORT);      
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);  
 
	bind(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr)); //将套接字servfd与servaddr端口绑定在一起

	listen(servfd,10); //设置套接字servfd所绑定的那个端口最多允许接收10个客户的请求
  
	while (1)
	{ 
		char buf[BUFFER_SIZE];
		 
		socklen_t length = sizeof(cliaddr);  //这里定义的length是为了作为下面的一个参数
		clifd = accept(servfd,(struct sockaddr*)&cliaddr,&length);  //本进程从与套接字所绑定的那个servfd端口中接收请求，函数返回一个新的套接字clifd，这个新的套接字绑定cliaddr端口（这个cliaddr是没有事先像servaddr一样来为其指明指向哪一个端口，这个端口指明的是所接收请求的那个客户的端口的）
	 
//函数返回一个新的clifd套接字，这个套接字绑定的是为cliaddr端口，cliaddr端口是为客户端的端口，通过这个套接字就可以于一个客户端进行通信了   
            
		strcpy(buf,"timestamp in server:"); //将要发送的信息放入到buf中
        printf("send data:%s\n",buf);       //输出要发送的信息
        
        send(clifd,buf,BUFFER_SIZE,0);      //将buf中的前BUFFER_SIZE个字节的数据发送出去
		close(clifd);			
	} 
	close(servfd);
	return 0;
}

