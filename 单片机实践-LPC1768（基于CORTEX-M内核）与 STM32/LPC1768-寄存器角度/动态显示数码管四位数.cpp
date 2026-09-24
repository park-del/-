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
  //时、分、秒分别为h、m、t
int a4=0,a3=0,a2=0,a1=0;
int xy=0;
int k=0;
int t1,t2,t3,t4;
 
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

 
/////////////////////////////////////////////////////////////////////////////
void EINT3_IRQHandler(void)
{   
if( LPC_GPIOINT->IO0IntStatF & 1<<7 )//B3按键时实现t++的操作，控制某一位数码管所显示的数字
{
			if(t1==1)  a1++;  
			if(t2==1)  a2++; 
			if(t3==1)  a3++;  
		  if(t4==1)  a4++;  
		  if(a1>=10) a1=0;
	    if(a2>=10) a2=0;
	    if(a3>=10) a3=0;
	    if(a4>=10) a4=0;
	   
	   LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
}
	  
 
      
		 
			
	 
if( LPC_GPIOINT->IO0IntStatF & 1<<9 )//B1按键实现k++的操作通过k的值来选中某一位数码管，123、456分别选中第123位（从右往左数）数码管
{
		  k++;  //在这里面加上一个myDelay()延时的程序就不会出现按键抖动的情况了
		  if(k==1)  { t1=1; t2=0; t3=0; t4=0; }  
			if(k==2)  { t2=1; t1=0; t3=0; t4=0; }//第四位数码管显示1 2 3分别代表输入数据的位数
			if(k==3)  { t3=1; t1=0; t2=0; t4=0; }
			if(k==4)  { t4=1; t1=0; t2=0; t3=0; }
			if(k==5)  { xy=1;  t2=0; t3=0;   }
		  myDelay(2000);
			LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
}
}
/////////////////////////////////////////////////////////////////////////////
int main (void)
{  
	  int i;
    SystemInit();                                                      
    GPIOInit(); 
   
	   LPC_GPIOINT->IO0IntEnF  |= (7<<7);  //设置P0.7 P0.8 P0.9下降沿中断使能
	  LPC_GPIOINT->IO0IntClr =0xffffffff; //清除中断标志
      LPC_GPIO2->FIOSET  =0x7f;
 
	 
//////////////////////////////////////////////////////////////////////////////////////////////////////
	  
	 
	  NVIC_SetPriority(EINT3_IRQn,2); //B2按键,执行interupt1()
	  NVIC_EnableIRQ(EINT3_IRQn);

	 LPC_GPIO2->FIOSET |=0x7f;
	
	  while(1)
	  {
		 for(i=0; i<4000; i++)
	  {  //上面是一个灯亮，下面是一个灯亮，本质就是两个数码管同时发亮的动态扫描程序
	    
			switch( a1 )
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
	     switch(a2)
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
	     switch(a3)
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
			  case 9: led_off();  xuan_2(shuzi_9);  myDelay(10);   break;
			  default :  break;
		}
	    myDelay(10);
	    switch(a4)
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
		  if(xy==1)  break;   //标志位为1，退出for循环
		}   //for循环动态扫描
	 if(xy==1) break;     //标志位为1，退出while循环
	}			
//////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	  
}	
		