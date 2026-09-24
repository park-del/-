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
void UART0_senddata(int data) //查询方式发送8位数据data（所谓查询指的就是通过查发送接收状态寄存器LSR中的值）
{
   LPC_UART0->THR=data;                //要发送的数据需要放到寄存器THR中
	  
	 while((LPC_UART0->LSR &0x40 )== 0);      //等待数据发送完毕
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
	 int i;
	 char a[10]={'s','d','g','f','r','t','f','d','s','f'};
	 SystemInit();                                                      
     GPIOInit(); 
	 UART0_int();
	//由于THR是一个8位的寄存器，每次最多只能发送一个字节，即是一个字符
	//统一用通过发送单个字节的方式来发送数据，即如果是数字的话就通过'8'、'4'、'3'来发送你想要的数字（统一用不以hex的方式来显示数据）		
	 UART0_senddata('b');  
	 UART0_senddata('v');
	 
	 for(i=0;i<10;i++)  //要想发送一串字符，可以通过字符数组的方式
	      UART0_senddata(a[i]);
 
			 
/*发送字符总结：
（1）可以通过以下的方式来发送键盘上所显示的各个符号（如标点符号等，注意：标点符号只有英文模式下的才可以发送，中文模式下的不可以发送）
	     UART0_senddata(' ');	  UART0_senddata(',');	 UART0_senddata(' ');	 UART0_senddata('>');		
         UART0_senddata('(');	  UART0_senddata('?');	 UART0_senddata('/');	 UART0_senddata('$');
			
（2）可以通过以下的方式来发送各个数字      
		 UART0_senddata('0');	  UART0_senddata('9');	 UART0_senddata('5');	 UART0_senddata('5');	
  
（3）可以通过以下的方式来发送各个字母
         UART0_senddata('a');	  UART0_senddata('A');	 UART0_senddata('v');	 UART0_senddata('d');	 
*/

//在while(1)的循环体里面发送数据时要有一个判断条件来进行有条件的发送
 
 return 0;

}		