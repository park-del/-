#include "LPC17xx.h"                                                     
  
int ROW[]={ 1<<0,1<<0,1<<1,1<<2,1<<3};  //有四行，分别用ROW[1]------ROW[4]来表示
int COL[]={ 1<<0,1<<4,1<<5,1<<6,1<<7};  //有四列，分别用COL[1]------COL[4]来表示             
uint8_t key=0;	


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

  

void GPIOInit( void )
{  	
	LPC_GPIO2->FIODIR   |= (255<<0);      //设置P2.0---P2.7为GPIO输出    （其中R1---R4分别接P2.0---P2.3；其中L1---L4分别接P2.4---P2.7）
	LPC_GPIO1->FIODIR   |= (7<<18);       //设置P1.18、P1.19、P1.10为GPIO输出
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
/*
  让1号列线先输出低电平，其他3条列线输出高电平，然后依次检验1--4这四条行线，看是否有一条行线输出低电平
  通过 LPC_GPIO2->FIOPIN 来判断P2的各个引脚的电平情况，按键按下去会将该按键对应的行线与列线短接，行线会输出相应的低电平
*/
	  
	  LPC_GPIO2->FIOCLR   = COL[1];  //第1列输出低电平，第2、3、4列输出高电平
	  LPC_GPIO2->FIOSET  = COL[2] | COL[3] | COL[4];
	   
	  
	  LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 );    //让四条行线都为输入模式
	  if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key=1;         //第1列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key=2;         //第1列、第2行的按键按下
      if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key=3;         //第1列、第3行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key=4;         //第1列、第4行的按键按下

    //-------------- 扫描第2列 --------------
 
	  LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
  	  LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
	  LPC_GPIO2->FIOCLR  |= COL[2];  //第2列输出低电平，第1、3、4列输出高电平
	
	  LPC_GPIO2->FIOSET  |= COL[1] | COL[3] | COL[4];
	    
			
	  LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 );
	  if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key=5;    //第2列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key=6;    //第2列、第2行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key=7;    //第2列、第3行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key=8;    //第2列、第4行的按键按下

    //-------------- 扫描第3列 --------------
		 
	  LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
  	  LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
	  LPC_GPIO2->FIOCLR   = COL[3];  //第3列输出低电平，第1、2、4列输出高电平
	
	  LPC_GPIO2->FIOSET  = COL[1] | COL[2] | COL[4];
	   
	    
			
	  LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 );
	  if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key=9;     //第1列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key=10;    //第1列、第2行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key=11;    //第1列、第3行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key=12;    //第1列、第4行的按键按下
		
		//-------------- 扫描第4列 --------------
		 
	  LPC_GPIO2->FIODIR  |= (1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
  	  LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出高电平 
	  LPC_GPIO2->FIOCLR  = COL[4];  //第3列输出低电平，第1、2、3列输出高电平
	
	  LPC_GPIO2->FIOSET  = COL[1] | COL[2] | COL[3];
	   
	   
	  LPC_GPIO2->FIODIR  &=~(1<<0 | 1<<1 | 1<<2 | 1<<3 ); 
	  if((LPC_GPIO2->FIOPIN & (1<<0) ) == 0)  key=11;    //第1列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<1) ) == 0)  key=12;    //第1列、第2行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<2) ) == 0)  key=13;    //第1列、第3行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<3) ) == 0)  key=14;    //第1列、第4行的按键按下
		
     
      LPC_GPIO2->FIOCLR  = COL[1] | COL[2] | COL[3] | COL[4];    //让四条列线都输出低电平 
    
		myDelay(6);
	return key;
}
 
int main (void)
{    
	 SystemInit();                                                      
     GPIOInit();
	 
	  LPC_GPIO1->FIOSET |= 1<<18;
	  LPC_GPIO1->FIOSET |= 1<<19;
	  LPC_GPIO1->FIOSET |= 1<<20;
	
	 while(1)
	 {
	     if( KEY_Scan() !=0 )    //说明有按键按下
	     {
			    switch(key) //执行不同按键按下时所执行的程序
					{
						case 1  : LPC_GPIO1->FIOCLR |= 1<<18; break;
						case 2  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 3  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 4  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						 
					  case 5  : LPC_GPIO1->FIOCLR |= 1<<18; break;
						case 6  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 7  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 8  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						 
						
						case 9  : LPC_GPIO1->FIOCLR |= 1<<18; break;
						case 10  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 11  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 12  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						 
						
						case 13  : LPC_GPIO1->FIOCLR |= 1<<18; break;
						case 14  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 15  : LPC_GPIO1->FIOCLR |= 1<<19; break;
						case 16 : LPC_GPIO1->FIOCLR |= 1<<19; break;
						 
					
					  default : break;
					}
			 
			    key=0;
			 }
	 
	 }
	 return 0;
 }

