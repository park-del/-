		#include "LPC17xx.h"                                                     
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

		int ROW[]={ 1<<0,1<<0,1<<1,1<<2,1<<3};  //有四行，分别用ROW[1]------ROW[4]来表示
		int COL[]={ 1<<0,1<<4,1<<5,1<<6,1<<7};  //有四列，分别用COL[1]------COL[4]来表示             
		uint8_t key=0;	
		int t1,t2,t3,n1,n2,n3,n4,sum1,sum2,sum;
		int a[5];
		int k=0;
		int k1,k2,k3,k4;
		void myDelay (uint32_t ulTime)
		{
				uint32_t i;
				uint32_t j;
				i = 0;
				j=0;
				while (ulTime--) {
						for (i = 0; i < 250; i++){for(j=0;j<4;j++);};
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
			
				if((data & 0x01)!=0)
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
		void GPIOInit( void )
		{  	
			LPC_GPIO2->FIODIR    |= (255<<0);      //设置P2.0---P2.7为GPIO输出    （其中R1---R4分别接P2.0---P2.3；其中L1---L4分别接P2.4---P2.7）
			LPC_GPIO1->FIODIR    |= (7<<18);       //设置P1.18、P1.19、P1.10为GPIO输出
			
			
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

		/*
		采用行扫描的方式，即是   
													 （1） 让四条行线初始输出低电平
													 （2） 第1条列线输出低电平，其余3条输出高电平，检验看是否有某一行为低电平
													 （3） 第2条列线输出低电平，其余3条输出高电平，检验看是否有某一行为低电平
													 （4） 第3条列线输出低电平，其余3条输出高电平，检验看是否有某一行为低电平
		*/
		uint8_t  KEY_Scan(void)
		{	
				int i;
			 
				LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 );        //让四条行线都为输出模式
				LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
				//-------------- 扫描第1列 --------------
				LPC_GPIO2->FIOCLR   = COL[1];  //第1列输出低电平，第2、3、4列输出高电平
				LPC_GPIO2->FIOSET  = COL[2] | COL[3] | COL[4];
				 
				
				LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 );    //让四条行线都为输入模式
				if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key='1';    //第1列、第1行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key='2';    //第1列、第2行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key='3';    //第1列、第3行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key='A';    //第1列、第4行的按键按下

				myDelay(10);
				//-------------- 扫描第2列 --------------
		 
				LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
				LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
				LPC_GPIO2->FIOCLR  |= COL[2];  //第2列输出低电平，第1、3、4列输出高电平
			
				LPC_GPIO2->FIOSET  |= COL[1] | COL[3] | COL[4];
					
					
				LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 );
				if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key='4';    //第2列、第1行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key='5';    //第2列、第2行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key='6';    //第2列、第3行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key='B';    //第2列、第4行的按键按下

				myDelay(10);
				//-------------- 扫描第3列 --------------
				 
				LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
				LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
				LPC_GPIO2->FIOCLR   = COL[3];  //第3列输出低电平，第1、2、4列输出高电平
			
				LPC_GPIO2->FIOSET  = COL[1] | COL[2] | COL[4];
				 
					
					
				LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 );
				if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key='7';    //第1列、第1行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key='8';    //第1列、第2行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key='9';    //第1列、第3行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key='C';  //第1列、第4行的按键按下
				
				myDelay(10);
				//-------------- 扫描第4列 --------------
				 
				LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
				LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
				LPC_GPIO2->FIOCLR  = COL[4];  //第3列输出低电平，第1、2、3列输出高电平
			
				LPC_GPIO2->FIOSET  = COL[1] | COL[2] | COL[3];
				 
				 
				LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
				if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key='*';    //第1列、第1行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key='0';    //第1列、第2行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key='#';    //第1列、第3行的按键按下
				if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key='D';    //第1列、第4行的按键按下
				
				 
				LPC_GPIO2->FIOCLR  = COL[1] | COL[2] | COL[3] | COL[4];    //让四条列线都输出低电平 
				
				 
				myDelay(100);
			return key;
		}

		void EINT3_IRQHandler(void)
		{   
		if( ( LPC_GPIOINT->IO2IntStatF & 1<<3 ) && key=='*') 
		{
					k++;
					if(k==1)  sum1 = n1 + n2*10+ n3*100;
					if(k==2)  sum2 = n1 + n2*10+ n3*100;
					if(k>=3)  k=0;
					myDelay(10);
					LPC_GPIOINT->IO2IntClr |=0xffffffff; //清楚中断标志  
		}
			
			
			
		if( ( LPC_GPIOINT->IO2IntStatF & 1<<0 ) && key=='A') 
		{
			/*	sum = sum1 + sum2;
					k4=sum/1000;
					k3=(sum-k4*1000)/100;
					k2=(sum-(k3*10+k4)*100)/10;
					k1=(sum-(k2*100+k3*10+k4)*10);  */
					sum = sum1 + sum2;
					k1 =  sum2%10;
					k2 = (sum2%100 - k1)/10; 
					k3 = (sum2%1000- k2*10-k1)/100;
					k4 = (sum2%10000-k3*100-k2*10-k1)/1000;
					 
					myDelay(10);
					LPC_GPIOINT->IO2IntClr |=0xffffffff; //清楚中断标志
		}
				
		if( ( LPC_GPIOINT->IO2IntStatF & 1<<1)  && key=='B' ) 
		{
					sum = sum1 - sum2;
					k1 = sum%10;
					k2 = (sum%100 - k1)/10; 
					k3 = (sum%1000- k2*10-k1)/100;
					k4 = (sum%10000-k3*100-k2*10-k1)/1000;
					 
			myDelay(10);
			LPC_GPIOINT->IO2IntClr |=0xffffffff; //清楚中断标志  
					
		}
					
			 
		if( ( LPC_GPIOINT->IO2IntStatF & 1<<2) && key=='C' )//B1按键实现k++的操作通过k的值来选中某一位数码管，123、456分别选中第123位（从右往左数）数码管
		{
					sum = sum1 * sum2;
					k1 = sum%10;
					k2 = (sum%100 - k1)/10;
					k3 = (sum%1000- k2*10-k1)/100;
					k4 = (sum%10000-k3*100-k2*10-k1)/1000;
					 
			myDelay(10);
			LPC_GPIOINT->IO2IntClr |=0xffffffff; //清楚中断标志  
		}


		if( ( LPC_GPIOINT->IO2IntStatF & 1<<3) && key=='D' )//B1按键实现k++的操作通过k的值来选中某一位数码管，123、456分别选中第123位（从右往左数）数码管
		{
				 sum = sum1/sum2;
					k1 = sum%10;
					k2 = (sum%100 - k1)/10; 
					k3 = (sum%1000- k2*10-k1)/100;
					k4 = (sum%10000-k3*100-k2*10-k1)/1000;
					 
			myDelay(10);
			LPC_GPIOINT->IO2IntClr |=0xffffffff; //清楚中断标志

		}
		myDelay(10);
		LPC_GPIOINT->IO2IntClr |=0xffffffff; 
		}

		int main (void)
		{    
			 int xy=1,i,m=1;
			 SystemInit();                                                      
			 GPIOInit();
			
			 NVIC_SetPriority(EINT3_IRQn,4);  
			 NVIC_EnableIRQ(EINT3_IRQn);
			
			 LPC_GPIOINT->IO2IntEnF  |= ( 1<<0 | 1<<1 | 1<<2 | 1<<3 );  //设置P2.0 P2.1 P2.2,P2.3,下降沿中断使能
			 LPC_GPIOINT->IO2IntClr =0xffffffff; //清除中断标志
			
			 
				
			 for(i=0;i<2;i++)
			{
			 while( xy )
			 {
					 if( KEY_Scan() !=0 )    //说明有按键按下
					 {
							switch(key) //执行不同按键按下时所执行的程序
							{
								case '1'  : xuan_4(shuzi_1); n1=1; xy=0;   break;
								case '2'  : xuan_4(shuzi_2); n1=2; xy=0;   break;
								case '3'  : xuan_4(shuzi_3); n1=3; xy=0;   break;
								case 'A'  : myDelay(100); break;
								 
								case '4'  : xuan_4(shuzi_4); n1=4; xy=0;   break;
								case '5'  : xuan_4(shuzi_5); n1=5; xy=0;   break;
								case '6'  : xuan_4(shuzi_6); n1=6; xy=0;   break;
								case 'B'  : break;
								 
								
								case '7'  : xuan_4(shuzi_7); n1=7; xy=0;    break;
								case '8'  : xuan_4(shuzi_8); n1=8; xy=0;    break;
								case '9'  : xuan_4(shuzi_9); n1=9; xy=0;    break;
								case 'C'  : break;
								 
								
								case '*'  : myDelay(100); break;
								case '0'  : xuan_4(shuzi_0); n1=0; xy=0;    break;
								case '#'  : xy=0; break;
								case 'D'  : break;
								 
								default : break;
							}
					 
							key=0;
					 }
				 }
				 myDelay(2000);
				 xy=1;
					 
					 while( xy )
			 {
					 if( KEY_Scan() !=0 )    //说明有按键按下
					 {
							switch(key) //执行不同按键按下时所执行的程序
							{
								case '1'  : xuan_3(shuzi_1); n2=1; xy=0;  break;
								case '2'  : xuan_3(shuzi_2); n2=2; xy=0;  break;
								case '3'  : xuan_3(shuzi_3); n2=3; xy=0;  break;
								case 'A'  : break;
								 
								case '4'  : xuan_3(shuzi_4); n2=4; xy=0;  break;
								case '5'  : xuan_3(shuzi_5); n2=5; xy=0;  break;
								case '6'  : xuan_3(shuzi_6); n2=6; xy=0;  break;
								case 'B'  : break;
								 
								
								case '7'  : xuan_3(shuzi_7); n2=7; xy=0;  break;
								case '8'  : xuan_3(shuzi_8); n2=8; xy=0;  break;
								case '9'  : xuan_3(shuzi_9); n2=9; xy=0;  break;
								case 'C'  : break;
								 
								
								case '*'  : break;
								case '0'  : xuan_3(shuzi_0); n2=0; xy=0;  break;
								case '#'  : xy=0; break;
								case 'D'  : break;
								 
								default : break;
							}
							key=0;
					 }
			 } 
			 
			 
				myDelay(2000);
				xy=1;
					 
					 while( xy )
			 {
					 if( KEY_Scan() !=0 )    //说明有按键按下
					 {
							switch(key) //执行不同按键按下时所执行的程序
							{
								case '1'  : xuan_2(shuzi_1); n3=1; xy=0;  break;
								case '2'  : xuan_2(shuzi_2); n3=2; xy=0;  break;
								case '3'  : xuan_2(shuzi_3); n3=3; xy=0;  break;
								case 'A'  : xy=0;break;
								 
								case '4'  : xuan_2(shuzi_4); n3=4; xy=0;  break;
								case '5'  : xuan_2(shuzi_5); n3=5; xy=0;  break;
								case '6'  : xuan_2(shuzi_6); n3=6; xy=0;  break;
								case 'B'  : break;
								 
								
								case '7'  : xuan_2(shuzi_7); n3=7; xy=0;  break;
								case '8'  : xuan_2(shuzi_8); n3=8; xy=0;  break;
								case '9'  : xuan_2(shuzi_9); n3=9; xy=0;  break;
								case 'C'  : break;
								 
								
								case '*'  : break;
								case '0'  : xuan_2(shuzi_0); n3=0; xy=0; break;
								case '#'  : break;
								case 'D'  : break;
								 
								default : break;
							}
							key=0;
					 }
			 } 
			 myDelay(2000);
			 xy=1;
			 
			 
					 while( xy )
			 {
					 if( KEY_Scan() !=0 )    //说明有按键按下
					 {
							switch(key) //执行不同按键按下时所执行的程序
							{
							 
								case 'A'  : myDelay(100);break;
								 
								 
								case 'B'  : myDelay(100);break;
								 
								
								 
								case 'C'  : myDelay(100);break;
								 
								
								case '*'  : myDelay(100);break;
							 
								case '#'  : xy=0;break;
								case 'D'  : myDelay(100);break;
								 
								default : break;
							}
							key=0;
					 }
			 } 
			 myDelay(2000);
			 xy=1;
			 
			 
		 }
			 while(1)
			 {
				for(i=0; i<4000; i++)
				{  //上面是一个灯亮，下面是一个灯亮，本质就是两个数码管同时发亮的动态扫描程序
						switch(k1)
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
					 switch(k2)
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
					switch(k3)
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
					switch(k4)
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
			
			
			 return 0;
		 }