#include "LPC17xx.h"  
#define FPCLK 25000000 
#define  UART_BPS  9600

#define LCD_RS    (1<<18) 		  
#define LCD_RW    (1<<19)		   
#define LCD_E     (1<<20)		  

#define write 1<<19    //对于rw，0表示写，1表示读
#define read  1<<19
#define command 1<<18  //对应rs，0为指令模式，1为数据模式
#define shuju 1<<18     

#define LCD_DSPLAYMODE  0x38     
#define LCD_DSPLAYCLOSE 0x08    
#define LCD_DSPLAYCLR   0x01    
#define LCD_CURSORMOVE  0x06   
#define LCD_DSPLAYOPEN  0x0c   

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
///////////////////////////////////////////////////////////////////////////
void GPIOInit( void )
{
    int i;
	  LPC_GPIO0->FIODIR    |=  (1<<7); //P0.7------------B    //为输出状态
	  LPC_GPIO0->FIODIR    |=  (1<<8); //P0.8------------MR（清零端）
		LPC_GPIO0->FIODIR    |=  (1<<9); //P0.9------------CP（时钟）
	
    LPC_PINCON->PINSEL0  |= (0x01<<4)|(0x01<<6);  //设置P2.2和P2.3分别为TXD0和REDX0	
	
	 	LPC_GPIO1->FIODIR    |= (7<<18);       //设置P1.18、P1.19、P1.10为GPIO输出（分别接RS,RW,E）
	   
	  LPC_GPIO2->FIODIR    |= (255<<0);      //设置P2.0---P2.7为GPIO输出(测试的时候用)
	  
	  for(i=0;i<8;i++)                       //使刚开始时的灯全灭
	    LPC_GPIO2->FIOSET    |=  1<<i;
}
///////////////////////////////////////////////////////////////////////////////////
void RTC_Init (void)
{
    LPC_RTC->CCR   = 0x00;  
    LPC_RTC->ILR   = 0x03; 
    LPC_RTC->CIIR  = 0x01;  //每隔1s触发
    LPC_RTC->AMR   = 0xf8; 
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
void LCD_init()
{
	void LCD_WriteCommand(uint8_t Code);
	LCD_WriteCommand(LCD_DSPLAYMODE);  //38（指令6）
	 
	LCD_WriteCommand(LCD_DSPLAYOPEN);  //0c（指令4）
	 
	LCD_WriteCommand(LCD_CURSORMOVE);  //06（指令3）
	 
	LCD_WriteCommand(LCD_DSPLAYCLR);   //01（指令1）
   
}
///////////////////////////////////////////////////////////////////////////////////
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
}
/////////////////////////////////////////
int UART0_recedata()
{
	  char data;
     while((LPC_UART0->LSR &0x01) == 0 );    //等待接收数据完毕
     data= LPC_UART0->RBR;     
	 return data;
}
void UART0_senddata(int data) //查询方式发送8位数据data（所谓查询指的就是通过查发送接收状态寄存器LSR中的值）
{
   LPC_UART0->THR=data;                //要发送的数据需要放到寄存器THR中
	  
	 while((LPC_UART0->LSR &0x40 )== 0);      //等待数据发送完毕
}	
void UART0_GetStr(char *m,int num)   
{
	for(;num>0;num--)
	  *(m++)=UART0_recedata();
}
////////////////////////////////////////
void LCD_WriteCommand(uint8_t Code)
{
	myDelay(5);
	LPC_GPIO1->FIOCLR=write;  //rs=0
	LPC_GPIO1->FIOCLR=command;  //rw=0
	 
	send_data(Code);
  LPC_GPIO1->FIOSET=LCD_E;  //e=1
 	LPC_GPIO1->FIOCLR=LCD_E;  //e=0
	 
	myDelay(5);
	//使默认情况下的
	LPC_GPIO1->FIOSET=read; //rw=1
	LPC_GPIO1->FIOSET=shuju; //rs=1
  
	myDelay(5);
}

//发数据
 void LCD_WriteData(uint8_t Data)
{
  myDelay(5);
  LPC_GPIO1->FIOCLR=write;//rw=0;
  LPC_GPIO1->FIOSET=shuju;//rs=1
  
 
  send_data(Data); 
  LPC_GPIO1->FIOSET=LCD_E;//e=1
  LPC_GPIO1->FIOCLR=LCD_E;//e=0
	 
  myDelay(5);
  LPC_GPIO1->FIOSET=read;//rw=1
  LPC_GPIO1->FIOSET=shuju;//rs=1
	
  myDelay(5);
}

void LCD_pos(unsigned char pos)
{
LCD_WriteCommand(pos|0x80);
}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void recieve_settime(char a[],char b[],char c[],char h0[],char m0[],char s0[])
{
int i;
for(i=0;i<11;i++)
   UART0_senddata(a[i]);
UART0_GetStr(h0,2);	 
for(i=0;i<2;i++)
	 UART0_senddata(h0[i]);
for(i=0;i<70;i++)
   UART0_senddata(' ');
	  
for(i=0;i<11;i++)
   UART0_senddata(b[i]);
UART0_GetStr(m0,2);
 for(i=0;i<2;i++)
	 UART0_senddata(m0[i]);
for(i=0;i<70;i++)
   UART0_senddata(' ');

for(i=0;i<11;i++)
   UART0_senddata(c[i]);
UART0_GetStr(s0,2);
 for(i=0;i<2;i++)
	 UART0_senddata(s0[i]);
for(i=0;i<70;i++)
   UART0_senddata(' ');
}

void receive_clock(char d[],char e[],char f[],char h1[],char m1[],char s1[])
{
int i;
for(i=0;i<11;i++)
   UART0_senddata(d[i]);
UART0_GetStr(h1,2);
 for(i=0;i<2;i++)
	 UART0_senddata(h1[i]);
for(i=0;i<70;i++)
   UART0_senddata(' ');	 

for(i=0;i<11;i++)
   UART0_senddata(e[i]);
UART0_GetStr(m1,2);
 for(i=0;i<2;i++)
	 UART0_senddata(m1[i]);
for(i=0;i<70;i++)
   UART0_senddata(' ');	 
	 
for(i=0;i<11;i++)
   UART0_senddata(f[i]);
UART0_GetStr(s1,2);
 for(i=0;i<2;i++)
	 UART0_senddata(s1[i]);
for(i=0;i<70;i++)
   UART0_senddata(' ');
}

void transfer(char h[],char m[],char s[],int *h_,int *m_,int *s_)
{
 *h_=(int)( h[0]-48)*10+(int)(h[1]-48);
 *m_=(int)( m[0]-48)*10+(int)(m[1]-48);
 *s_=(int)( s[0]-48)*10+(int)(s[1]-48);
}

void RTC_time(int h_0,int m_0,int s_0)
{
 LPC_RTC->SEC   = s_0;   //设置RTC初始时开始计时的时间
 LPC_RTC->MIN   = m_0;
 LPC_RTC->HOUR  = h_0;
}

void RTC_clock(int h_1,int m_1,int s_1)
{
LPC_RTC->ALSEC =  s_1;        //设置RTC报警时的时间 
LPC_RTC->ALMIN =  m_1;
LPC_RTC->ALHOUR = h_1;
}
void read_time(int *h,int *m,int *s)
{
     *s=LPC_RTC->SEC;     //读取此时的时间
     *m=LPC_RTC->MIN;  
     *h=LPC_RTC->HOUR ;

}
void show_hms(char warn[],char warn_pos[])
{
 int i;
 LCD_pos(0x49);  //显示h
 LCD_WriteData(0x68);
  
 LCD_pos(0x4c);  //显示m
 LCD_WriteData(0x6d);
 
 LCD_pos(0x4f);  //显示s
 LCD_WriteData(0x73);
 
 for(i=0;i<5;i++)   //显示警告标志
 {
   LCD_pos(warn_pos[i]);
   LCD_WriteData(warn[i]);
 }
 
 LCD_pos(0x07);  //显示h
 LCD_WriteData(0x68);
 
 LCD_pos(0x0d);  //显示s
 LCD_WriteData(0x73);
 
 LCD_pos(0x0a);  //显示m
 LCD_WriteData(0x6d);
}

void show_shuzi(char pos[],int s0_1,int s0_2,int m0_1,int m0_2,int h0_1,int h0_2)
{
 int i=0;
 LCD_pos(pos[i++]);
 switch(s0_1)
 {
   case 0:  LCD_WriteData(0x30); break;
	 case 1:  LCD_WriteData(0x31); break;
	 case 2:  LCD_WriteData(0x32); break;
	 case 3:  LCD_WriteData(0x33); break;
	 case 4:  LCD_WriteData(0x34); break;
	 case 5:  LCD_WriteData(0x35); break;
	 case 6:  LCD_WriteData(0x36); break;
	 case 7:  LCD_WriteData(0x37); break;
	 case 8:  LCD_WriteData(0x38); break;
   case 9:  LCD_WriteData(0x39); break;
 }
 
 LCD_pos(pos[i++]);
 switch(s0_2)
 {
   case 0:  LCD_WriteData(0x30); break;
	 case 1:  LCD_WriteData(0x31); break;
	 case 2:  LCD_WriteData(0x32); break;
	 case 3:  LCD_WriteData(0x33); break;
	 case 4:  LCD_WriteData(0x34); break;
	 case 5:  LCD_WriteData(0x35); break;
	 case 6:  LCD_WriteData(0x36); break;
	 case 7:  LCD_WriteData(0x37); break;
	 case 8:  LCD_WriteData(0x38); break;
   case 9:  LCD_WriteData(0x39); break;
 }
 LCD_pos(pos[i++]);
 switch(m0_1)
 {
   case 0:  LCD_WriteData(0x30); break;
	 case 1:  LCD_WriteData(0x31); break;
	 case 2:  LCD_WriteData(0x32); break;
	 case 3:  LCD_WriteData(0x33); break;
	 case 4:  LCD_WriteData(0x34); break;
	 case 5:  LCD_WriteData(0x35); break;
	 case 6:  LCD_WriteData(0x36); break;
	 case 7:  LCD_WriteData(0x37); break;
	 case 8:  LCD_WriteData(0x38); break;
   case 9:  LCD_WriteData(0x39); break;
 }
 
 LCD_pos(pos[i++]);
 switch(m0_2)
 {
   case 0:  LCD_WriteData(0x30); break;
	 case 1:  LCD_WriteData(0x31); break;
	 case 2:  LCD_WriteData(0x32); break;
	 case 3:  LCD_WriteData(0x33); break;
	 case 4:  LCD_WriteData(0x34); break;
	 case 5:  LCD_WriteData(0x35); break;
	 case 6:  LCD_WriteData(0x36); break;
	 case 7:  LCD_WriteData(0x37); break;
	 case 8:  LCD_WriteData(0x38); break;
   case 9:  LCD_WriteData(0x39); break;
 }
 
 LCD_pos(pos[i++]);
 switch(h0_1)
 {
   case 0:  LCD_WriteData(0x30); break;
	 case 1:  LCD_WriteData(0x31); break;
	 case 2:  LCD_WriteData(0x32); break;
	 case 3:  LCD_WriteData(0x33); break;
	 case 4:  LCD_WriteData(0x34); break;
	 case 5:  LCD_WriteData(0x35); break;
	 case 6:  LCD_WriteData(0x36); break;
	 case 7:  LCD_WriteData(0x37); break;
	 case 8:  LCD_WriteData(0x38); break;
   case 9:  LCD_WriteData(0x39); break;
 }
 
 LCD_pos(pos[i++]);
 switch(h0_2)
 {
   case 0:  LCD_WriteData(0x30); break;
	 case 1:  LCD_WriteData(0x31); break;
	 case 2:  LCD_WriteData(0x32); break;
	 case 3:  LCD_WriteData(0x33); break;
	 case 4:  LCD_WriteData(0x34); break;
	 case 5:  LCD_WriteData(0x35); break;
	 case 6:  LCD_WriteData(0x36); break;
	 case 7:  LCD_WriteData(0x37); break;
	 case 8:  LCD_WriteData(0x38); break;
   case 9:  LCD_WriteData(0x39); break;
 }
}

//RTC中断所需要用到的一些变量
int s,m,h;
int s11,s22,m11,m22,h11,h22;
char current_pos[]={0x4e,0x4d,0x4b,0x4a,0x48,0x47};
int a0,a1,a2;
void  RTC_IRQHandler(void)
{
	  int i;
   if( (LPC_RTC->ILR & 0x1) == 1) //说明触发了报警中断
	 {
	   read_time(&h,&m,&s);  //读取当前时间分别赋值给h,m,s
		 
		 s11=s%10;
	   s22=(s-s11)/10;
		 
		 m11=m%10;
	   m22=(m-m11)/10;
		 
		 h11=h%10;
	   h22=(h-h11)/10;
		 
		 show_shuzi(current_pos,s11,s22,m11,m22,h11,h22);
	 }
	 
	 if( (LPC_RTC->ILR & 0x2) == 2) //触发了报警中断
	 {
	 for(i=0;i<5;i++)
				{
				LPC_GPIO2->FIOCLR |= (1<<0);   //由于小灯泡是低电平点亮，这一段程序是使小灯泡亮的
        LPC_GPIO2->FIOCLR |= (1<<7);
				myDelay(500);
					
        LPC_GPIO2->FIOCLR |= (1<<1);
        LPC_GPIO2->FIOCLR |= (1<<6);						
			 myDelay(500);
					
			  LPC_GPIO2->FIOCLR |= (1<<2);
        LPC_GPIO2->FIOCLR |= (1<<5);						
			  myDelay(500);
					
        LPC_GPIO2->FIOCLR |= (1<<3);
        LPC_GPIO2->FIOCLR |= (1<<4);						
			 myDelay(500);
					
			 
			  LPC_GPIO2->FIOSET |= (1<<0);   //由于小灯泡是低电平点亮，这一段程序是使小灯泡亮的
        LPC_GPIO2->FIOSET |= (1<<7);
				myDelay(500);
					
        LPC_GPIO2->FIOSET |= (1<<1);
        LPC_GPIO2->FIOSET |= (1<<6);						
			  myDelay(500);
					
			  LPC_GPIO2->FIOSET |= (1<<2);
        LPC_GPIO2->FIOSET |= (1<<5);						
			 myDelay(500);
					
        LPC_GPIO2->FIOSET |= (1<<3);
        LPC_GPIO2->FIOSET |= (1<<4);						
			 myDelay(500);
				    	    
				}
		 
	 RTC_time(a0,a1,a2);
	 }
	 LPC_RTC->ILR = 0x03; //清除中断标志位
}
int main (void)
{  
	int i; 
	
	char h0[10]; char m0[10]; char s0[10];  //设置初始时间
    char a[]={'s','e','t','-','h','o','u','r',':',' '};
	char b[]={'s','e','t','-','m','i','n',':',' ',' '}; 
    char c[]={'s','e','t','-','s','e','c',':',' ',' '};
	
	char h1[10]; char m1[10]; char s1[10];  //设置报警时间
	char d[]={'w','a','r','n','-','h','o','u','r',':'};
	char e[]={'w','a','r','n','-','m','i','n',':',' '};
	char f[]={'w','a','r','n','-','s','e','c',':',' '};
    char warn[]={'w','a','r','n',':'},warn_pos[]={0x00,0x01,0x02,0x03,0x04};
	
	int h_0,m_0,s_0;//存放转换为10进制后设定时间的时分秒	
    int h_1,m_1,s_1;
	int s0_1,s0_2,m0_1,m0_2,h0_1,h0_2; //设定初始时间（分别得到时分秒的每一位数）
    int s1_1,s1_2,m1_1,m1_2,h1_1,h1_2; //设定报警时间
		
	char time_pos[]={0x4e,0x4d,0x4b,0x4a,0x48,0x47};
	char clock_pos[]={0x0c,0x0b,0x09,0x08,0x06,0x05};	
	
    SystemInit();                                                      
    GPIOInit();
	UART0_int();
	LCD_init();
    RTC_Init();
	
	NVIC_EnableIRQ(RTC_IRQn);     
    NVIC_SetPriority(RTC_IRQn, 4); 
   
	recieve_settime(a,b,c,h0,m0,s0);   //板子发送a、b、c数组内容给电脑，同时将接收到的小时、分钟、秒数（均是字符型）分别存放在数组h0、m0、s0中 (初始的时间)
    receive_clock(d,e,f,h1,m1,s1);     //板子发送d、e、f数组内容给电脑，同时将接收到的小时、分钟、秒数（均是字符型）分别存放在数组h1、m1、s1中（闹钟的时间）
 
	transfer(h0,m0,s0,&h_0,&m_0,&s_0); //将数组h0、m0、s0中存放字符（字符型的时间）转化为h_0,m_0,s_0（整型的时间）   （初始的时间）
	transfer(h1,m1,s1,&h_1,&m_1,&s_1); //将数组h1、m1、s1中存放字符（字符型的时间）转化为h_1,m_1,s_1（整型的时间）   （闹钟的时间）
	
	RTC_time(h_0,m_0,s_0);             //设置RTC初始时的时间
	RTC_clock(h_1,m_1,s_1);            //设置RTC报警的时间
 
    show_hms(warn,warn_pos);           //在LCD上显示h、m、s、warn


////////////////////////////////////////	 
//求出初始时间的时分秒的个位十位
	s0_1=s_0%10;               //秒的个位数
    s0_2=(s_0-s0_1)/10;        //秒的十位数
 
    m0_1=m_0%10;               //分的个位数
    m0_2=(m_0-m0_1)/10;        //分的十位数
 
    h0_1=h_0%10;               //时的个位数
    h0_2=(h_0-h0_1)/10;        //时的十位数
	
//求出闹钟时间的时分秒的个位十位	
   s1_1=s_1%10;               //秒的个位数
   s1_2=(s_1-s1_1)/10;        //秒的十位数
 
   m1_1=m_1%10;               //分的个位数
   m1_2=(m_1-m1_1)/10;        //分的十位数
 
   h1_1=h_1%10;               //时的个位数
   h1_2=(h_1-h1_1)/10;        //时的十位数
 //////////////////////////////////////// 
 show_shuzi(time_pos,s0_1,s0_2,m0_1,m0_2,h0_1,h0_2);
 show_shuzi(clock_pos,s1_1,s1_2,m1_1,m1_2,h1_1,h1_2);
 
 a0=h_0;
 a1=m_0;
 a2=s_0;
 LPC_RTC->CCR   = 0x01;  
 while(1);
  
				
}
