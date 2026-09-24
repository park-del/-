/*********************************************Copyright (c)***********************************************
**                                    XMUT CS Embedded System LAB.
**
**                                 http://kczx.xmut.edu.cn/G2S/hpc.cc

**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2016-03-07
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          HuangPengCheng
** Created date:        2016-03-07
** Version:             V1.00
** Descriptions:        ????????
**
*********************************************************************************************************/
#include "LPC17xx.h"  
/* LPC17xx?????            */
/*********************************************************************************************************
  ???
*********************************************************************************************************/
#define shuzi_0    0x03        
#define shuzi_1    0x9f
#define shuzi_2    0x25
#define shuzi_3    0x0d
#define shuzi_4    0x99
#define shuzi_5    0x49
#define shuzi_6    0x41
#define shuzi_7    0x1f
#define shuzi_8    0x01
#define shuzi_9    0x09

#define FPCLK 25000000 
#define  UART_BPS  9600

int m[100];
 
void send_data(int data)    //将send_data()函数放在最前面，这样在位选的函数里也是可以调用这个函数来进行通行传数据的
{
   int j;
   LPC_GPIO0->FIOCLR =  1<<8; 
   LPC_GPIO0->FIOSET =  1<<8;   
   for(j=0;j<8;j++)
	{
	 LPC_GPIO0->FIOCLR = 1<<9;
	
	 if(data & 0x01!=0)
		 LPC_GPIO0->FIOSET = 1<<7;
	 else
		 LPC_GPIO0->FIOCLR = 1<<7;
		
	 data >>=1;
	 LPC_GPIO0->FIOSET = 1<<9;
	}
/* 
	 myDelay(1);   
   LPC_GPIO0->FIOCLR =  1<<8;  在通信控制数码管时不能有左边这一句话，没有myDelay()函数的话会一直处于复位状态（默认输出低电平，数码管一直亮），有myDelay()函数时会
	 每隔myDelay()函数间隔的时间全亮一次。
*/
}

 void xuan_1(int m)
{
   LPC_GPIO1->FIOSET |= 1<<8; //选中第1个数码管
   LPC_GPIO1->FIOCLR |= 1<<9 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	 	
   send_data(m);
}
void xuan_2(int m)
{
   LPC_GPIO1->FIOSET |= 1<<9; //选中第2个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
  send_data(m);
}
void xuan_3(int m)
{
   LPC_GPIO1->FIOSET |= 1<<10; //选中第3个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
   send_data(m);
}
void xuan_4(int m)
{
   LPC_GPIO0->FIOSET |= 1<<5; //选中第4个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9 | 1<<10;
	
   send_data(m);
}
void led_off()
{
   send_data(0xff);//此时的清楚标志就是全部输出高电平，使数码管都不亮
}

void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}


 
/*********************************************************************************************************
** Function name:       myDelay
** Descriptions:        ????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
/*
void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}
*/
/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO???
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
void GPIOInit( void )
{
	  LPC_PINCON->PINSEL0  &= ~(0x3 <<14);  /*设置P0.7-----P0.9为GPIO输出*/
	  LPC_PINCON->PINSEL0  &= ~(0x3 <<16);
	  LPC_PINCON->PINSEL0  &= ~(0x3 <<18);  
				
	  LPC_GPIO0->FIODIR    |=  (1<<7);     //P0.7------------B    //为输出状态
	  LPC_GPIO0->FIODIR    |=  (1<<8);     //P0.8------------MR（清零端）
	  LPC_GPIO0->FIODIR    |=  (1<<9);     //P0.9------------CP（时钟）			
	
	  
	  LPC_PINCON->PINSEL2  &= ~(0x3 <<16); /*设置P1.8--------P1.11为片选*/
	  LPC_PINCON->PINSEL2  &= ~(0x3 <<18);
	  LPC_PINCON->PINSEL2  &= ~(0x3 <<20);
	  LPC_PINCON->PINSEL0  &= ~(0x3 <<10); 
				
	  LPC_GPIO1->FIODIR    |=  (1<<8); 
	  LPC_GPIO1->FIODIR    |=  (1<<9);
	  LPC_GPIO1->FIODIR    |=  (1<<10);
	  LPC_GPIO0->FIODIR    |=  (1<<5);  
	   
	  LPC_PINCON->PINSEL0 |= (0x01<<4)|(0x01<<6);  //设置P2.2和P2.3分别为TXD0和REDX0
}	

void UART0_int()
{
   int Fdiv;
   LPC_UART0->FCR =0x07;
	
   LPC_UART0->LCR =0x83;     
   Fdiv = (FPCLK /4 / 16) / UART_BPS;
   LPC_UART0->DLM = Fdiv / 256;
   LPC_UART0->DLL = Fdiv % 256;
   LPC_UART0->LCR =0x03;
}
 
//*********************************************************************************************************
void UART0_senddata(int data) //查询方式发送8位数据data（所谓查询指的就是通过查发送接收状态寄存器LSR中的值）
{
   LPC_UART0->THR=data;                //要发送的数据需要放到寄存器THR中
	  
	 while((LPC_UART0->LSR &0x40 )== 0);      //等待数据发送完毕
}	

int UART0_recedata()
{
	 int data;
   while((LPC_UART0->LSR &0x01) == 0 );    //等待接收数据完毕
     
			data= LPC_UART0->RBR;     
		return data;
		
  // return data;	
}

void UART0_GetStr(int *m,int num)
{
	for(;num>0;num--)
	{
		*(m++)=UART0_recedata();
	}
}
 
//*********************************************************************************************************
 
/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
int main (void)
{   
   int i;
   char a[10]={'s','d','g','f','r','t','f','d','s','f'};
   SystemInit();                                                      
   GPIOInit(); 
   UART0_int();
	
   LPC_GPIO0->FIOCLR=1<<7;	

   while(1)
	{
	 UART0_GetStr(m,1);
		
	if(m[0]=='a')    //开发板接收到PC机发送的字符a时数码管显示0，同时开发板向PC机发送一串字符
	{
			
		       led_off();
			   myDelay(4000);
			   xuan_1(shuzi_0);
			   myDelay(4000);
			   led_off();
			   
		  for(i=0;i<10;i++)
   	           UART0_senddata(a[i]);
			   
			    m[0]=0; 
	}
	  
	if(m[0]=='b')   //开发板接收到PC机发送的字符b时数码管显示7，同时开发板向PC机发送数字7
	{
		       led_off();
			   myDelay(4000);
			   xuan_1(shuzi_7);
			   myDelay(4000);
			   led_off();
			 
			UART0_senddata('7');
			   m[0]=0; 
	}

	if(m[0]=='c')   //开发板接收到PC机发送的字符c时数码管显示9，同时开发板向PC机发送数字9
	{
	          led_off();
			   myDelay(4000);
			   xuan_1(shuzi_9);
			   myDelay(4000);
			   led_off();
			 
			UART0_senddata('9');
			   m[0]=0; 
	}	
}  
     return 0;

}		