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
int t1=0,t2=0,t3=0,t4=0,k=1;
int n1,n2,n3,sum1,sum2,sum,m1,m2,m3;  //m与n是标志位
int xy=0;
void GPIOInit( void )
{
	      LPC_PINCON->PINSEL0  &= ~(0x3 << 14);
          LPC_PINCON->PINSEL0  &= ~(0x3 << 16); /* P0.7 p0.8 p0.9为GPIO中断（要先设置端口为GPIO功能） */
		  LPC_PINCON->PINSEL0  &= ~(0x3 << 18);
	          
	      LPC_GPIO0->FIODIR    &=  ~(1<<7); 
          LPC_GPIO0->FIODIR    &=  ~(1<<8);    /* 将P0.7 P0.8 P0.9设置为输入 */
	      LPC_GPIO0->FIODIR    &=  ~(1<<9);
	
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
				
		  LPC_PINCON->PINSEL2  &= ~(0x3 <<16); /*设置P1.8--------P1.11为片选*/
	      LPC_PINCON->PINSEL2  &= ~(0x3 <<18);
	      LPC_PINCON->PINSEL2  &= ~(0x3 <<20);
	      LPC_PINCON->PINSEL0  &= ~(0x3 <<10); 
				
		  LPC_GPIO1->FIODIR    |=  (1<<8); 
	      LPC_GPIO1->FIODIR    |=  (1<<9);
		  LPC_GPIO1->FIODIR    |=  (1<<10);
		  LPC_GPIO0->FIODIR    |=  (1<<5);     	
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
void xuan_1(int m)
{
   LPC_GPIO1->FIOSET |= 1<<8; //选中第1个数码管
   LPC_GPIO1->FIOCLR |= 1<<9 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	 	
   LPC_GPIO2->FIOCLR  |= m; 
}
void xuan_2(int m)
{
   LPC_GPIO1->FIOSET |= 1<<9; //选中第2个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
   LPC_GPIO2->FIOCLR  |= m; 
}
void xuan_3(int m)
{
   LPC_GPIO1->FIOSET |= 1<<10; //选中第3个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
   LPC_GPIO2->FIOCLR  |= m; 
}
void xuan_4(int m)
{
   LPC_GPIO0->FIOSET |= 1<<5; //选中第4个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9 | 1<<10;
	
   LPC_GPIO2->FIOCLR  |= m; 
}
void led_off()
{
	 LPC_GPIO2->FIOSET |=0x7f;
}

void EINT3_IRQHandler(void)
{   
	 
if( LPC_GPIOINT->IO0IntStatF & 1<<7 )
		{
		if(k==1)  t1++;  
		if(k==2)  t2++; 
		if(k==3)  t1++;  
		if(k==4)  t2++;  
		LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
		}
	  
if( LPC_GPIOINT->IO0IntStatF & 1<<8 )
      {
			if(k==2)  //1与2是一个两位数，k为2时n1和n2都已经被赋值了
			{ 
			sum1 = n1 + n2*10;
			led_off();
		    myDelay(4000);
			xuan_1(shuzi_1);
			myDelay(4000);
				 
			}
           if(k==4) //3与4是一个两位数，k为4时n1和n2都已经被赋值了
			{	
			sum2 = n1 + n2*10;
			led_off();
			myDelay(4000);
			xuan_1(shuzi_2);
			myDelay(4000);
			 }
		   if(k>=5) //按下B2按键，按下第1次，选择第1个二位数的个位数，按下第2次选择第1个2位数的十位数；按下第3次，选择第2个二位数的个位数，按下第4次选择第2个2位数的十位数
			{     //按下第5次进行加法运算得到运算结果的个位数、十位数、百位数，然后开启动态扫描的程序并输出结果
			sum = sum1 + sum2;
		    n1 = sum%10;
			n2 = (sum%100 - n1)/10; 
			n3 = (sum%1000- n2*10-n1)/100;
			xy=1;    
			}
			LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
      }
	 
if( LPC_GPIOINT->IO0IntStatF & 1<<9 )
      {
		    k++;
		    if(k==1)  { t1=0;}  
			if(k==2)  { t2=0;}
			if(k==3)  { t1=0;}
			if(k==4)  { t2=0;}
		  LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
      }

}
int main (void)
{  
	  int i;
      SystemInit();                                                      
      GPIOInit();    
	  
    
	  NVIC_SetPriority(EINT3_IRQn,4);  
	  NVIC_EnableIRQ(EINT3_IRQn);
	
	  LPC_GPIOINT->IO0IntEnF  |= (7<<7);  //设置P0.7 P0.8 P0.9下降沿中断使能
	  LPC_GPIOINT->IO0IntClr =0xffffffff; //清除中断标志
      LPC_GPIO2->FIOSET  =0x7f;
	  while (1)
		{//在关掉灯立刻又打开灯之前要加一个myDelay（），否则会出现残影
		
		
    if(k==1 || k==3) //选中数码管4
    {		
      led_off();
			xuan_4(shuzi_0);
     	switch(t1)
			{
			  case 1: led_off();  xuan_4(shuzi_0); myDelay(10); n1=0; break;
			  case 2: led_off();  xuan_4(shuzi_1); myDelay(10); n1=1; break;
			  case 3: led_off();  xuan_4(shuzi_2); myDelay(10); n1=2; break;
			  case 4: led_off();  xuan_4(shuzi_3); myDelay(10); n1=3; break;
			  case 5: led_off();  xuan_4(shuzi_4); myDelay(10); n1=4; break;
			  case 6: led_off();  xuan_4(shuzi_5); myDelay(10); n1=5; break;
			  case 7: led_off();  xuan_4(shuzi_6); myDelay(10); n1=6; break;
			  case 8: led_off();  xuan_4(shuzi_7); myDelay(10); n1=7; break;
			  case 9: led_off();  xuan_4(shuzi_8); myDelay(10); n1=8; break;
			  case 10: led_off();  xuan_4(shuzi_9); myDelay(10); n1=9; break;
			  default : t1=1; break;
			} 
    }
		    
		//if(k>=2) { t2=0; t3=0; }	
		if(k==2 || k==4) //选中数码管2
		{ 
			led_off();
     	xuan_3(shuzi_0);
		  switch(t2)
			{
				case 1: led_off();  xuan_3(shuzi_0); myDelay(10); n2=0; break;
			  case 2: led_off();  xuan_3(shuzi_1); myDelay(10); n2=1; break;
			  case 3: led_off();  xuan_3(shuzi_2); myDelay(10); n2=2; break;
			  case 4: led_off();  xuan_3(shuzi_3); myDelay(10); n2=3; break;
			  case 5: led_off();  xuan_3(shuzi_4); myDelay(10); n2=4; break;
			  case 6: led_off();  xuan_3(shuzi_5); myDelay(10); n2=5; break;
			  case 7: led_off();  xuan_3(shuzi_6); myDelay(10); n2=6; break;
			  case 8: led_off();  xuan_3(shuzi_7); myDelay(10); n2=7; break;
			  case 9: led_off();  xuan_3(shuzi_8); myDelay(10); n2=8; break;
			  case 10: led_off();  xuan_3(shuzi_9); myDelay(10);n2=9; break;
			  default : t2=1; break;
			} 
		}
		
	  
	
  
		
	//加法的程序
	if(xy==1) 
{
	 
	for(i=0; i<4000; i++)
	{  //上面是一个灯亮，下面是一个灯亮，本质就是两个数码管同时发亮的动态扫描程序
	 switch(n1)
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
			  case 9: led_off(); xuan_2(shuzi_9);  myDelay(10);    break;
			  default :  break;
	}
	myDelay(10);
	
	
	}   //for循环动态扫描
   xy=0;
	k=1;
}  //if语句
 
   }

}	
		
		
