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
#define shuzi_0    0x03  //Q0是最低位连接a段选，Q7是最高位连接g段选    
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
//int q1=0,q2=0,q3=0,q4=0;
//int xyy[3]; 
//int n1[3],n2[3],n3[3],n4[3]; 
//int h1=0,h2=0;
int m=0,s=0,t=0; //t为总秒数，s为秒，m为分钟
int xy=0;   //关闭秒表
int k=-1,q=0;  //两个都是标志位

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
    while (ulTime--) 
		   for (i = 0; i < 5000; i++);
}
/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO???
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
 void  HC595_Init(void)
{  
//-----------HC595引脚初始化------------
	LPC_PINCON->PINSEL0 |=  3<<30;   // 设置P0.15为SCK （时钟信号）
	LPC_PINCON->PINSEL1 |= 0x3f;     // 设置P0.16、P0.17、P0.18分别为SSEL（）、MISO（主机接收从机发送）、MOSI（主机发送从机接收）
  //上面这两段程序是定义SPI的相应引脚的
	
	LPC_PINCON->PINSEL0  &= ~(0x3 << 14);   //设置P0.7、P0.8、P0.9、P0.10为GPIO输出
    LPC_PINCON->PINSEL0  &= ~(0x3 << 16);
	LPC_PINCON->PINSEL0  &= ~(0x3 << 18);
	LPC_PINCON->PINSEL0  &= ~(0x3 << 20);
	
	LPC_GPIO0->FIODIR    |=  (1<<7);	
	LPC_GPIO0->FIODIR    |=  (1<<8);
	LPC_GPIO0->FIODIR    |=  (1<<9);
	LPC_GPIO0->FIODIR    |=  (1<<10);
	
	LPC_GPIO0->FIOCLR |= (1<<7);      //P0.7输出低电平作为主机选中从机的引脚   （P0.7连接从机的SSE0，即是0E使能，低电平有效，从机被选中）
    //LPC_GPIO0->FIOSET |= (1<<8);    //P0.8输出高电平表示不选中主机          （P0.8连接主机的SSE0，即是P0.16，表示主机是不被选中的,这句话可以又或者没有）
	LPC_GPIO0->FIOSET |= (1<<9);      //P0.9输出高电平表示使复位处于无效的状态（让他为高电平）
    LPC_GPIO0->FIOCLR |= (1<<10);     //P0.10通过先输低电平，再输高电平来控制输出一个上升沿，从而将输入的数据从Q0-----Q7这8个端口打出去
	
	//主机的MOSI连接从机的SER,即是主机的P0.17连接从机SER
	//从机的MIS0连接主机的MISO
	//主机的SCK连接从机的SH
	
	/*
  1.主机与从机相连的部分（进行数据传输的部分）
	    主机的：                  从机的：
			                            
			SCK0（P0.15）---------------SH                              //通过SPCCR寄存器来进行控制
		                  //通过端口输出的高低电平来控制
			MISO0（P0.17）--------------Q7'  （从机往主机发送数据）      //
	        MOSI0（P0.18）--------------DS   （主机往从机发送数据）      //通过SPDR寄存器来进行控制
	    
     
												 
  2.主机单独的部分											 
       主机的：			
			 SSE0（P0.16）---------------P0.8 （拉高） 									 
	
  3.从机单独的部分	
		 	 从机的：
			 ST-------------------------P0.10 （P0.10端口先拉低再拉高制造一个上升沿，将数据达到Q0---------Q7端）
			 MR-------------------------P0.9  （P0.9端口输出高电平使复位处于无效的状态）
			
  4.其他说明	   
		 senddata（int data）中的data为8位二进制数，一个字节，表示每次发送的数据位数最多为8位.
	(1)主机中的SCK0 SSE0 MISO MOSI0本质上就是一个端口（将相应的端口功能设置成SCK、SSE、MISO、MOSI就行了）。
	(2)主机与从机真正相连进行通信的只有三根线，就是SCK、MISO、MOSI（SSK是脉冲，表示这个端口产生的是脉冲；MOSI是发送数据，表示这个端口发送的是数据；MISO是接收数据，表示
		 这个端口是接收数据的）。
	(3)主机与从机中SSE这个端口是接电平的(低电平表示选中，是可以单独接一个电源的)，主机与从机中的MR这个端口是接电平的（高电平表示无效，是可以单独接一个电源的），主机与从机
	   中的ST这个端口是用来接电平的（先产生低电平后产生高电平进行刷新，此时主机发送给从机的数据就可会从Q0-----Q7端给发送出来，也是可以接一个电源的）。
		 上述中的可以单独接一个电源的意思是说可以通过控制某一个端口输出电平的情况来进行控制，即通过端口来控制；也可以也可以单独另外使它接电源或者接地来进行控制。
	 
  5.相关寄存器说明
	 一般要设置的寄存器： SPCCR时钟控制寄存器：用来产生SCK，即是发送的一个时钟脉冲
                       SPCR控制寄存器来控制SCK触发方式、主机模式（即主机是发送方）、高或低低字节先传输（Q0为8位二进制数据的最高位还是最低位）			
			                 SPSR状态寄存器：只看第7位就行了，用来等待数据发送完毕的标志
											 
    这些寄存器中SPCCR直接在程序中给其赋一大于8且为偶数的值就行了，SPSR寄存器只需要关注它的第7位就行了，这两个寄存器都没有什么，重点关注SPCR寄存器（这个寄存器要设置的内容
		有点多）
		SPCR寄存器：
    位	   7	    6	   5	   4	   3	    2:0
    功能	SPIE	LSBF	MSTR	CPOL	CPHA	  保留	
		<1> 第3为和第四位共同控制发送SCK（时钟脉冲怎么触发）来发送数据，这两位总共有四种组合，故总共有四种触发的方式（注意：在SCK端口产生的脉冲中，一个时钟周期发送一位数据）。
		<2> 第5位为主模式（为1时）或者从模式（为0时）选择，设置当前开发板是发送数据的一方还是接收数据的一方
		<3> 第6位为LSBF
		    LSBF为1：表示每字节数据是从低位开始传输的，如传输的单字节数据为0x87，化为二进制为1000 0111，因为是从低位开始传输的，故Q0-----Q7所发送的数据分别为1110 0001（简单
		             串行通信也是从数据的低位开始传输的）
		    LSBF为0：表示每字节数据是从高位开始传输的，如传输的单字节数据为0x87，化为二进制为1000 0111，因为是从高位开始传输的，故Q0-----Q7所发送的数据分别为1000 0111
		<4> 第7位为是用来设置否触发SPI中断的：为1表示触发SPI中断，为0表示禁止SPI中断。
		    当SPIF置为时会自动触发SPI中断（SPIF为SPI的状态寄存器SPSR的第7位，当发送完一个data后SPSR寄存器的第7位会被置1，表示发送数据完毕，这也就说明了SPCR寄存器的第7位
				是用来控制当发送完一个数据后是否触发相应的中断的，SPI中断的函数名为……），
*/
	
	 
//---------数码管位选引脚初始化---------	
	LPC_PINCON->PINSEL2  &= ~(0x3 <<16); /*设置P1.8--------P1.10、P0.5为片选*/
	LPC_PINCON->PINSEL2  &= ~(0x3 <<18);
	LPC_PINCON->PINSEL2  &= ~(0x3 <<20);
	LPC_PINCON->PINSEL0  &= ~(0x3 <<10); 
				
	LPC_GPIO1->FIODIR    |=  (1<<8); 
	LPC_GPIO1->FIODIR    |=  (1<<9);
	LPC_GPIO1->FIODIR    |=  (1<<10);
	LPC_GPIO0->FIODIR    |=  (1<<5);  
		
	//----------SPI寄存器初始化-----------  
	LPC_SPI->SPCCR =0x52;    // 设置SPI时钟分频
	LPC_SPI->SPCR  =0x70;    // 设置SPI接口模式，MSTR=1，主模式CPOL=0，CPHA=0，LSBF=1，  
	/* 注意;将SPCR控制寄存器的LSBF设置为1表示每字节数据从低位开始传输，这样就和之前简单串行通信时一样了，就实现了与简单串行通信代码间的兼容 */
} 

int senddata(int data)
{  
    
  	LPC_SPI->SPDR = data;
    while( (LPC_SPI->SPSR & 0x80)==0 );
	LPC_GPIO0->FIOSET |= (1<<10);   //这两步是使P0.10产生一个上升沿（P0.10是与从机的ST相连的），从机的ST产生一个上升沿后数据才会从SPDR数据寄存器输出到Q0-----Q7，
    LPC_GPIO0->FIOCLR |= (1<<10); 	//这就数字电路中的8个一位的触发器，打一个上升沿后这8个数据会存放到这8个触发器中
	return(LPC_SPI->SPDR);
}

void xuan_1(int m)
{
   LPC_GPIO1->FIOSET |= 1<<8; //选中第1个数码管
   LPC_GPIO1->FIOCLR |= 1<<9 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	 senddata(m);
}
void xuan_2(int m)
{
   LPC_GPIO1->FIOSET |= 1<<9; //选中第2个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<10;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	 senddata(m);
}
void xuan_3(int m)
{
   LPC_GPIO1->FIOSET |= 1<<10; //选中第3个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9;
   LPC_GPIO0->FIOCLR |= 1<<5;	
	
   senddata(m);
}
void xuan_4(int m)
{
   LPC_GPIO0->FIOSET |= 1<<5; //选中第4个数码管
   LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9 | 1<<10;
	
   senddata(m);
}
void led_off()
{
   senddata(0xff);//此时的清楚标志就是全部输出高电平，使数码管都不亮
}
 
//*********************************************************************************************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////		 

/*
void Eint0_1_2Init(void)
{
    //P2.10、P2.11、P2.12接口设置成01时外部中断0、1、2。
	  //分别是P2.10------------------1<<20
	          //P2.11------------------1<<21
	          //P2.12------------------1<<22	          
           
     
    	LPC_PINCON->PINSEL4  |=( 1 << 20); //设置P2.10、P2.11、P2.12,即是外部中断0位下降沿触发的方式
	    LPC_PINCON->PINSEL4  |=( 1 << 22);   
      LPC_PINCON->PINSEL4  |=( 1 << 24); 
	    LPC_SC->EXTMODE |= 0x07;  //将外部中断0,1,2设置为边沿触发，最低3位设1    
	    LPC_SC->EXTPOLAR &= 0xF8; //设置为下降沿触发，最低3位设置成0    
	    LPC_SC->EXTINT = 0x07;    //最低3位写入1使标志位为0
}
*/

void GPIOInit( void )
{
	     LPC_PINCON->PINSEL4  &= ~(0x3 << 0);
         LPC_PINCON->PINSEL4  &= ~(0x3 << 2); /* P2.0 p2.1 p2.2为GPIO */
		 LPC_PINCON->PINSEL4  &= ~(0x3 << 4);
	          
	     LPC_GPIO0->FIODIR    &=  ~(1<<0); 
         LPC_GPIO0->FIODIR    &=  ~(1<<1);    /*  P2.0 p2.1 p2.2为输入 */
	     LPC_GPIO0->FIODIR    &=  ~(1<<2);
	
		 LPC_GPIOINT->IO2IntEnF  |= (7<<0);  //p2.0 p2.1 p2.2设置下降沿中断使能
	     LPC_GPIOINT->IO2IntClr =0xffffffff; //清楚中断标志
         led_off();//使刚开始的数码管全灭
  	
}


//*********************************************************************************************************
/*void TIMER0_IRQHandler(void) //如果想要在执行某个程序的时候定时器继续计时则该程序在主函数里面写，本程序中的闹钟就是主函数中的while(1)里面执行动态扫描的程序，由定时器来
{   //每经过1s执行t++的程序，主函数来动态扫描显示定时器的时间
   t++;
	 if(t==60)  //秒数满59向分钟进1,同时秒数清0
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
*/





void TIMER0_IRQHandler(void) //如果想要在执行某个程序的时候定时器继续计时则该程序在主函数里面写，本程序中的闹钟就是主函数中的while(1)里面执行动态扫描的程序，由定时器来
{   //每经过1s执行t++的程序，主函数来动态扫描显示定时器的时间
   t++;
	 if(t>=60)  //秒数满59向分钟进1,同时秒数清0
	 {
		 
		 m=t/60;
		 if(m>=60)
		 {  m=0; }		  
		 a3=m%10;
	     a4=(m-m%10)/10;   //最后一位数码管
		 s=t%60;
		 a1=s%10;
	     a2=(s-s%10)/10;
		 myDelay(1);   //定时器也会出现抖动的情况，如果不加这个延时的函数，闹钟的秒数会出现2的倍数的加法
		 
	 }	
   else if(t<60)
	 { 
		 s=t;m=0;
		 a1=s%10;
	     a2=(s-s%10)/10;
		 a3=m%10;
	     a4=(m-m%10)/10;
		 myDelay(1);   //定时器也会出现抖动的情况，如果不加这个延时的函数，闹钟的秒数会出现2的倍数的加法
	 }		 
    myDelay(1);
	 // srand((unsigned)LPC_TIM0->TC;
	 
	LPC_TIM0->IR =0xff;
}



////////////////////////////////////////////////////////////////////////////////////////
void EINT3_IRQHandler(void)   //修改时间,暂停定时器，控制位选数码管
{
	if( LPC_GPIOINT->IO2IntStatF & 1<<0 ) //按B1键
	{
	  k++;
		myDelay(1); 
	  if(q==1)
	  {
       
     	 if(k==0)  {led_off(); xuan_4(shuzi_0); a1=0; myDelay(2000);}    
			 if(k==1)  {led_off(); xuan_4(shuzi_1); a1=1; myDelay(2000);}
			 if(k==2)  {led_off(); xuan_4(shuzi_2); a1=2; myDelay(2000);}
			 if(k==3)  {led_off(); xuan_4(shuzi_3); a1=3; myDelay(2000);}
			 if(k==4)  {led_off(); xuan_4(shuzi_4); a1=4; myDelay(2000);}
			 if(k==5)  {led_off(); xuan_4(shuzi_5); a1=5; myDelay(2000);}
			 if(k==6)  {led_off(); xuan_4(shuzi_6); a1=6; myDelay(2000);}
			 if(k==7)  {led_off(); xuan_4(shuzi_7); a1=7; myDelay(2000);}
			 if(k==8)  {led_off(); xuan_4(shuzi_8); a1=8; myDelay(2000);}
			 if(k==9)  {led_off(); xuan_4(shuzi_9); a1=9; k=-1;myDelay(2000);} 
       
	   }
    if(q==2)
	  {
       
     	 if(k==0)  {led_off(); xuan_3(shuzi_0); a2=0; myDelay(2000);}    
			 if(k==1)  {led_off(); xuan_3(shuzi_1); a2=1; myDelay(2000);}
			 if(k==2)  {led_off(); xuan_3(shuzi_2); a2=2; myDelay(2000);}
			 if(k==3)  {led_off(); xuan_3(shuzi_3); a2=3; myDelay(2000);}
			 if(k==4)  {led_off(); xuan_3(shuzi_4); a2=4; myDelay(2000);}
			 if(k==5)  {led_off(); xuan_3(shuzi_5); a2=5; myDelay(2000);}
			 if(k==6)  {led_off(); xuan_3(shuzi_6); a2=6; myDelay(2000);}
			 if(k==7)  {led_off(); xuan_3(shuzi_7); a2=7; myDelay(2000);}
			 if(k==8)  {led_off(); xuan_3(shuzi_8); a2=8; myDelay(2000);}
			 if(k==9)  {led_off(); xuan_3(shuzi_9); a2=9; k=-1;myDelay(2000);} 
       
	   }
	if(q==3)
	   {
        
     	 if(k==0)  {led_off(); xuan_2(shuzi_0); a3=0; myDelay(2000);}    
			 if(k==1)  {led_off(); xuan_2(shuzi_1); a3=1; myDelay(2000);}
			 if(k==2)  {led_off(); xuan_2(shuzi_2); a3=2; myDelay(2000);}
			 if(k==3)  {led_off(); xuan_2(shuzi_3); a3=3; myDelay(2000);}
			 if(k==4)  {led_off(); xuan_2(shuzi_4); a3=4; myDelay(2000);}
			 if(k==5)  {led_off(); xuan_2(shuzi_5); a3=5; myDelay(2000);}
			 if(k==6)  {led_off(); xuan_2(shuzi_6); a3=6; myDelay(2000);}
			 if(k==7)  {led_off(); xuan_2(shuzi_7); a3=7; myDelay(2000);}
			 if(k==8)  {led_off(); xuan_2(shuzi_8); a3=8; myDelay(2000);}
			 if(k==9)  {led_off(); xuan_2(shuzi_9); a3=9; k=-1;myDelay(2000);} 
       
	   }
	if(q==4)
	   {
       
     	 if(k==0)  {led_off(); xuan_1(shuzi_0); a4=0; myDelay(2000);}    
			 if(k==1)  {led_off(); xuan_1(shuzi_1); a4=1; myDelay(2000);}
			 if(k==2)  {led_off(); xuan_1(shuzi_2); a4=2; myDelay(2000);}
			 if(k==3)  {led_off(); xuan_1(shuzi_3); a4=3; myDelay(2000);}
			 if(k==4)  {led_off(); xuan_1(shuzi_4); a4=4; myDelay(2000);}
			 if(k==5)  {led_off(); xuan_1(shuzi_5); a4=5; myDelay(2000);}
			 if(k==6)  {led_off(); xuan_1(shuzi_6); a4=6; myDelay(2000);}
			 if(k==7)  {led_off(); xuan_1(shuzi_7); a4=7; myDelay(2000);}
			 if(k==8)  {led_off(); xuan_1(shuzi_8); a4=8; myDelay(2000);}
			 if(k==9)  {led_off(); xuan_1(shuzi_9); a4=9; k=-1;myDelay(2000);} 
       
	    }
     LPC_GPIOINT->IO2IntClr =0xffffffff; //清楚中断标志
	}

  if( LPC_GPIOINT->IO2IntStatF & 1<<1 ) //按B2键,开启或者关闭定时器
                      
   {
	  xy++;
	  if(xy>=2)
		   xy=0;
	  myDelay(1);
      LPC_GPIOINT->IO2IntClr =0xffffffff; //清楚中断标志
   }

    if( LPC_GPIOINT->IO2IntStatF & 1<<2 ) //按B2键,进行数码管位选
   {
	   q++; 
	   if(q>4)
	   {
		    q=0;
	   }		
	  myDelay(2000);
      LPC_GPIOINT->IO2IntClr =0xffffffff; //清楚中断标志
   }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//*********************************************************************************************************
 /*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP????,??P0.7?BEEP,????,???????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
//   $GPRMC,152252,A,2513.3072,N,10346.3723,E,0.0,230.4,250503,1.3,W,A,* 02  解析出经纬度信息并发送出来


int main (void)
{  
   int table[10]=  { 0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};
   int i,j;
	 SystemInit();                                                      
     HC595_Init(); 
     LPC_SPI->SPCCR =0x52&0xfe;    // 设置SPI时钟分频
	 LPC_GPIO0->FIOCLR |= (1<<7);  //使能端，也就是片选线,使片选线输出低电平，表示一直选中那一个从机
	 LPC_GPIO0->FIOSET |= (1<<9);  //复位端，表示使复位一直处于高电平的状态，使复位处于无效的状态 
	 
/////////////////////////////////////////////////////////////////////////////////(定时器程序的部分)	 
	   GPIOInit( );
	 	
	   LPC_TIM0->TC=0;       //TC不用管，给它设置成0就行了，设置定时的时间通过PR和MRx来共同设置
	   LPC_TIM0->PR =0;     //设置分频，PR为0就是不分频，将MCx中的内容*1s就是定时的时间；PR为1就是2分频，将MCx中的内容*2s就是定时的时间;PR为3就是3分频，将MCx中的内容*3s就是定时的时间  
       LPC_TIM0->MR0 = (100000000)/(4.0*10);  //后面有8个0为4s,每经过1s产生一次复位中断，触发中断服务程序 
	
       LPC_TIM0->MCR =0x03;  //匹配后产生中断  0x03 0x01  0xc1
       LPC_TIM0->IR =0xff;
	 
       NVIC_SetPriority(TIMER0_IRQn,4);   //定时器中断的优先级和GPIO中断一样
	   NVIC_EnableIRQ(TIMER0_IRQn);
	 
       NVIC_SetPriority(EINT3_IRQn,4);  
	   NVIC_EnableIRQ(EINT3_IRQn);

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
			  case 9: led_off();  xuan_3(shuzi_9); myDelay(10);   break;
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
			  case 9: led_off();  xuan_2(shuzi_9); myDelay(10);   break;
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
			  case 9: led_off();  xuan_1(shuzi_9); myDelay(10);   break;
			  default :  break;
		}   
		if(xy==1)
			LPC_TIM0->TCR =0x00; //关闭定时器0
		if(xy==0)  
		{
			t=a1+a2*10+(a3+a4*10)*60;
			LPC_TIM0->TCR =0x01; //启动定时器0 
		 }
			  	    
		}   //跳出for循环

	 }  //跳出while循环

 return 0;
   
}		