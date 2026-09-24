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
#define shuzi_0    0x3f        
#define shuzi_1    0x06
#define shuzi_2    0x5b
#define shuzi_3    0x4f
#define shuzi_4    0x66
#define shuzi_5    0x6d
#define shuzi_6    0x7d
#define shuzi_7    0x07
#define shuzi_8    0x7f
#define shuzi_9    0x6f



/*********************************************************************************************************
** Function name:       myDelay
** Descriptions:        ????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}
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
				
				
		  LPC_GPIO0->FIODIR    |=  (1<<7); //P0.7------------B
	      LPC_GPIO0->FIODIR    |=  (1<<8); //P0.8------------CP
		  LPC_GPIO0->FIODIR    |=  (1<<9); //P0.9------------MR
				
}				

 
/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
  
void send_data(int data)
{
     int j;
     LPC_GPIO0->FIOCLR =  1<<8; //不清零前（什么也不做是输出低电平），清零后也是输出低电平
     LPC_GPIO0->FIOSET =  1<<8;  //输出端清零后，要将清零端输入高电平，不执行清零的工作

//这个for循环是用来传送8位数据的程序，虽然是一位一位进行传送的，但由于没有延时函数，可以认为是同时传送的，即是Q0-------Q7同时输出不同的电平
for(j=0;j<8;j++)
	{
	//虽然是一位一位传送的，
	  LPC_GPIO0->FIOCLR = 1<<9;
	
		if(data & 0x01!=0)
		   LPC_GPIO0->FIOSET = 1<<7;
		else
			LPC_GPIO0->FIOCLR = 1<<7;
		
		data >>=1;
		LPC_GPIO0->FIOSET = 1<<9;
	}
	
	
	  myDelay(4000);  //注意：这里必须要加一个延时函数，表示相邻两次执行的时间间隔
	//如果这里没有延时函数，执行完上面的程序后会立即执行下面的LPC_GPIO0->FIOCLR =  1<<8; 程序，立即清楚Q0----Q7的输出，这样Q0--------Q7就相当于一直在输出0
  LPC_GPIO0->FIOCLR =  1<<8; 
}

int main (void)
{  
	int i; int data[]= {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3f,0x7f,0xff};    //（data的两个16进制数分别代表了依次通信的8个二进制数）
	 // int data[]= {0x87,0x94,0x83,0x54,0xba,0x1F,0x3f,0x7f,0xff};
    SystemInit();                                                      
    GPIOInit(); 
		 
	while(1)
	{
	 for(i=1;i<=9;i++)
	 {	
		 send_data(0X87);  //为0x87,1000 0111,从低位到高位分别对应了L8---L1,为1时不亮，为0时亮（从低到高分别对应了Q0------Q7的输出，为1表示输出高电平，为0表示输出
			//低电平）
	 }
	}
	
    return 0;
}
