 //客户端的程序
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
 
//目标机做为客户端（目标机执行的是client.c程序），宿主机（PC机）作为服务端（宿主机执行的是server.c程序），目标机来访问宿主机

#define SERVER_PORT 20000 //定义了PC机的一个端口号
#define CLIENT_PORT ((20001+rand())%65536)  //定义了目标机的一个端口号
/*定义client对象的一个端口号（端口号要尽量的大，因为系统内部的一些进程已经把前面的端口号都给占用了，大的端口号是一般是为空闲的，本
进程就可以使用这个空闲的端口来与其他的进程进行通信了）*/
 
#define BUFFER_SIZE 255
#define REUQEST_MESSAGE "welcome to connect the server.\n"
 
void usage(char *name)
{
	printf("usage: %s IpAddr\n",name);
}
 
int main(int argc, char **argv)
{	
	int servfd,clifd,length;  //套接字标识符servfd表示PC机，套接字标识符clifd表示目标机
	struct sockaddr_in servaddr,cliaddr;   //端口对象servaddr对应的是一个PC机的端口，端口对象cliaddr对应是一个目标机的端口
	
	/*
	注意：端口对象中的sin_addr.s_addr数据成员表示的是一个ip地址，是用来寻找某一台主机的，而sin_port表示的是为一个端
    口号，对应的是所确定的那台主机的某一个端口。这样，通过sin_addr.s_addr与sin_port就能够定位到某台主机上的某一个端口了。
   （由于另一个数据成员sin_family一般默认为其赋值AF_INET，故只需要关注sin_addr.s_addr与sin_port这两个数据成员就行了） 
	*/
	

	socklen_t socklen = sizeof(servaddr); //计算PC机的端口对象所占的字节数的
	char buf[BUFFER_SIZE];                //缓冲区的大小为255
	
	if (argc < 2)
	{
		usage(argv[0]);
		exit(1);
/*
在c语言中的argv是为一个命令行参数的，即argv这个字符串数组（数组中的每一个元素是用来存放字符串的）是用来存放所输入的命令的（这个argv中的每一个元素是为一个字符串，所存放的
命令是以空格来进行隔开的），第1个参数argc就是用来指明这个argv数组的元素个数的。
如果在linux中执行的时候输入“./client”,则此时argc是为1的，表示argv这个数组中只有一个元素，即argv[0]="./client"；
如果在linux中执行的时候输入“./client 192.168.0.120”,即在执行的时候就指明这个进程所使用的ip地址，此时argc是为2的，表示argv这个数组中是有2个元素的，其中argv[0]="./client",
     argv[1]="192.168.0.120"。

这个参数argc与argv的作用就是在执行这个函数的时候给这个函数传递一些参数时用来保存这些参数的。
注意：在客户机与宿主机通信时这里所传递的参数argv[1]是为宿主机的ip地址的,在做实验的时候我们输入的是为“./client 192.168.0.105”的，这里指明的ip地址，即是argv[1]就是为宿主机
      （PC机）的ip地址的。即是目标机利用“./client 192.168.105”执行程序时所指明的这个ip地址就是为要访问的服务器的，在执行时就指明了这个目标机要访问那一个服务器。
*/

	}
	 
	srand(time(NULL));  //设置随机数的中种子为time(NULL),由于用到了time函数，故需要包含头文件<time.h>。

	bzero(&cliaddr,sizeof(cliaddr)); 	/*这个函数的作用就是为一个清除内存的作用，即将cliaddr的前sizeof(cliaddr)个字节都置为0。（这个函数时包含在头文件<string.h>中的）*/
	cliaddr.sin_family = AF_INET;                //指定采用的是ipv4协议来进行通信的
	cliaddr.sin_port = htons(CLIENT_PORT);       //指定cliaddr所绑定的端口号
	cliaddr.sin_addr.s_addr = htons(INADDR_ANY); //指定cliaddr所绑定的ip地址（这里所绑定的是为本台主机的任何一个ip地址的，如果是目标机里的进程执行这个程序则绑定的就是目标机的ip地址，如果PC机里的进程执行这个程序则绑定的就是PC机的ip地址）
	/*通过上述的赋值操作后，servaddr这个端口对象指向的是为目标机的一个端口的（由于argv[1]是为192.168.105的，是将这个值赋值给了servaddr.sin_addr，而192.168.105是为目标机的ip
	  地址的，故这个servaddr端口对象对应的是为目标机的某一个端口的，对应的是为目标机的65535-----86636中的某一个端口的） */


	/*
   
    注意：在网络上进行传输时的数据默认是采用大端的方式来进行存储的，而在PC机上的数据在内存中则默认是采用小端的方式来进行存储的，因此pc机上的数据在网络上进行传输时要
	先将数据转换为大端方式的存储形式，然后才能够在网络上进行传输，而pc机从网络上接收一个数据时则要将其转换为小端的方式才能够再存放到内存中去。
    
	实现主机字节序与网络字节序的转换的函数：
    htons(m)：htons()函数用来实现将主机字节序（小端方式存储的）转换为网络字节序（大端方式存储的）
    */
 
	
	bzero(&servaddr,sizeof(servaddr));      //对结构体对象servaddr清楚内存的操作：
	servaddr.sin_family = AF_INET;          //指定采用的是ipv4协议来进行通信的
	inet_aton(argv[1],&servaddr.sin_addr);  //将一个字符串表示的点分十进制IP地址argv[1]转换为网络字节序存储在servaddr.sin_addr（数字表示的二进制地址）中。
	servaddr.sin_port = htons(SERVER_PORT); //指定servaddr所绑定的端口号为2000
    /*通过上述的赋值操作后，servaddr这个端口对象指向的是为PC机的2000号端口的（由于argv[1]是为192.168.105的，是将这个值赋值给了servaddr.sin_addr，而192.168.105是为PC机的ip
	  地址的，故这个servaddr端口对象对应的是为PC机的第2000号端口的。*/
   
	clifd = socket(AF_INET,SOCK_STREAM,0);                   //创建了一个套接字clifd
	bind(clifd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));  //将套接字clifd绑定到目标机的某一个端口（即是cliaddr这个端口）
 
	connect(clifd,(struct sockaddr*)&servaddr, socklen);     /*本进程（目标机上的进程）执行connect时会发送一个连接请求到PC机的2000号端口（就是函数中的第2个参数所指明的那个
	端口，即是servaddr这个端口），同时会将第1个套接字中的端口号赋值给accept()函数的第2个参数。 */
	length = recv(clifd,buf,BUFFER_SIZE,0); /*本进程（目标机上的进程）执行recv()函数时会从目标机上的一个端口中（套接字clifd所绑定的端口）取出BUFFER_SIZE字节长度的数据放入到buf中、*/
    
	printf("from server %s :\n\t%s ",argv[1],buf);
 
	close(clifd);  //关闭本进程所使用的套接字
	return 0;
}

