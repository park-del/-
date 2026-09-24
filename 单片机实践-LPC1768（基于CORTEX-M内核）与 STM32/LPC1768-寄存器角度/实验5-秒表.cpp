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
int t=0,m=0; int n1,n2,n3,n4;  //时、分、秒分别为h、m、t


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

void TIMER0_IRQHandler(void) //如果想要在执行某个程序的时候定时器继续计时则该程序在主函数里面写，本程序中的闹钟就是主函数中的while(1)里面执行动态扫描的程序，由定时器来
{   //每经过1s执行t++的程序，主函数来动态扫描显示定时器的时间
   t++;
	 if(t==60)  //秒数满59向分钟进1,同时秒数清0
	 {
		 m++;
     t=0;
	 }		 
	 if(m==60)
		  m=0;
	 
	 n1=t%10;
	 n2=(t-t%10)/10;
	 
	 n3=m%10;
	 n4=(m-m%10)/10;
	 myDelay(1);   //定时器也会出现抖动的情况，如果不加这个延时的函数，闹钟的秒数会出现2的倍数的加法
	 
	LPC_TIM0->IR =0xff;
}
int main (void)
{  
	  int i;
    SystemInit();                                                      
    GPIOInit(); 
   
	  LPC_GPIO2->FIOSET =0x7f;
////////////////////////////////////////////////////////////////////	 
	  LPC_TIM0->TC=0;       //TC不用管，给它设置成0就行了，设置定时的时间通过PR和MRx来共同设置（TC每经过一个单位分频加1） 
	  LPC_TIM0->PR =0;     //设置分频，PR为0就是不分频，将MCx中的内容*1s就是定时的时间；PR为1就是2分频，将MCx中的内容*2s就是定时的时间;PR为3就是3分频，将MCx中的内容*3s就是定时的时间  
    LPC_TIM0->MR0 = (100000000)/4.0;  //后面有8个0为4s,每经过1s产生一次复位中断，触发中断服务程序 
	
    LPC_TIM0->MCR =0x03;  //匹配后产生中断  0x03 0x01  0xc1
    LPC_TIM0->IR =0xff;
	 
    NVIC_SetPriority(TIMER0_IRQn,4);  
	  NVIC_EnableIRQ(TIMER0_IRQn);
	
	  
	  LPC_TIM0->TCR =0x01; //启动定时器0
	  while(1)
	  {
		 for(i=0; i<4000; i++)
	  {  //上面是一个灯亮，下面是一个灯亮，本质就是两个数码管同时发亮的动态扫描程序
	    
			switch( n1 )
		  {
	      case 0: led_off();  xuan_4(shuzi_0); myDelay(10);   break;
			  case 1: led_off();  xuan_4(shuzi_1); myDelay(10);   break;
			  case 2: led_off();  xuan_4(shuzi_2); myDelay(10);   break;
			  case 3: led_off();  xuan_4(shuzi_3); myDelay(10);   break;
			  case 4: led_off();  xuan_4(shuzi_4); myDelay(10);   break;
			  case 5: led_off();  xuan_4(shuzi_5); myDelay(10);   break;
			  case 6: led_off();  xuan_4(shuzi_6); myDelay(10);   break;
			  case 7: led_off();  xuan_4(shuzi_7); myDelay(10);   break;
			  case 8: led_off();  xuan_4(shuzi_8); myDelay(10);   break;
			  case 9: led_off();  xuan_4(shuzi_9); myDelay(10);   break;
			  default :  break;
		  }
	     myDelay(10);
	     switch(n2)
		 {
	          case 0: led_off();  xuan_3(shuzi_0); myDelay(10);   break;
			  case 1: led_off();  xuan_3(shuzi_1); myDelay(10);   break;
			  case 2: led_off();  xuan_3(shuzi_2); myDelay(10);   break;
			  case 3: led_off();  xuan_3(shuzi_3); myDelay(10);   break;
			  case 4: led_off();  xuan_3(shuzi_4); myDelay(10);   break;
			  case 5: led_off();  xuan_3(shuzi_5); myDelay(10);   break;
			  case 6: led_off();  xuan_3(shuzi_6); myDelay(10);   break;
			  case 7: led_off();  xuan_3(shuzi_7); myDelay(10);   break;
			  case 8: led_off();  xuan_3(shuzi_8); myDelay(10);   break;
			  case 9: led_off(); xuan_3(shuzi_9); myDelay(10);    break;
			  default :  break;
		 }
      myDelay(10);
	     switch(n3)
		{
	          case 0: led_off();  xuan_2(shuzi_0); myDelay(10);   break;
		 	  case 1: led_off();  xuan_2(shuzi_1); myDelay(10);   break;
			  case 2: led_off();  xuan_2(shuzi_2); myDelay(10);   break;
			  case 3: led_off();  xuan_2(shuzi_3); myDelay(10);   break;
			  case 4: led_off();  xuan_2(shuzi_4); myDelay(10);   break;
			  case 5: led_off();  xuan_2(shuzi_5); myDelay(10);   break;
			  case 6: led_off();  xuan_2(shuzi_6); myDelay(10);   break;
			  case 7: led_off();  xuan_2(shuzi_7); myDelay(10);   break;
			  case 8: led_off();  xuan_2(shuzi_8); myDelay(10);   break;
			  case 9: led_off(); xuan_2(shuzi_9);  myDelay(10);   break;
			  default :  break;
		}
	    myDelay(10);
	    switch(n4)
		{
	          case 0: led_off();  xuan_1(shuzi_0); myDelay(10);   break;
			  case 1: led_off();  xuan_1(shuzi_1); myDelay(10);   break;
			  case 2: led_off();  xuan_1(shuzi_2); myDelay(10);   break;
			  case 3: led_off();  xuan_1(shuzi_3); myDelay(10);   break;
			  case 4: led_off();  xuan_1(shuzi_4); myDelay(10);   break;
			  case 5: led_off();  xuan_1(shuzi_5); myDelay(10);   break;
			  case 6: led_off();  xuan_1(shuzi_6); myDelay(10);   break;
			  case 7: led_off();  xuan_1(shuzi_7); myDelay(10);   break;
			  case 8: led_off();  xuan_1(shuzi_8); myDelay(10);   break;
			  case 9: led_off();  xuan_1(shuzi_9);  myDelay(10);  break;
			  default :  break;
		}   
	    myDelay(10);
	
	  }   //for循环动态扫描
	}			
}	
		
