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
	      LPC_PINCON->PINSEL0  &= ~(0x3 << 14);   //P0.7、P0.8、P0.9设置为GPIO输入
		  LPC_PINCON->PINSEL0  &= ~(0x3 << 16);
		  LPC_PINCON->PINSEL0  &= ~(0x3 << 18);
	
	      LPC_GPIO0->FIODIR    &= ~(1<<7);            /* P0.7、P0.8、P0.9为输入        */
	      LPC_GPIO0->FIODIR    &= ~(1<<8);
		  LPC_GPIO0->FIODIR    &= ~(1<<9);
	
	
	
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<0);  /*设置P2.0-----P2.6为GPIO输出*/
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<2);
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<4);
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<6);
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<8);
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<10);
	      LPC_PINCON->PINSEL4  &= ~(0x3 <<12);
	
	      LPC_GPIO2->FIODIR    |=  (1<<0);
	      LPC_GPIO2->FIODIR    |=  (1<<1); 
	      LPC_GPIO2->FIODIR    |=  (1<<2); 
	      LPC_GPIO2->FIODIR    |=  (1<<3); 
	      LPC_GPIO2->FIODIR    |=  (1<<4);
		  LPC_GPIO2->FIODIR    |=  (1<<5);
		  LPC_GPIO2->FIODIR    |=  (1<<6);
				
		  LPC_PINCON->PINSEL2  &= ~(0x3 <<16); /*设置P1.8--------P1.10、P0.5为片选*/
	      LPC_PINCON->PINSEL2  &= ~(0x3 <<18);
	      LPC_PINCON->PINSEL2  &= ~(0x3 <<20);
	      LPC_PINCON->PINSEL0  &= ~(0x3 <<10); 
				
		  LPC_GPIO1->FIODIR    |=  (1<<8); 
	      LPC_GPIO1->FIODIR    |=  (1<<9);
		  LPC_GPIO1->FIODIR    |=  (1<<10);
	      LPC_GPIO0->FIODIR    |=  (1<<5);     	
}

//选中数码管，参数m为要输出的数字
void xuan_1(int m)//选中第1个数码管
{
   LPC_GPIO1->FIOSET |= 1<<8; 
   LPC_GPIO1->FIOCLR |= 1<<9 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	 	
   LPC_GPIO2->FIOCLR  |= m; 
}
void xuan_2(int m)//选中第2个数码管
{
   LPC_GPIO1->FIOSET |= 1<<9; 
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
   LPC_GPIO2->FIOCLR  |= m; 
}
void xuan_3(int m)//选中第3个数码管
{
   LPC_GPIO1->FIOSET |= 1<<10; 
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
   LPC_GPIO2->FIOCLR  |= m; 
}
void xuan_4(int m)//选中第4个数码管
{
   LPC_GPIO0->FIOSET |= 1<<5; 
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9 | 1<<10;
	
   LPC_GPIO2->FIOCLR  |= m; 
}
void led_off()   //使数码管全灭
{
   LPC_GPIO2->FIOSET |=0x7f;
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
int k=10; int i;
void TIMER0_IRQHandler(void) //如果想要在到达定时器所到达的时间后就停止计时去执行某个程序，则该程序在定时器的中断服务函数里面写，而不在主函数里面写，
{//因为执行定时器中断服务函数的时候定时器会停止计时，而在执行主函数时却不会停止计时。（执行某个任务，定时器停止计时------定时器设置一个时间，到达时
 //间后去执行任务）
 
	 k--;
	 switch(k)
	 {
	   case 9: led_off();  xuan_4(shuzi_9); break;
	   case 8: led_off();  xuan_4(shuzi_8); break;
	   case 7: led_off();  xuan_4(shuzi_7); break;
	   case 6: led_off();  xuan_4(shuzi_6); break;
	   case 5: led_off();  xuan_4(shuzi_5); break;
	   case 4: led_off();  xuan_4(shuzi_4); break;
       case 3: led_off();  xuan_4(shuzi_3); break;
	   case 2: led_off();  xuan_4(shuzi_2); break;
	   case 1: led_off();  xuan_4(shuzi_1); break;
	   
	 }
	 
	if(k==0)    //三路抢答器的实现思想：当数码管显示0时
	{  led_off();  xuan_4(shuzi_0);  
	
		  while(1)  //等待循环，直到某一个按键按下去跳出循环，否则的话系统会立刻进行if判断，执行下面的程序
	    {
			if((LPC_GPIO0->FIOPIN & (1<<7) ) == 0) //按键设置为输入时，按下按键相应位会被置0
	    {
			 led_off();  xuan_1(shuzi_9); break;
			
			}
	
	    if((LPC_GPIO0->FIOPIN & (1<<8) ) == 0)
			{
			 led_off();  xuan_2(shuzi_9); break;
			
			}	
			
			if((LPC_GPIO0->FIOPIN & (1<<9) ) == 0)
		  {
			 led_off();  xuan_3(shuzi_9); break;
			}
	    
			}
	}
	
	if(k==0)
		k=10;
		
	LPC_TIM0->IR =0xff;
}
int main (void)
{  
	int i;
    SystemInit();                                                      
    GPIOInit(); 
   
	LPC_GPIO2->FIOSET =0x7f;
////////////////////////////////////////////////////////////////////	 
	LPC_TIM0->TC=0;       //TC不用管，给它设置成0就行了，设置定时的时间通过PR和MRx来共同设置
	LPC_TIM0->PR =0;     //设置分频，PR为0就是不分频，将MCx中的内容*1s就是定时的时间；PR为1就是2分频，将MCx中的内容*2s就是定时的时间;PR为3就是3分频，将MCx中的内容*3s就是定时的时间  
    LPC_TIM0->MR0 = (100000000)/4.0;  //后面有8个0为4s,每经过1s产生一次复位中断，触发中断服务程序 
	
    LPC_TIM0->MCR =0x03;  //匹配后产生中断  0x03 0x01  0xc1
    LPC_TIM0->IR =0xff;
	 
    NVIC_SetPriority(TIMER0_IRQn,4);  
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	  
	LPC_TIM0->TCR =0x01; //启动定时器0
	while(1); 
}	