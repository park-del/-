 //客户端的程序
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
 
#define SERVER_PORT 28006  
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
	socklen_t socklen = sizeof(servaddr);

	char buf[BUFFER_SIZE];     
	
	if (argc < 2)
	{
		usage(argv[0]);
		exit(1);
	}
	
	if ((clifd = socket(AF_INET,SOCK_STREAM,0)) < 0)  
	{
		printf("create socket error!\n");
		exit(1);
	}
	 
	cliaddr.sin_family = AF_INET;   
	cliaddr.sin_port = htons(CLIENT_PORT);  
	cliaddr.sin_addr.s_addr = htons(INADDR_ANY);  
 
	servaddr.sin_family = AF_INET;
	inet_aton(argv[1],&servaddr.sin_addr);
	servaddr.sin_port = htons(SERVER_PORT);
 
	if (bind(clifd,(struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)   
	{
		printf("bind to port %d failure!\n",CLIENT_PORT);
		exit(1);
	}
 
	if (connect(clifd,(struct sockaddr*)&servaddr, socklen) < 0)   
	{
		printf("can't connect to %s!\n",argv[1]);
		exit(1);
	}
	
       
    length = recv(clifd,buf,BUFFER_SIZE,0); 
    if (length < 0)
	{
		printf("error comes when recieve data from server %s!",argv[1]);
		exit(1);
	}
	printf("%s\n",buf);   //打印出接收到的信息

    strcpy(buf,"222222222");
    send(clifd,buf,BUFFER_SIZE,0);
	close(clifd);  
	return 0;
}

