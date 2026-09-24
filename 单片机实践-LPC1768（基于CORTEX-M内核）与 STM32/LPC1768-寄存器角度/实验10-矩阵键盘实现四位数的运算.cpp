#include "LPC17xx.h"                                                     
#define shuzi_0    0x3f        //这是显示数字的宏定义
#define shuzi_1    0x06
#define shuzi_2    0x5b
#define shuzi_3    0x4f
#define shuzi_4    0x66
#define shuzi_5    0x6d
#define shuzi_6    0x7d
#define shuzi_7    0x07
#define shuzi_8    0x7f
#define shuzi_9    0x6f
///////////////////////////////////////////////
//进行数码管的位选并显示数字
void xuan(int j,int m)
{
	   if(j==1)//选中第1位数码管
	  {
		LPC_GPIO1->FIOSET |= 1<<8; 
        LPC_GPIO1->FIOCLR |= 1<<9 | 1<<10;
        LPC_GPIO1->FIOCLR |= 1<<14;	
	     	
        LPC_GPIO2->FIOCLR  |= m;
	   }
		if(j==2)//选中第2位数码管
	  {
		 LPC_GPIO1->FIOSET |= 1<<9; 
         LPC_GPIO1->FIOCLR |= 1<<8 | 1<<10;
         LPC_GPIO1->FIOCLR |= 1<<14;
	
         LPC_GPIO2->FIOCLR  |= m; 
	   }
		if(j==3)//选中第3位数码管
	  {
		 LPC_GPIO1->FIOSET |= 1<<10; 
         LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9;
         LPC_GPIO1->FIOCLR |= 1<<14;
	
         LPC_GPIO2->FIOCLR  |= m; 
	   }
		if(j==4)//选中第4位数码管
	  {
        LPC_GPIO1->FIOSET|= 1<<14; 
        LPC_GPIO1->FIOCLR |= 1<<8 | 1<<9 | 1<<10;
	
        LPC_GPIO2->FIOCLR  |= m; 
	   }
}

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

void led_off()
{
   LPC_GPIO2->FIOSET |=0x7f;
}
                     

///////////////////////////////////////////////////////
//进行动态扫描
void dongtai(int s1[5])    
{
	     int k,i;
	     for(k=0;k<1000;k++)
		{
			LPC_GPIO1->FIOSET |= 1<<8; 
			LPC_GPIO2->FIOCLR |= s1[1];		
			myDelay(10);
			
			LPC_GPIO1->FIOCLR |= 1<<8;       //每次位选前清除之前的位选和亮的数字
			LPC_GPIO2->FIOSET |=s1[1];	
			LPC_GPIO1->FIOSET |= 1<<9;
			LPC_GPIO2->FIOCLR |=s1[2] ;
			myDelay(10);
				
			LPC_GPIO1->FIOCLR |= 1<<9;
			LPC_GPIO2->FIOSET |=s1[2] ;	 
			LPC_GPIO1->FIOSET |= 1<<10;
			LPC_GPIO2->FIOCLR |=s1[3];
			myDelay(10);
			
			LPC_GPIO1->FIOCLR |= 1<<10;
			LPC_GPIO2->FIOSET |=s1[3] ;	  
			LPC_GPIO1->FIOSET |= 1<<14;
			LPC_GPIO2->FIOCLR |=s1[4];
			myDelay(10);
				
			LPC_GPIO1->FIOCLR |= 1<<14;
			LPC_GPIO2->FIOSET |=s1[4] ;	
		 }
}

int ROW[]={ 1<<0,1<<7,1<<8,1<<9,1<<5};     //有四行，分别用ROW[1]------ROW[4]来表示，接P2.7-P2.10
int COL[]={ 1<<0,1<<22,1<<23,1<<24,1<<25};  //有四列，分别用COL[1]------COL[4]来表示，接P1.22-P1.25             
uint8_t key=0;	
int k=0; 
int a1[5]={0,0,0,0,0};  //两个操作数
int a2[5]={0,0,0,0,0};
int xy=0,s[5],w[5],z;
int q,p,t1[5]={0,0,0,0,0},t2[5]={0,0,0,0,0},sum; 
int t;
 
///////////////////////////////////////////
//对引脚初始化
void GPIOInit( void )   
{  	
	 //初始化
	 int ROW[5]={ 1<<0,1<<7,1<<8,1<<9,1<<5};     //有四行，分别用ROW[1]------ROW[4]来表示，接P2.7-P2.9、P0.5
     int COL[5]={ 1<<0,1<<22,1<<23,1<<24,1<<25};  //有四列，分别用COL[1]------COL[4]来表示，接P1.22-P1.25     

	  //设置P2.7-P2.10为GPIO输出（其中R1---R4分别接P2.7-P2.9和P0.5；其中L1---L4分别接P1.22-P1.25）
	 LPC_GPIO2->FIODIR  |= (1<<7  | 1<<8 | 1<<9 );   //行输出模式
     LPC_GPIO0->FIODIR  |= (1<<5); 
	 LPC_GPIO1->FIODIR  |= (1<<22 | 1<<23 | 1<<24 | 1<<25); //列输出模式

	 LPC_GPIO2->FIODIR    |=  (1<<0| 1<<1| 1<<2| 1<<3| 1<<4| 1<<5| 1<<6);  //数码管段选P2.0-----P2.6为GPIO输出
     LPC_GPIO1->FIODIR    |=  (1<<8| 1<<9| 1<<10| 1<<14); //P1.8--------P1.10，P1.14为位选数码管
}


/*
采用行扫描的方式，即是   
                       （1） 让四条行线初始输出低电平
                       （2） 第1条列线输出低电平，其余3条输出高电平，检验看是否有某一行为低电平
                       （3） 第2条列线输出低电平，其余3条输出高电平，检验看是否有某一行为低电平
                       （4） 第3条列线输出低电平，其余3条输出高电平，检验看是否有某一行为低电平
*/

////////////////////////////////////
 //行输出模式  
void INIThang(void)   
{
	  LPC_GPIO2->FIODIR  |= (1<<7  | 1<<8 | 1<<9 );  
	  LPC_GPIO0->FIODIR  |= (1<<5);
  	  LPC_GPIO2->FIOSET = ROW[1] | ROW[2]| ROW[3] | ROW[4];    //让四条行线都输出模式，输出高电平 
	  LPC_GPIO0->FIOSET = ROW[4]; 
}

///////////////////////////////////////////////////////
//列扫描法
uint8_t  KEY_Scan(void)
{	
	 
    //-------------- 扫描第1列 --------------
/*
  让1号列线先输出低电平，其他3条列线输出高电平，然后依次检验1--4这四条行线，看是否有一条行线输出低电平
  通过 LPC_GPIO2->FIOPIN 来判断P2的各个引脚的电平情况，按键按下去会将该按键对应的行线与列线短接，行线会输出相应的低电平
*/
	  INIThang( );
	  LPC_GPIO1->FIOCLR  = COL[1];                     //第1列输出低电平，第2、3、4列输出高电平
	  LPC_GPIO1->FIOSET  = COL[2] | COL[3] | COL[4];
	   
	  
	  LPC_GPIO2->FIODIR  &=~(1<<7  | 1<<8 | 1<<9 );   //让四条行线都为输入模式
	  LPC_GPIO0->FIODIR  &=~(1<<5);  
	
	  if((LPC_GPIO2->FIOPIN & (1<<7) ) == 0)  key=1;         //第1列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<8) ) == 0)  key=2;         //第1列、第2行的按键按下
      if((LPC_GPIO2->FIOPIN & (1<<9) ) == 0)  key=3;         //第1列、第3行的按键按下
	  if((LPC_GPIO0->FIOPIN & (1<<5) ) == 0)  key=4;         //第1列、第4行的按键按下

    //-------------- 扫描第2列 --------------
 
	  INIThang( );
		
	  LPC_GPIO1->FIOCLR  |= COL[2];                  //第2列输出低电平，第1、3、4列输出高电平
	  LPC_GPIO1->FIOSET  |= COL[1] | COL[3] | COL[4];
	    
			
	  LPC_GPIO2->FIODIR  &=~(1<<7  | 1<<8 | 1<<9 );   //让四条行线都为输入模式
	  LPC_GPIO0->FIODIR  &=~(1<<5);  
	  if((LPC_GPIO2->FIOPIN & (1<<7) ) == 0)  key=5;    //第2列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<8) ) == 0)  key=6;    //第2列、第2行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<9) ) == 0)  key=7;    //第2列、第3行的按键按下
	  if((LPC_GPIO0->FIOPIN & (1<<5) ) == 0)  key=8;    //第2列、第4行的按键按下

    //-------------- 扫描第3列 --------------
		 
	  INIThang( );
			
	  LPC_GPIO1->FIOCLR   = COL[3];               //第3列输出低电平，第1、2、4列输出高电平
	  LPC_GPIO1->FIOSET  = COL[1] | COL[2] | COL[4];
	   
	    
			
	  LPC_GPIO2->FIODIR  &=~(1<<7  | 1<<8 | 1<<9 );   //让四条行线都为输入模式
	  LPC_GPIO0->FIODIR  &=~(1<<5);  
	  if((LPC_GPIO2->FIOPIN & (1<<7) ) == 0)  key=9;     //第1列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<8) ) == 0)  key=10;    //第1列、第2行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<9) ) == 0)  key=11;    //第1列、第3行的按键按下
	  if((LPC_GPIO0->FIOPIN & (1<<5) ) == 0)  key=12;    //第1列、第4行的按键按下
		
		//-------------- 扫描第4列 --------------
		 
	  INIThang( );
			
	  LPC_GPIO1->FIOCLR  = COL[4];           //第4列输出低电平，第1、2、3列输出高电平
	  LPC_GPIO1->FIOSET  = COL[1] | COL[2] | COL[3];
	   
	   
	  LPC_GPIO2->FIODIR  &=~(1<<7  | 1<<8 | 1<<9 );   //让四条行线都为输入模式
	  LPC_GPIO0->FIODIR  &=~(1<<5);  
	  if((LPC_GPIO2->FIOPIN & (1<<7) ) == 0)  key=13;    //第1列、第1行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<8) ) == 0)  key=14;    //第1列、第2行的按键按下
	  if((LPC_GPIO2->FIOPIN & (1<<9) ) == 0)  key=15;    //第1列、第3行的按键按下
	  if((LPC_GPIO0->FIOPIN & (1<<5) ) == 0)  key=16;    //第1列、第4行的按键按下
		
     
     LPC_GPIO2->FIOCLR  = COL[1] | COL[2] | COL[3] | COL[4];    //让四条列线都输出低电平 
    
		myDelay(6);
	return key;
}



void jisuan(int t)
{
	{
			                    										
			              q=a1[1]*1000+a1[2]*100+a1[3]*10+a1[4];  //第一个操作数
						  p=a2[1]*1000+a2[2]*100+a2[3]*10+a2[4];  //第二个操作数
		                  if(t==1) sum=q+p;     //加法
						  if(t==2) sum=q-p;     //减法
		                  if(t==3) sum=q*p;     //乘法
	                      if(t==4) sum=q/p;     //除法
		
			              t1[1]=sum/1000;                             //千位
		          
		                  if(t1[1]>=10)
                             t1[1]=t1[2]=t1[3]=t1[4]=0; 
						  else
						  {
							 t1[2]=(sum-t1[1]*1000)/100;              //百位
							 t1[3]=(sum-(t1[1]*10+t1[2])*100)/10;     //十位
							 t1[4]=sum-(t1[1]*100+t1[2]*10+t1[3])*10; //个位
						  }			
										  
					       switch( t1[1] )   //将算得的千位、百位、十位、个位这四个数分别转换为相应的段码
		                   {
	                           case 0: t2[1]=shuzi_0;break;
			                   case 1: t2[1]=shuzi_1;break;  
			                   case 2: t2[1]=shuzi_2;break;   
			                   case 3: t2[1]=shuzi_3;break;  
			                   case 4: t2[1]=shuzi_4;break;  
			                   case 5: t2[1]=shuzi_5;break;  
			                   case 6: t2[1]=shuzi_6;break;   
			                   case 7: t2[1]=shuzi_7;break;  
			                   case 8: t2[1]=shuzi_8;break;   
			                   case 9: t2[1]=shuzi_9;break; 
			                   default :  break;
		                   }
						 
						   switch( t1[2] )
		                   {
	                           case 0: t2[2]=shuzi_0;break;
			                   case 1: t2[2]=shuzi_1;break;  
			                   case 2: t2[2]=shuzi_2;break;   
			                   case 3: t2[2]=shuzi_3;break;  
			                   case 4: t2[2]=shuzi_4;break;  
			                   case 5: t2[2]=shuzi_5;break;  
			                   case 6: t2[2]=shuzi_6;break;   
			                   case 7: t2[2]=shuzi_7;break;  
			                   case 8: t2[2]=shuzi_8;break;   
			                   case 9: t2[2]=shuzi_9;break; 
			                   default :  break;
		                   }
						  
						   switch( t1[3] )
		                   {
	                       case 0: t2[3]=shuzi_0;break;
			                   case 1: t2[3]=shuzi_1;break;  
			                   case 2: t2[3]=shuzi_2;break;   
			                   case 3: t2[3]=shuzi_3;break;  
			                   case 4: t2[3]=shuzi_4;break;  
			                   case 5: t2[3]=shuzi_5;break;  
			                   case 6: t2[3]=shuzi_6;break;   
			                   case 7: t2[3]=shuzi_7;break;  
			                   case 8: t2[3]=shuzi_8;break;   
			                   case 9: t2[3]=shuzi_9;break; 
			                   default :  break;
		                   }
						
						   switch( t1[4] )
		                   {
	                          case 0: t2[4]=shuzi_0;break;
			                   case 1: t2[4]=shuzi_1;break;  
			                   case 2: t2[4]=shuzi_2;break;   
			                   case 3: t2[4]=shuzi_3;break;  
			                   case 4: t2[4]=shuzi_4;break;  
			                   case 5: t2[4]=shuzi_5;break;  
			                   case 6: t2[4]=shuzi_6;break;   
			                   case 7: t2[4]=shuzi_7;break;  
			                   case 8: t2[4]=shuzi_8;break;   
			                   case 9: t2[4]=shuzi_9;break; 
			                   default :  break;
		                   }
							
						   dongtai(t2);   //动态扫描t2就行了
						   myDelay(2000); 
	}									
}
	////////////////////////////////////////////////////////////////
void input1(int i)
{
	    while(xy==0)
		{
	     if( KEY_Scan() !=0  )    //说明有按键按下
	     {
			    switch(key) //执行不同按键按下时所执行的程序
					{
						//第一列     //在相应的位置的数码管显示位数
						case 1  : led_off();xuan(i,shuzi_1); w[i]=shuzi_1;a1[i]=1; myDelay(2000); xy=1;break;   //第1行
						case 2  : led_off();xuan(i,shuzi_2); w[i]=shuzi_2;a1[i]=2; myDelay(2000); xy=1;break;   //第2行       
						case 3  : led_off();xuan(i,shuzi_3); w[i]=shuzi_3;a1[i]=3; myDelay(2000); xy=1;break;   //第3行
						case 4  : led_off();xuan(i,shuzi_4); w[i]=shuzi_4;a1[i]=4; myDelay(2000); xy=1;break;   //第4行
						
						//第二列
					  case 5  : led_off();xuan(i,shuzi_5);   w[i]=shuzi_5;a1[i]=5; myDelay(2000); xy=1;break;
						case 6  : led_off();xuan(i,shuzi_6); w[i]=shuzi_6;a1[i]=6; myDelay(2000); xy=1;break;
						case 7  : led_off();xuan(i,shuzi_7); w[i]=shuzi_7;a1[i]=7; myDelay(2000); xy=1;break;
						case 8  : led_off();xuan(i,shuzi_8); w[i]=shuzi_8;a1[i]=8; myDelay(2000); xy=1;break;
						 
						//第三列
						case 9   :led_off();xuan(i,shuzi_9); w[i]=shuzi_9;a1[i]=9; myDelay(2000); xy=1;break;
						case 10  :led_off();xuan(i,shuzi_0); w[i]=shuzi_0;a1[i]=0; myDelay(2000); xy=1;break;
						case 11  :     dongtai(w); xy=1;break;                     							
						case 12  :
		 			
					  default : break;
					}	 
			       key=0;
		 }		
		} 
	 xy=0;	 
}

void input2(int i)
{
	    while(xy==0)
	 {
	     if( KEY_Scan() !=0  )    //说明有按键按下
	     {
			    switch(key) //执行不同按键按下时所执行的程序
					{
						//第一列     //在相应的位置的数码管显示位数
						case 1  : led_off();xuan(i,shuzi_1); s[i]=shuzi_1;a2[i]=1; myDelay(2000); xy=1;break;   //第1行
						case 2  : led_off();xuan(i,shuzi_2); s[i]=shuzi_2;a2[i]=2; myDelay(2000); xy=1;break;   //第2行       
						case 3  : led_off();xuan(i,shuzi_3); s[i]=shuzi_3;a2[i]=3; myDelay(2000); xy=1;break;   //第3行
						case 4  : led_off();xuan(i,shuzi_4); s[i]=shuzi_4;a2[i]=4; myDelay(2000); xy=1;break;   //第4行
						
						//第二列
					  case 5  : led_off();xuan(i,shuzi_5); s[i]=shuzi_5;a2[i]=5; myDelay(2000); xy=1;break;
						case 6  : led_off();xuan(i,shuzi_6); s[i]=shuzi_6;a2[i]=6; myDelay(2000); xy=1;break;
						case 7  : led_off();xuan(i,shuzi_7); s[i]=shuzi_7;a2[i]=7; myDelay(2000); xy=1;break;
						case 8  : led_off();xuan(i,shuzi_8); s[i]=shuzi_8;a2[i]=8; myDelay(2000); xy=1;break;
						 
						//第三列
						case 9   :led_off();xuan(i,shuzi_9); s[i]=shuzi_9;a2[i]=9; myDelay(2000); xy=1;break;
						case 10  :led_off();xuan(i,shuzi_0); s[i]=shuzi_0;a2[i]=0; myDelay(2000); xy=1;break;
						case 11  :   dongtai(s); xy=1;break;                     							
						case 12  :
		 			
					  default : break;
					}	 
			       key=0;
			  }		
		 } 
	 xy=0;	 
}
void input3(int i)
{
	int x;
	    while(xy==0)
	 {
	     if( KEY_Scan() !=0  )    //说明有按键按下
	     {
			    switch(key) //执行不同按键按下时所执行的程序
					{
						 
						//第四列
						case 13  : x=1; jisuan(x);break;//四位数加法
							      									 
						case 14  : x=2; jisuan(x);break;//四位数减法
							      
						case 15  : x=3; jisuan(x);break;//四位数乘法
							       
						case 16  : x=4; jisuan(x);break;//四位数除法
							        			
					  default : break;
					}	 
			       key=0;
			  }		
		 } 
}
//////////////////////////////////////////////////////
//主函数
int main (void)
{    
	SystemInit();                                                      
    GPIOInit();
	for(z=1;z<=5;z++)
	{
		input1(z);    //依次执行input1(1)---input1(4),输入第1个操作数的四位数;input1(5)进行动态扫描第一个操作数
	}
		 
    for(z=1;z<=5;z++)     //依次执行intput2(1)---input2(4),输入第2个操作数的四位数;input2(5)进行动态扫描第二个操作数
	{
		input2(z);
	} 
    for(z=1;z<=10;z++)
	{
		input3(z);
	}  
	return 0;
}
