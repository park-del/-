/* Server.c */
 
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

 
#define SERVER_PORT 39579
#define BUFFER_SIZE 255
int clifd_id[11]; 
int index=1;

void *server_1(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
           {
      buf_send=(char *)data;
      send(clifd_id[1],buf_send,BUFFER_SIZE,0);
      printf("NO1  :send data:%s\n",buf_send);
      
      recv(clifd_id[1],buf_recv,BUFFER_SIZE,0);
      printf("NO1  :rece data:%s\n",buf_recv);
      sleep(3);   
        }
   close(clifd_id[1]);
}

void *server_2(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];  
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[2],buf_send,BUFFER_SIZE,0);
      printf("NO2  :send data:%s\n",buf_send);
      
      recv(clifd_id[2],buf_recv,BUFFER_SIZE,0);
      printf("NO2  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[2]);
}

void *server_3(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[3],buf_send,BUFFER_SIZE,0);
      printf("NO3  :send data:%s\n",buf_send);
      
      recv(clifd_id[3],buf_recv,BUFFER_SIZE,0);
      printf("NO3  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[3]);

}

void *server_4(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[4],buf_send,BUFFER_SIZE,0);
      printf("NO4  :send data:%s\n",buf_send);
      
      recv(clifd_id[4],buf_recv,BUFFER_SIZE,0);
      printf("NO4  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[4]);

}
void *server_5(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[5],buf_send,BUFFER_SIZE,0);
      printf("NO5  :send data:%s\n",buf_send);
      
      recv(clifd_id[5],buf_recv,BUFFER_SIZE,0);
      printf("NO5  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[5]);

}
void *server_6(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[6],buf_send,BUFFER_SIZE,0);
      printf("NO6  :send data:%s\n",buf_send);
      
      recv(clifd_id[6],buf_recv,BUFFER_SIZE,0);
      printf("NO6  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[6]);

}
void *server_7(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[7],buf_send,BUFFER_SIZE,0);
      printf("NO7  :send data:%s\n",buf_send);
      
      recv(clifd_id[7],buf_recv,BUFFER_SIZE,0);
      printf("NO7  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[7]);

}
void *server_8(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[8],buf_send,BUFFER_SIZE,0);
      printf("NO8  :send data:%s\n",buf_send);
      
      recv(clifd_id[8],buf_recv,BUFFER_SIZE,0);
      printf("NO8  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[8]);

}
void *server_9(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[9],buf_send,BUFFER_SIZE,0);
      printf("NO9  :send data:%s\n",buf_send);
      
      recv(clifd_id[9],buf_recv,BUFFER_SIZE,0);
      printf("NO9  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[9]);

}
void *server_10(void *data)
{
     char *buf_send,buf_recv[BUFFER_SIZE];
     while(1)
            {
      buf_send=(char *)data;
      send(clifd_id[10],buf_send,BUFFER_SIZE,0);
      printf("NO10  :send data:%s\n",buf_send);
      
      recv(clifd_id[10],buf_recv,BUFFER_SIZE,0);
      printf("NO10  :rece data:%s\n",buf_recv);
      sleep(3);   
            }
   close(clifd_id[10]);

}
int main(int argc, char **argv)
{
	int servfd,clifd;
	struct sockaddr_in servaddr,cliaddr;  
   pthread_t th[11];

	if ((servfd = socket(AF_INET,SOCK_STREAM,0)) < 0) //创建了一个套接字servfd
	{
		printf("create socket error!\n");
		exit(1);
	}
 

	servaddr.sin_family = AF_INET;                //指定采用的是ipv4协议来进行通信的
	servaddr.sin_port = htons(SERVER_PORT);       //指定servfd所指向的某台主机的端口号
	servaddr.sin_addr.s_addr = htons(INADDR_ANY); //一台主机是可以有多个ip地址的，这里所设置的是主机
 

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
	 
      clifd_id[index]=clifd;
               
      switch(index)
               {
       case 1: pthread_create(&th[1],NULL,server_1,(void*)argv[1]); break;
       case 2: pthread_create(&th[2],NULL,server_2,(void*)argv[2]); break;
       case 3: pthread_create(&th[3],NULL,server_3,(void*)argv[3]); break;
       case 4: pthread_create(&th[4],NULL,server_4,(void*)argv[4]); break;
       case 5: pthread_create(&th[5],NULL,server_5,(void*)argv[5]); break;
       case 6: pthread_create(&th[6],NULL,server_6,(void*)argv[6]); break;
       case 7: pthread_create(&th[7],NULL,server_7,(void*)argv[7]); break;
       case 8: pthread_create(&th[8],NULL,server_8,(void*)argv[8]); break;
       case 9: pthread_create(&th[9],NULL,server_9,(void*)argv[9]); break;
       case 10: pthread_create(&th[10],NULL,server_10,(void*)argv[10]); break;
               }           

		index++;           	
	} 
	close(servfd);
	return 0;
}

