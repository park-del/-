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

int m[6]={0,0,0,0,0,0};
 
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
 
int UART0_recedata()
{
	 int data;
   while((LPC_UART0->LSR &0x01) == 0 );    //等待接收数据完毕
     
			data= LPC_UART0->RBR;     
		return data;
		
  // return data;	
}

void UART0_GetStr(int *m,int num)  //接收数据的函数,接收的数据存放到数组m中，参数num是用来控制执行一次这个函数会接受多少个字符
//如num为1表示每次执行这个函数接收1个字符，这1个字符存放到m[0]中；如num为6表是每次执行这个函数接收6个字符，接收的这6个字符依次存放到
//m[0]、m[1]、……、m[5]中。
//可以通过if(m[0]==?)来执行单个字符的程序；可以通过if(m[0]==?&&……&&m[5]==?)来执行一个字符串的程序
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
   SystemInit();                                                      
   GPIOInit(); 
   UART0_int();
	
   LPC_GPIO0->FIOCLR=1<<7;	
	
   while(1)
   { 
		 UART0_GetStr(m,1);   //这个函数的作用是将PC端发送的字符（对应m[0]）或者字符串（对应m[0]、m[1]、……、m[n]）接收过来并存放到数组m中
/*这个UART0_GetStr(m,1)函数一般是放到while循环里面的，这样可以发送多次某个字符从而执行多次这个字符所对应的程序，如果放到while循环外面，则就只执行一次接收
  字符放到数组m中的任务，这个字符对应的程序也就只能执行一次。	 */
		 //每次传的都是首地址m，故每次最先接收数据的都是m[0]
/*
		 输入的时候可以输入一个字符存放到m[0]中， 然后在主函数中通过if（ m[0]== ）的不同取值来控制从pc端发送不同单个字符时所执行的程序，这样就起到
		 类似于按键的功能了，即‘a’按键执行程序，‘b’按键执行的程序，‘c’按键执行的程序		 
*/
		 /*注意当为UART0_GetStr(m,1)时，也是可以同时输入多个字符的（此时PC机会根据参数1来决定单个单个地发送，如果是参数2则就是两个两个地发送），如输入的为abc，则
		   就相当于PC机先发送a放到m[0]中，然后开发板执行m[0]==a的程序；接着PC机再发送b放到m[0]中，然后开发板执行m[0]==b的程序，依次类推 
		   可以测试一下：发送的为aaa,bbb,abc;(可以根据这种方式来为发送的数据进行编码，从而决定数码管依次显示不同的花样)
	   */
		 if( m[0]== 'a')
		{		 //显示一个
			   led_off();
			   myDelay(10);
			   xuan_1(shuzi_4);
			   myDelay(4000);
			   led_off();
			   m[0]=0;     //注意，执行完某个发送的字符所对应的程序后一定要使存放这个字符的m[0]清0，否则的话这个if语句所执行的程序会一直执行
	
		}
		
		 if( m[0]== 'b')
		{		 
			   led_off();
			   myDelay(4000);
			   xuan_1(shuzi_7);
			   myDelay(4000);
			   led_off();
			   m[0]=0; 
		}
		
		 if( m[0]== 'c')
		{		 
			   led_off();
			   myDelay(4000);
			   xuan_1(shuzi_8);
			   myDelay(4000);
			   led_off();
			   m[0]=0; 
		}
		
   }
 
 return 0;

}		