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
int a1=0,a2=0,a3=0,a4=0;  //秒表显示时间的四个数字
int m,t=0; 
int xyy[3]; int xy=0;    
int n1[3],n2[3],n3[3],n4[3]; 
int k=0;  //关闭秒表
int h1=0,h2=0;
void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}


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
	
	
	  LPC_PINCON->PINSEL0=0x00000005;  //在进行UART串口通信时要将对应端口设置Tex和Rex表示这个开发板与其他设备进行通信时的发送端和接收端。
	//为UART0就找Tex0和Rex0，为UART1就找Tex1和Rex1
}	

void UART0_int()
{
   int m;
   LPC_PINCON->PINSEL0=0x00000005;
	 
	//UADLM,UADLL用来设置发送端和接收端的波特率，即是每秒发送或者接收的二进制位数
	
	
	 LPC_UART0->LCR =0x83;    //LCR只需要设置第7位和第01位，第7位除数锁存器，就是DLL和DLM的开关，当第7位置1时才能向DLM、DLL中写数据来控制通信的波特率；当第7位为0时表示
	 //不能向DLM和DLL中写入数据，也就不能改变波特率。故在对寄存器DLM和DLL进行操作时先使LCR中的第7位为1，操作完后再使LCR中的第7位为0。
	 m=(100000000/4.0)/(16*115200);
	 LPC_UART0->DLM = m/256;
	 LPC_UART0->DLL = m%256;
    //LCR中的第0、1位表示通信的数据长度，一般情况下将第0位和第1位置1表示默认通信的数据长度为1字节，刚好与发送的8位寄存器THR和接受的8位寄存器RBR的长度相一致
	 LPC_UART0->LCR =0x03;
}
 
//*********************************************************************************************************
void send_data1(int data) //查询方式发送8位数据data（所谓查询指的就是通过查发送接收状态寄存器LSR中的值）
{
   LPC_UART0->THR=data;  //要发送的数据需要放到寄存器THR中
	  
	 while(LPC_UART0->LSR &0x40 == 0);  //LSR为发送和接收的状态寄存器，只需要关注第0位和第6位就行了，当有数据来需要进行接受时第0位会被置1；当发送完数据后第6位会被置1
  // LPC_UART0->LSR &0x40 == 0中的4位于第6位，数据发送完毕后第6位会被置1，这句话的意思就是等待第6位数据发送完毕后就接着去执行
	//对于LSR，  第0位----------------接收的状态（有数据要接收时置1）             第6位----------------发送的状态（发送完数据时置1）
}	

int UART0_recedata()
{
  int m;
	while(LPC_UART0->LSR &0x01==0);  //当LSR的第0位被置1时表示有数据要接收，否则的话就一直等待
  m= LPC_UART0->RBR; //发送的数据会被发送到接收寄存器RBR中去，当LSR的第0位被置1时表示有数据要接收，就将要接受的数据（寄存器RBR中的内容就是要接收的数据）取出来保存到变量m
  return m;
}
 
//*********************************************************************************************************
 

/*
     只需知道IIR所表示的三个中断就行了（后面的字母表示中断的名称）
	  两种接收中断（接收中断指的是接收数据来时触发的中断）：（1）010  RDA     
	     RDA中断：接收缓冲区FIFO可以设置1、4、8、14这4个触发点，所谓触发的意思就是触发中断，接收缓冲区FIFO可以暂存16个字节的数据，假如设置触发点为8个字节时，当接收缓冲区
	              FIFO的数据量达到8个字节时就会触发RDA中断，同时使IIR中的第1、2、3位置为010。（注意触发点只是针对于接收缓冲区FIFO来说的，发送缓冲区没有所谓的触发点的概念）
	                                                     （2）110  CTI
	     CTI中断：假如设置接收缓冲区FIFO的触发点为4，当接收缓冲区接收第1个字节的数据时就会启动定时器，开始计时，如果在计时器所规定的时间内接收缓冲区FIFO没有缓冲4个字节的
	             数据，那么就会触发CTI中断，同时使IIR中第1、2、3位置110。
	
	    
	可以在UART0的中断服务函数里面用switch（LPC_UART0->IIR && 0xff）来区分不同的中断，即是执行接收中断，还是执行发送中断，接收中断是执行RDA中断，还是执行CTI中断，
	接收中断与发送中断都是在UART0的中断服务函数里面写的，是通过LPC_UART0->IIR && 0xff的不同取值来区分不同的中断的（类似于GPIO中断中共享通道3，是通过if语句来区分不同的
	GPIO中断的）。
	
	  一种发送中断（发送中断指的是发送数据满足某个条件时所触发的中断）：（1）001 THRE  （当发送缓冲区FIFO由非空变为空时触发中断）
	      THRE中断：当发送缓冲区中的数据发送完后，此时发送缓冲区是没有数据的，表示已发送完毕，此时可以触发中断来执行相应的功能从而判断发送缓冲区的数据是否发送完毕。
	
	 
	RDA中断要执行的功能：RDA中断要执行的功能其实就是取数据，触发了RDA中断此时就知道了接收缓冲区有多少字节的数据了，就可以有针对性地取数据了。如触发点设置为8，则当RDA中断触发
	                    时，此时知道了接收缓冲区FIFO中有8个数据，就可以在RDA中断的中断服务函数里面写上 循环8次从RBR中取数据并放到一个变量中保存的程序。
	
	THRE中断要执行的功能：当执行THRE中断执行完后表示数据已发送完毕，此时可以设置一个标志位，发送数据的程序就根据这个标志位来决定是暂停还是接着往下执行，起到类似于
                       while(LPC_UART0->LSR &0x40 == 0);的功能。
*/
void UART_IN(void)
{
	int rec_data1[50],i=0,m=0,rec_data2[4];

	switch(LPC_UART0->IIR & 0xff)   //IIR为中断标识寄存器，可以理解为中断状态寄存器，只需关注其第1、2、3位就行了。
 {   
	case 0x04: for(m=0;m<4;m++)  rec_data2[m]=LPC_UART0->RBR; break;                                //发生RDA中断
	//触发了RDA中断表示现在接收缓冲区FIFO中有4个数据，可以利用for循环循环四次的方式依次从接收缓冲区中取出这4个数据放到一个数组中
	 
	case 0x0c:  while( LPC_UART0->LSR & 0x01 ==1 ) rec_data1[i++]=LPC_UART0->RBR; break;  //发生CTI中断 
  /*触发了CTI中断表示在定时器所规定时间内接收缓冲区FIFO没有达到触发点的数据量，但是却并不知道现在接收缓冲区FIFO中具体是有多少数据的，故也就没有办法通过for循环的方式来
	  数据，只能通过while循环查询的方式才能够将接收缓冲区FIFO中的数据区完。     */
	 
	/*当将RDA中断的触发点设置为4、8、14时，（以触发点8为例）可能会出现发送方只发送了5个数据，而没有发送8个数据的情况，此时由于无法触发RDA，故也就无法取数据，必须搭配CTI
	  一起使用，CTI中断负责当发送方发送的数据个数为小于8个情况下的读取数据。
	 如果将RDA中断的触发点设置为了1就没有发送方发送的数据个数小于1的情况了，此时只需要用RDA中断，就不需要在用CTI中断了。	*/

  default:break;	
 }

 

}	







/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
int main (void)
{   
	 int i,j; int sum;
	 SystemInit();                                                      
     GPIOInit(); 
	 UART0_int();
	      
	 NVIC_SetPriority(UART0_IRQn,0);  
     NVIC_EnableIRQ(UART0_IRQn);
	
	 send_data1(0x69);
	 sum= UART0_recedata();
	 
     LPC_UART0->IER =0x03;   //中断使能寄存器IER，只关注第0位和第1位，第0位置1表示接收中断使能，第1位置表示发送中断使能（前面的NVIC的中断使能是总开关，这里的IER
	                        //是具体的哪一个中断，接收中断还是发送中断的开关）
 
 return 0;

}		