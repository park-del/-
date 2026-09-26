/* Server.c */
 
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
 
#define SERVER_PORT 28006  
#define BUFFER_SIZE 255
 
  
int main(int argc, char **argv)
{
	int servfd,clifd;
	struct sockaddr_in servaddr,cliaddr;  
 
	if ((servfd = socket(AF_INET,SOCK_STREAM,0)) < 0)  
	{
		printf("create socket error!\n");
		exit(1);
	}
 

	servaddr.sin_family = AF_INET;              
	servaddr.sin_port = htons(SERVER_PORT);      
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);  
 

	if (bind(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0) 
	{
		printf("bind to port %d failure!\n",SERVER_PORT);
		exit(1);
	}
 
	if (listen(servfd,10) < 0)
	{
		printf("call listen failure!\n");
		exit(1);
	}
 
	while (1)
	{ 
		char buf[BUFFER_SIZE];
		 
		socklen_t length = sizeof(cliaddr);
		clifd = accept(servfd,(struct sockaddr*)&cliaddr,&length);

		if (clifd < 0)
		{
			printf("error comes when call accept!\n");
			break;
		}
	 
              
		strcpy(buf,"1111111");  
        printf("send data:%s\n",buf);     
        send(clifd,buf,BUFFER_SIZE,0);   

        recv(clifd,buf,BUFFER_SIZE,0);
        printf("recv data:%s\n",buf);
	    
		close(clifd);			
	} 
	close(servfd);
	return 0;
}

