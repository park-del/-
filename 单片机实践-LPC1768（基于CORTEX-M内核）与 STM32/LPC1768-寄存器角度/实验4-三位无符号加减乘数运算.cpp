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
int t1=0,t2=0,t3=0,k=1;
int n1,n2,n3,n4,sum1,sum2,sum;  //m与n是标志位
int xy=0;
void GPIOInit( void )
{
	      LPC_PINCON->PINSEL0  &= ~(0x3 << 14);
          LPC_PINCON->PINSEL0  &= ~(0x3 << 16); /* P0.7 p0.8 p0.9为GPIO中断 */
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
				
		  LPC_PINCON->PINSEL2  &= ~(0x3 <<16); /*设置P1.8--------P1.10、P0.5为片选*/
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
///////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////
void EINT3_IRQHandler(void)
{   
if( LPC_GPIOINT->IO0IntStatF & 1<<7 )//B3按键时实现t++的操作，控制某一位数码管所显示的数字
{
			if(k==1)  t1++;  
			if(k==2)  t2++; 
			if(k==3)  t3++;  
		    if(k==4)  t1++;  
			if(k==5)  t2++;
			if(k==6)  t3++;
		    LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
}
	  
if( LPC_GPIOINT->IO0IntStatF & 1<<8 )//
{
//只能在k=3，k=6,k=7，k=8时按下B2按键才会有反应，因为只有k=3，k=6,k=7，k=8的if语句，k=3和k=6时按下B2是保存数的功能，k=7和k=8时按下分别是加法和减法的功能
			if(k==3)  //当输入完第1个三位数时，此时k=3，按下B2按键将这个三位数赋值给sum1（保存起来）
			{ 
				sum1 = n1 + n2*10+ n3*100;
			    led_off();
				myDelay(4000);
				xuan_1(shuzi_1);
				myDelay(4000);
			}
            if(k==6) //当输入完第2个三位数时，此时k=6，按下B2按键将这个三位数赋值给sum2（保存起来）
			{	
				sum2 = n1 + n2*10+ n3*100;
			    led_off();
				myDelay(4000);
				xuan_1(shuzi_1);
			    myDelay(4000);
			}
			
			if(k==7)  // 按下B2进行加法运算
			{     
			sum = sum1 + sum2;
		    n1 = sum%10;
			n2 = (sum%100 - n1)/10; 
			n3 = (sum%1000- n2*10-n1)/100;
			n4 = (sum%10000-n3*100-n2*10-n1)/1000;
			xy=1;    
			}
			
			if (k==8) //按下B2按键实现减法功能
			{
			sum = sum1 - sum2;  //只需在这里改变加减乘除，下面不变就行了
		    n1 = sum%10;
			n2 = (sum%100 - n1)/10; 
			n3 = (sum%1000- n2*10-n1)/100;
			n4 = (sum%10000-n3*100-n2*10-n1)/1000;
			xy=1; 
			}
      
			if (k==9) //按下B2按键实现乘法功能
			{
			sum = sum1 * sum2;  //只需在这里改变加减乘除，下面不变就行了
		    n1 = sum%10;
			n2 = (sum%100 - n1)/10; 
			n3 = (sum%1000- n2*10-n1)/100;
			n4 = (sum%10000-n3*100-n2*10-n1)/1000;
			xy=1; 
			}
			LPC_GPIOINT->IO0IntClr |=0xffffffff; //清楚中断标志  
      
}
			
	 
if( LPC_GPIOINT->IO0IntStatF & 1<<9 )//B1按键实现k++的操作通过k的值来选中某一位数码管，123、456分别选中第123位（从右往左数）数码管
{
		    k++;  //在这里面加上一个myDelay()延时的程序就不会出现按键抖动的情况了
		    if(k==1)  { t1=0;}  
			if(k==2)  { t2=0;  led_off(); myDelay(4000); xuan_1(shuzi_2); myDelay(4000);}//第四位数码管显示1 2 3分别代表输入数据的位数
			if(k==3)  { t3=0;  led_off(); myDelay(4000); xuan_1(shuzi_3); myDelay(4000);}
			if(k==4)  { t1=0;  led_off(); myDelay(4000); xuan_1(shuzi_1); myDelay(4000);}
			if(k==5)  { t2=0;  led_off(); myDelay(4000); xuan_1(shuzi_2); myDelay(4000);}
			if(k==6)  { t3=0;  led_off(); myDelay(4000); xuan_1(shuzi_3); myDelay(4000);}

//B1按键是k++的操作，在k=6时输入完了两个3位数，接下来就是模式选择了，共有456789这6中可以设置的模式
//B2按键是执行指定模式的操作，在某一个模式下按下B2按键就会执行相应的操作
            if(k==7)  { led_off(); myDelay(4000); xuan_1(shuzi_7); myDelay(4000); }  //第四位数码管显示7表示可以进行加法操作了
			if(k==8)  { led_off(); myDelay(4000); xuan_1(shuzi_8); myDelay(4000); }  //第四位数码管显示8表示可以进行减法操作了
		    if(k==9)  { led_off(); myDelay(4000); xuan_1(shuzi_9); myDelay(4000); }
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
		
	  if(k==1 || k==4) //选中数码管4
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
	 if(k==2 || k==5) //选中数码管2
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
		
	 if(k==3 || k==6) //选中数码管3
	 { 
			led_off();
         	xuan_3(shuzi_0);
		    switch(t3)
			{
		      case 1: led_off();  xuan_2(shuzi_0); myDelay(10); n3=0; break;
			  case 2: led_off();  xuan_2(shuzi_1); myDelay(10); n3=1; break;
			  case 3: led_off();  xuan_2(shuzi_2); myDelay(10); n3=2; break;
			  case 4: led_off();  xuan_2(shuzi_3); myDelay(10); n3=3; break;
			  case 5: led_off();  xuan_2(shuzi_4); myDelay(10); n3=4; break;
			  case 6: led_off();  xuan_2(shuzi_5); myDelay(10); n3=5; break;
			  case 7: led_off();  xuan_2(shuzi_6); myDelay(10); n3=6; break;
			  case 8: led_off();  xuan_2(shuzi_7); myDelay(10); n3=7; break;
			  case 9: led_off();  xuan_2(shuzi_8); myDelay(10); n3=8; break;
			  case 10: led_off(); xuan_2(shuzi_9); myDelay(10); n3=9; break;
			  default : t3=1; break;
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
      xy=0;
	  k=1;
	 }  //if语句
    }  //while循环

}	
		
		
		
	 
     

		
		