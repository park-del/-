/* Server.c */
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
 
#define SERVER_PORT 20000     //定义一个端口号（端口号的值）
#define LENGTH_OF_LISTEN_QUEUE 10 //length of listen queue in server

#define BUFFER_SIZE 255
#define WELCOME_MESSAGE "welcome to connect the server. "
  
int main(int argc, char **argv)
{
	int servfd,clifd;
	struct sockaddr_in servaddr,cliaddr;    //经过下面的赋值之后sercaddr为PC机上的一个端口，cliaddr为目标机上的一个端口
 
	servfd = socket(AF_INET,SOCK_STREAM,0); //创建了一个套接字servfd
	 
	bzero(&servaddr,sizeof(servaddr));

	 
	servaddr.sin_family = AF_INET;                //指定采用的是ipv4协议来进行通信的
	servaddr.sin_port = htons(SERVER_PORT);       //指定servfd所指向的是本台主机的2000端口的（ip地址指明的是为本台主机的）
	servaddr.sin_addr.s_addr = htons(INADDR_ANY); //一台主机是可以有多个ip地址的，这里所设置的是本台主机的任意一个ip地址的
    //servaddr对应的是本台主机的2000号端口的


	bind(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr)); /*将套接字servfd与主机的2000号端口（servaddr端口）绑定在一起（注意：bind()函数中的第2个参数是为一个
    struct sockaddr *类型的变量的，这里用到的是为struct sockaddr_in类型的，故要进行强制类型转换）*/
 
 
	listen(servfd,LENGTH_OF_LISTEN_QUEUE); /*本进程执行到这里时不会被阻塞，一监听主机的2000号端口（servaddr端口）中是否有连接请求，没有连接请求的话则一直会被阻塞，
	有连接请求的话才会解除阻塞接着往下执行。 */

	//总结：上面的执行过程就是 创建套接字与端口-->将套接字与端口绑定在一起-->监听这个套接字所对应的这个端口是否有连接请求（即是是否有其他的进程尝试建立连接）
 
	while (1)
	{ 
		char buf[BUFFER_SIZE];
		long timestamp;

		socklen_t length = sizeof(cliaddr);

		clifd = accept(servfd,(struct sockaddr*)&cliaddr,&length);  /*本进程接收主机的2000号端口（servaddr端口）中的连接请求，函数返回一个新的套接字，这个新的套接字绑
		定了cliaddr这个端口（即是发送连接请求的那个进程的套接字所绑定的端口），即套接字clifd绑定的是为目标机上的一个端口的（即是为cliaddr端口的）。
		
		注意：在本进程中端口对象cliaddr是没有赋值的，即是并没有在本进程中来指明这个端口对象是指明哪一个端口的，在建立好了连接之后这个端口对象是会自动的被赋值的，即是
		      这个端口会被赋值为发送连接请求的进程所使用的套接字所对应的端口，即是函数connect()中的第1个参数所绑定的端口。

        即是accept(servfd,(struct sockaddr*)&cliaddr,&length)与connect(clifd,(struct sockaddr*)&servaddr, socklen)是互相搭配着使用的，cliaddr会被赋值为套接字clifd所绑定
		的那个端口，而connect所中的套接字clifd所绑定的是为目标机的某一个端口的。
		*/

		strcpy(buf,WELCOME_MESSAGE); //将要发送的数据WELCOME_MESSAGE放入到缓冲区buf中
	 
		
		printf("from client,IP:%s,Port:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
		timestamp = time(NULL);

		strcat(buf,"timestamp in server:");  //strcat()函数是一个拼接字符串的操作，会将in server拼接到buf的后面
		strcat(buf,ctime(&timestamp));       //这里timestpa是一个时间的，将时间加入到buf的后面

		send(clifd,buf,BUFFER_SIZE,0); //本进程执行send函数时会将buf中前BUFFER_SIZE个字节的数据发送到目标机的某一个端口（套接字clifd所绑定的某一个端口）
		close(clifd);		
		
	} 
	close(servfd);
	return 0;
}
//注意：当对应的端口中没有连接请求时执行listen()函数不会被阻塞，而accept()函数是会被阻塞的。
