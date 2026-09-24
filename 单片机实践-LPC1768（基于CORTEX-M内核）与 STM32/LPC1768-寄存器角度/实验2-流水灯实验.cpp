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
#include "LPC17xx.h"                                                    /* LPC17xx?????            */
/*********************************************************************************************************
  ???
*********************************************************************************************************/
#define BEEP            (1ul << 7)

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
        LPC_PINCON->PINSEL0  &= ~(0x3 << 20);       /* P0.7、P0.8、P0.9、P0.10为GPIO */
		LPC_PINCON->PINSEL0  &= ~(0x3 << 14);
		LPC_PINCON->PINSEL0  &= ~(0x3 << 16);
		LPC_PINCON->PINSEL0  &= ~(0x3 << 18);
	
		LPC_GPIO0->FIODIR    |=  (1<<10);           /*  P0.7、P0.8、P0.9、P0.10为输出  */                
        LPC_GPIO0->FIODIR    |=  (1<<7);
	    LPC_GPIO0->FIODIR    |=  (1<<8);
	    LPC_GPIO0->FIODIR    |=  (1<<9);

        LPC_PINCON->PINSEL2  &= ~(0x3 << 16);       /* P1.8、P1.9、P1.10为GPIO        */
		LPC_PINCON->PINSEL2  &= ~(0x3 << 18);
		LPC_PINCON->PINSEL2  &= ~(0x3 << 20);
	              
		LPC_GPIO1->FIODIR    &= ~(1<<8);            /* P1.8、P1.9、P1.10为输入        */
	    LPC_GPIO1->FIODIR    &= ~(1<<9);
		LPC_GPIO1->FIODIR    &= ~(1<<10);


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
	//注意定义变量时必须在开头，否则的话会出错
    int table1[ ]={0x300,0x300,0x300,0x300,0x500,0x500,0x500,0x500}; //0x300是控制P0.8和P0.9  ;0x500是控制P0.10和p0.8
    int table2[ ]={0x480,0x480,0x480,0x480,0x280,0x280,0x280,0x280}; //0x480是控制P0.10和P0.7 ;0x280控制P0.9和P0.7
		unsigned int i = 0,j;
		
    SystemInit();                                                      
    
    GPIOInit();

    while (1) 
			{
		    //P1.8接口是与B1按键相连的,P1.9接口是与B2按键相连的,P1.10接口是与B3按键相连的
		    //mydlay()函数中4000是1s，则2000是0.5s，400是0.1s 
					 
			/*
			  LPC_GPIO0->FIOCLR |= (1<<7);        
              LPC_GPIO0->FIOCLR |= (1<<8);
              LPC_GPIO0->FIOCLR |= (1<<9);
              LPC_GPIO0->FIOCLR |= (1<<10);
				加入这一段程序后刚开始时（即是默认的）P0.7、P0.8、P0.9、P0.10是输出低电平的，但此时小灯泡会全亮，说明了小灯泡是低电平点亮   
			*/
		   
		    LPC_GPIO0->FIOSET |= (1<<7);  //使刚开始时的四盏灯全灭
		    LPC_GPIO0->FIOSET |= (1<<8);
          	LPC_GPIO0->FIOSET |= (1<<9);
          	LPC_GPIO0->FIOSET |= (1<<10);
				
		    if((LPC_GPIO1->FIOPIN & (1<<8) ) == 0)
			{	   //按下B1按键，实现四盏灯亮灭循环5次
				for(i=0;i<5;i++)
				{
				LPC_GPIO0->FIOCLR |= (1<<7);   //由于小灯泡是低电平点亮，这一段程序是使小灯泡亮的
                LPC_GPIO0->FIOCLR |= (1<<8);
                LPC_GPIO0->FIOCLR |= (1<<9);
                LPC_GPIO0->FIOCLR |= (1<<10);						
			    myDelay(2000);
							
			    LPC_GPIO0->FIOSET |= (1<<7);  //由于小灯泡是低电平点亮，这一段程序是使小灯泡熄灭的
				LPC_GPIO0->FIOSET |= (1<<8);
          	    LPC_GPIO0->FIOSET |= (1<<9);
          	    LPC_GPIO0->FIOSET |= (1<<10);
			    myDelay(2000); 
				    	    
				}
			}
				
		    if((LPC_GPIO1->FIOPIN & (1<<9))  == 0)
			{      //按下B2按键，实现两盏灯交替亮灭循环3次（间隔0.2s）
   				for( i=0;i<3;i++ )
				{
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
			}
				
			if((LPC_GPIO1->FIOPIN & (1<<10) ) == 0)
			{
						
				for( j=0;j<2;j++ )      //外加一层循环是为了增加循环次数的
				{				    
					for( i=0;i<8;i++ )  //间隔0.5s
					{
					LPC_GPIO0->FIOCLR  =  table1[i];  //开启table1,同时关闭table2 
					LPC_GPIO0->FIOSET  =  table2[i];
					myDelay(2000);

					LPC_GPIO0->FIOSET  =  table1[i];  //开启table2,同时关闭table1
					LPC_GPIO0->FIOCLR  =  table2[i];
					myDelay(2000);
					}
				}
						
						
			  /*        按下B3按键，实现一盏一盏的亮，且一盏灯亮完熄灭后另一盏灯亮
						LPC_GPIO0->FIOSET |= (1<<7);
						myDelay(2000);
						LPC_GPIO0->FIOCLR |= (1<<7);
						myDelay(2000);	

						LPC_GPIO0->FIOSET |= (1<<8);
						myDelay(2000);
						LPC_GPIO0->FIOCLR |= (1<<8);
						myDelay(2000);
						
						LPC_GPIO0->FIOSET |= (1<<9);
						myDelay(2000);
					  LPC_GPIO0->FIOCLR |= (1<<9);
						myDelay(2000); 
						
						LPC_GPIO0->FIOSET |= (1<<10);
						myDelay(2000);
					  LPC_GPIO0->FIOCLR |= (1<<10);
						myDelay(2000);
				*/
					
			}
		
            }
					
}


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

