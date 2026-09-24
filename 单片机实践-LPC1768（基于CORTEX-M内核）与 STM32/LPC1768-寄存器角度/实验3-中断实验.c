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
int flag0,flag1;
/* LPC17xx?????            */
/*********************************************************************************************************
  ???
*********************************************************************************************************/
#define BEEP            (1ul << 7)
int flag0,flag1,flag2;
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
        LPC_PINCON->PINSEL0  &= ~(0x3 << 20);       /* P0.7、P0.8、P0.9、P0.10为GPIO  */
		LPC_PINCON->PINSEL0  &= ~(0x3 << 14);
		LPC_PINCON->PINSEL0  &= ~(0x3 << 16);
		LPC_PINCON->PINSEL0  &= ~(0x3 << 18);
	
		LPC_GPIO0->FIODIR    |=  (1<<10);          /*  P0.7、P0.8、P0.9、P0.10为输出  */                
        LPC_GPIO0->FIODIR    |=  (1<<7);
	    LPC_GPIO0->FIODIR    |=  (1<<8);
	    LPC_GPIO0->FIODIR    |=  (1<<9);
}


/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
void interupt0()
{
	   int i;
       for( i=0;i<3;i++ )
	   {    //两盏灯交替闪灭3次，间隔0.2s
		 LPC_GPIO0->FIOCLR |= (1<<7); 
		 LPC_GPIO0->FIOCLR |= (1<<8);
		 myDelay(800);
						
		 LPC_GPIO0->FIOSET |= (1<<7);
		 LPC_GPIO0->FIOSET |= (1<<8);
		 myDelay(800); 
						
		 LPC_GPIO0->FIOCLR |= (1<<9);
		 LPC_GPIO0->FIOCLR |= (1<<10);
		 myDelay(800); 	
						
		 LPC_GPIO0->FIOSET |= (1<<9);		
		 LPC_GPIO0->FIOSET |= (1<<10);
		 myDelay(800);
	   }	
	   flag0=0;
}

void interupt1()
{
	  int i;
      for(i=0;i<5;i++)
	  {     //四盏灯闪灭5次次，间隔0.5s
		LPC_GPIO0->FIOCLR |= (1<<7);    
        LPC_GPIO0->FIOCLR |= (1<<8);
        LPC_GPIO0->FIOCLR |= (1<<9);
        LPC_GPIO0->FIOCLR |= (1<<10);						
		myDelay(2000);
							
		LPC_GPIO0->FIOSET |= (1<<7);   
		LPC_GPIO0->FIOSET |= (1<<8);
        LPC_GPIO0->FIOSET |= (1<<9);
        LPC_GPIO0->FIOSET |= (1<<10);
		myDelay(2000); 
	  }
      flag1=0;
}
void interupt2()
{
	  int i;
      for( i=0;i<3;i++ )
	  {     //两盏灯交替闪灭3次，间隔1s
		LPC_GPIO0->FIOCLR |= (1<<7); 
		LPC_GPIO0->FIOCLR |= (1<<8);
		myDelay(4000);
						
		LPC_GPIO0->FIOSET |= (1<<7);
		LPC_GPIO0->FIOSET |= (1<<8);
		myDelay(4000); 
						
		LPC_GPIO0->FIOCLR |= (1<<9);
		LPC_GPIO0->FIOCLR |= (1<<10);
		myDelay(4000); 	
						
		LPC_GPIO0->FIOSET |= (1<<9);		
		LPC_GPIO0->FIOSET |= (1<<10);
		myDelay(4000);
	  }							
      flag2=0;
}9

/*
LPC_ PINCON->PINMODE4 1=( 1 << 20);//设置P2.10为外部中断o的功能
LPC_ sC->EXTMODE &= OxEE;         //模式寄存器有8位，有两个16进制数来进行控制，这8位分别对应了8个外部中断的电平出发方式。其中0为电平触发，
                                   //1为边沿触发(这里是使最低1位为0,表示外部中断0是电平触发的)
LPC_ SC->EXTPOLAR &= 0xFE;//极寄存器也是有8位， 有两个16进制数来进行控制，若为电平触发，极寄存器为o表示低电平触发，为1表示高电平触发若为边沿触发，
                          //极寄存器为0表示下降沿触发，极寄存器为1表示上升沿触发(这里是使最低一位为0， 表示外部中断o位低电平触发)
LPC_ SC->EXTINT = 0x01;//中断标志寄存器有四位， 分别代表了外部中断0、1、2、3.当外部中断0发生时,中断标志寄存器中第一位会被置位1，CPU会根据中断标志
                       //寄存器中的该标志位判断是否去执行相应的中断服务程序，处理完中断后要把这个标志位给去掉，将这个标志位去掉的方法是给这个标志
                       //位再写入一个1。(否则的话CPU会一 直认为中断发生，去执行相应的中断服务程序) 
*/


void Eint0_1_2Init(void)
{


      //P2.10、P2.11、P2.12接口设置成01时外部中断0、1、2。
	  /*分别是P2.10------------------1<<20
	          P2.11------------------1<<21
	          P2.12------------------1<<22	          
      */        
      LPC_PINCON->PINSEL4  |=( 1 << 20); 
      LPC_PINCON->PINSEL4  |=( 1 << 22);   
      LPC_PINCON->PINSEL4  |=( 1 << 24); 
	  LPC_SC->EXTMODE &= 0xF8;  //将外部中断0、1、2、3设置为电平触发，最低3位设0    
	  LPC_SC->EXTPOLAR &= 0xF8; //设置为低电平触发，最低3位设0    
	  LPC_SC->EXTINT = 0x07;    //最低3位写入1使标志位为0
	
}

void EINT0_IRQHandler(void)
{
	  
   LPC_SC->EXTINT  = 0x07; //清楚中断标志位
   
   flag1=0;  //不执行interupt1()
   flag2=0;  //不执行interupt2()
   
   flag0=1;
   
}

void EINT1_IRQHandler(void)
{
	  
   LPC_SC->EXTINT  = 0x03; //清楚中断标志位
   
   flag1=1;
}

void EINT2_IRQHandler(void)
{
	  
   LPC_SC->EXTINT  = 0x03; //清楚中断标志位
  
   flag1=0;  //不执行interupt1()
   
   flag2=1;
}


int main (void)
{  
    SystemInit();                                                      
    GPIOInit();    
	Eint0_1_2Init();
    
	NVIC_SetPriority(EINT0_IRQn,0); //B1按键，执行interupt0()
	NVIC_EnableIRQ(EINT0_IRQn);
	  
	NVIC_SetPriority(EINT1_IRQn,5); //B2按键,执行interupt1()
	NVIC_EnableIRQ(EINT1_IRQn);
    
	NVIC_SetPriority(EINT2_IRQn,3); //B3按键,执行interupt2()
	NVIC_EnableIRQ(EINT2_IRQn);
	 
  
	while (1)
	{
	  LPC_GPIO0->FIOSET |= (1<<7);   //默认4盏灯全灭
	  LPC_GPIO0->FIOSET |= (1<<8);
      LPC_GPIO0->FIOSET |= (1<<9);
      LPC_GPIO0->FIOSET |= (1<<10);
	
	  if(flag0==1)
		  interupt0();
      if(flag1==1)
		  interupt0();
      if(flag2==1)
		  interupt0();
	}
		
}
		     
					
					
		
