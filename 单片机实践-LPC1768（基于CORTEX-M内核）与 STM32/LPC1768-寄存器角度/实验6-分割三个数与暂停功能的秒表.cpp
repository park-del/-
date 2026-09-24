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
   LPC_GPIO0->FIOCLR =  1<<8;  在通信控制数码管时不能有左边这一句话，否则的话会一直处于复位状态（默认输出低电平，数码管一直亮），有myDelay()函数时会
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
				
		LPC_GPIO0->FIODIR    |=  (1<<7); //P0.7------------B    //为输出状态
	    LPC_GPIO0->FIODIR    |=  (1<<8); //P0.8------------MR（清零端）
		LPC_GPIO0->FIODIR    |=  (1<<9); //P0.9------------CP（时钟）			
	
	    LPC_PINCON->PINSEL2  &= ~(0x3 <<16); /*设置P1.8--------P1.11为片选*/
	    LPC_PINCON->PINSEL2  &= ~(0x3 <<18);
	    LPC_PINCON->PINSEL2  &= ~(0x3 <<20);
	    LPC_PINCON->PINSEL0  &= ~(0x3 <<10); 
				
		LPC_GPIO1->FIODIR    |=  (1<<8); 
	    LPC_GPIO1->FIODIR    |=  (1<<9);
		LPC_GPIO1->FIODIR    |=  (1<<10);
		LPC_GPIO0->FIODIR    |=  (1<<5);   
}	
void Eint0_1_2Init(void)
{
    //P2.10、P2.11、P2.12接口设置成01时外部中断0、1、2。
	  /*分别是P2.10------------------1<<20
	          P2.11------------------1<<21
	          P2.12------------------1<<22	          
    */        
      LPC_PINCON->PINSEL4  |=( 1 << 20); //设置P2.10、P2.11、P2.12,即是外部中断0位下降沿触发的方式
	  LPC_PINCON->PINSEL4  |=( 1 << 22);   
      LPC_PINCON->PINSEL4  |=( 1 << 24); 
	  LPC_SC->EXTMODE |= 0x07;  //将外部中断0,1,2设置为边沿触发，最低3位设1    
	  LPC_SC->EXTPOLAR &= 0xF8; //设置为下降沿触发，最低3位设置成0    
	  LPC_SC->EXTINT = 0x07;    //最低3位写入1使标志位为0
}
//*********************************************************************************************************
void TIMER0_IRQHandler(void) //如果想要在执行某个程序的时候定时器继续计时则该程序在主函数里面写，本程序中的闹钟就是主函数中的while(1)里面执行动态扫描的程序，由定时器来
{   //每经过1s执行t++的程序，主函数来动态扫描显示定时器的时间
   t++;
	 if(t==100)  //秒数满59向分钟进1,同时秒数清0
	 {
		 m++;
     t=0;
		 
	 }		 
	 if(m==60)
		  m=0;
	 
	 a1=t%10;
	 a2=(t-t%10)/10;
	 
	 a3=m%10;
	 a4=(m-m%10)/10;
	 myDelay(1);   //定时器也会出现抖动的情况，如果不加这个延时的函数，闹钟的秒数会出现2的倍数的加法
	 
	  
	 // srand((unsigned)LPC_TIM0->TC;
	 
	LPC_TIM0->IR =0xff;
}
////////////////////////////////////////////////////////////////////////////////////////
void EINT0_IRQHandler(void)
{
	   
		h1++;
	if(h1==1)
		{   n1[0]=t%10; n2[0]=(t-t%10)/10; n3[0]=m%10; n4[0]=(m-m%10)/10;  }
    if(h1==2)
		{	  n1[1]=t%10; n2[1]=(t-t%10)/10; n3[1]=m%10; n4[1]=(m-m%10)/10;  }
	if(h1==3)
		{	  n1[2]=t%10; n2[2]=(t-t%10)/10; n3[2]=m%10; n4[2]=(m-m%10)/10;  }
			if(h1>=4)
			h1=0;
		myDelay(10);
	  LPC_SC->EXTINT  = 0x07; //清楚中断标志位
}

void EINT1_IRQHandler(void)
{
	xy++;
	if(xy>=2)
		 xy=0;
	myDelay(1);
    LPC_SC->EXTINT  = 0x07; //清楚中断标志位
}

void EINT2_IRQHandler(void) //开启或者关闭定时器
{
	k++;
	if(k>=2)
		 k=0;
	myDelay(1);
    LPC_SC->EXTINT  = 0x07; //清楚中断标志位
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
	 int i,j; int sum;
	 SystemInit();                                                      
     GPIOInit(); 
	 Eint0_1_2Init();         
	
	 LPC_TIM0->TC=0;       //TC不用管，给它设置成0就行了，设置定时的时间通过PR和MRx来共同设置
	 LPC_TIM0->PR =0;     //设置分频，PR为0就是不分频，将MCx中的内容*1s就是定时的时间；PR为1就是2分频，将MCx中的内容*2s就是定时的时间;PR为3就是3分频，将MCx中的内容*3s就是定时的时间  
     LPC_TIM0->MR0 = (100000000)/(4.0*100);  //后面有8个0为4s,每经过1s产生一次复位中断，触发中断服务程序 
	
     LPC_TIM0->MCR =0x03;  //匹配后产生中断  0x03 0x01  0xc1
     LPC_TIM0->IR =0xff;
	 
     NVIC_SetPriority(TIMER0_IRQn,4);  
	 NVIC_EnableIRQ(TIMER0_IRQn);

	 NVIC_SetPriority(EINT0_IRQn,0); //B1按键按下，执行外部中断0的程序，外部中断是要比定时器中断的优先级更高的
     NVIC_EnableIRQ(EINT0_IRQn);
	
	 NVIC_SetPriority(EINT1_IRQn,0); //B1按键按下，执行外部中断0的程序，外部中断是要比定时器中断的优先级更高的
     NVIC_EnableIRQ(EINT1_IRQn);
	  
	 NVIC_SetPriority(EINT2_IRQn,0); //B1按键按下，执行外部中断0的程序，外部中断是要比定时器中断的优先级更高的
     NVIC_EnableIRQ(EINT2_IRQn);
	
	 LPC_TIM0->TCR =0x01; //启动定时器0
	
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
			  case 9: led_off();  xuan_3(shuzi_9); myDelay(10);    break;
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
		if(xy==1)
			 LPC_TIM0->TCR =0x00; //关闭定时器0
		if(xy==0)  
		   LPC_TIM0->TCR =0x01; //启动定时器0
		if(k==1)  break;
    }  
		if(k==1)   break;
}
	

	  //显示第一个计数的数字
       for(j=0;j<3;j++)
			{
				k=0;
		    while(1)
	     { 
				 
		    for(i=0; i<4000; i++)
	     {  //上面是一个灯亮，下面是一个灯亮，本质就是两个数码管同时发亮的动态扫描程序
	    
			 switch(n1[j])
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
	     switch(n2[j])
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
			  case 9: led_off();  xuan_3(shuzi_9); myDelay(10);    break;
			  default :  break;
		  }
      myDelay(10);
	    switch(n3[j])
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
	    switch(n4[j])
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
		 if(k==1) break;
	   }
		if(k==1)  break;
	 } 
			  
	   }
 return 0;

}		