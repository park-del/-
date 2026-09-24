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
#define FPCLK 25000000 
#define  UART_BPS  9600
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
void GPIOInit( void )
{
	  LPC_PINCON->PINSEL0 |= (0x01<<4)|(0x01<<6);  //设置P2.2和P2.3分别为TXD0和REDX0
}	

void UART0_int()
{
   int Fdiv;
	
   LPC_UART0->LCR =0x83;     
   Fdiv = (FPCLK /4 / 16) / UART_BPS;
   LPC_UART0->DLM = Fdiv / 256;
   LPC_UART0->DLL = Fdiv % 256;
   LPC_UART0->LCR =0x03;
}
//*********************************************************************************************************
void UART0_senddata(int data) //查询方式发送8位数据data（所谓查询指的就是通过查发送接收状态寄存器LSR中的值，8位数据也就是一个字节，也就是说每次最多只能发送一个字节的数据）
{
   LPC_UART0->THR=data;                //要发送的数据需要放到寄存器THR中
	 while((LPC_UART0->LSR &0x40 )== 0);      //等待数据发送完毕
}	

int UART0_recedata()
{
   int data;
   while((LPC_UART0->LSR &0x01) == 0 );    //等待接收数据完毕
   data= LPC_UART0->RBR;     
	 return data;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void UART0_GetStr(char *m,int num)
{
	for(;num>0;num--)
	{
		*(m++)=UART0_recedata();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void zhuanhuan(char s1[],int t1,char s2[],int t2,char A1,char A2)
{
	 int i=0,j=0;    int n1=0,n2=0;    int m1=0,m2=0;
     char s1_f[2],s1_d[20],s1_s[20];   char s2_f[2],s2_d[20],s2_s[20]; 
	 double sum1=0;      double sum2=0;
	 double u1=1;        double u2=1;
	 int xx;             int yy;
	 char x[2];          char y[2];
///////////////////////////////////////////////////////////// 
     while( s1[i] != '.')  //最后退出循环时的s1[i]是等于' . '的
	 {
	    n1++; 
		  i++;
	 }
	 
	 s1_f[0]=s1[n1-2];           //将分存放起来（表示分的数字的个数永远只有两个）
	 s1_f[1]=s1[n1-1];

     for(i=0;i<n1-2;i++)        //将度存放起来 （表示度的数字的个数有n1-2个）
		  s1_d[i]=s1[i];

	 for(i=n1+1;i<t1;i++)       //将秒存放起来  （表示秒的数字的个数有m1个）
		  s1_s[m1++]=s1[i];

//下面是发送数据的程序
	 for(i=0;i<n1-2;i++)
	      UART0_senddata(s1_d[i]);    //发送度

	 UART0_senddata(' ');
	 UART0_senddata('d');
	 UART0_senddata(' ');
	 
	 for(i=0;i<2;i++)
		    UART0_senddata(s1_f[i]);     //发送分
	 
	 UART0_senddata(' ');
	 UART0_senddata('f');
     UART0_senddata(' ');
	 
	// for(i=0;i<m1;i++)
	//	 UART0_senddata(s1_s[i]);       //显示秒

	 for(i=0;i<m1;i++)  //40891,第一个字符减去48除以10，第二个字符减去48除以100，……（其实就是依次将小数点左移）
	 {  
		 u1=1;
		 for(j=0;j<i+1;j++)
			  u1=u1*10;
		 sum1+=(s1_s[i]-48)/(u1); 	
	 }
     
	 sum1=sum1*60;
	  
//	UART0_senddata(' ');
	 xx=sum1;      //xx最多是只有两位的，分和秒是最多只有两位的
    
	 x[1]=(xx%10)+48;   //求出的是第一位数
	 x[0]=(xx/10)+48;   //求出的是第二位数
	
	 
	UART0_senddata(' '); 
	UART0_senddata(x[0]);           //发送秒
	UART0_senddata(x[1]);
	UART0_senddata(' ');
	UART0_senddata('s');
	UART0_senddata(' ');
	UART0_senddata(A1);
	UART0_senddata(' ');
	UART0_senddata('\n');
///////////////////////////////////////////////////////////////////////////////////////////////////
     i=0;      j=0;	   
	 while( s2[i] != '.')  //最后退出循环时的s1[i]是等于' . '的
	 {
	    n2++; 
		  i++;
	 }
	 
	 s2_f[0]=s2[n2-2];           //将分存放起来（表示分的数字的个数永远只有两个）
	 s2_f[1]=s2[n2-1];

     for(i=0;i<n2-2;i++)        //将度存放起来 （表示度的数字的个数有n1-2个）
		  s2_d[i]=s2[i];

	 for(i=n2+1;i<t2;i++)       //将秒存放起来  （表示秒的数字的个数有m1个）
		  s2_s[m2++]=s2[i];

//下面是发送数据的程序
	 for(i=0;i<n2-2;i++)
	      UART0_senddata(s2_d[i]);    //发送度

	 UART0_senddata(' ');
	 UART0_senddata('d');
	 UART0_senddata(' ');
	 
	 for(i=0;i<2;i++)
		    UART0_senddata(s2_f[i]);     //发送分
	 
	 UART0_senddata(' ');
	 UART0_senddata('f');
     UART0_senddata(' ');
	 
	// for(i=0;i<m1;i++)
	//	 UART0_senddata(s1_s[i]);       //显示秒的原型

	 for(i=0;i<m2;i++)  //40891,第一个字符减去48除以10，第二个字符减去48除以100，……（其实就是依次将小数点左移）
	 {  
		 u2=1;
		 for(j=0;j<i+1;j++)
			  u2=u2*10;
		 sum2+=(s2_s[i]-48)/(u2); 	
	 }
     
	 sum2=sum2*60;
	  
//	UART0_senddata(' ');
	 yy=sum2;      //xx最多是只有两位的，分和秒是最多只有两位的
    
	 y[1]=(yy%10)+48;   //求出的是第一位数
	 y[0]=(yy/10)+48;   //求出的是第二位数
	
	 
	UART0_senddata(' '); 
	UART0_senddata(y[0]);           //发送秒
	UART0_senddata(y[1]);
	UART0_senddata(' ');
	UART0_senddata('s');
	UART0_senddata(' ');
	UART0_senddata(A2);
	UART0_senddata(' ');
 	UART0_senddata('\n');
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////		 

void sendp_s1_s2(char *p,char s1[],char s2[],char *A1,char *A2,int *t1,int *t2)   //p里面存放的是2513.3072,N,10346.3723,E
{
  int i=0;
  while(p[i]!=',')
     s1[(*t1)++]=p[i++];       //s1存放N或者S的数字字段
  
  *A1=p[i+1];               //A1存放N或者S     
  i=i+3;

  while(p[i]!=',')
      s2[(*t2)++]=p[i++];      //s2存放E或者W的数字字段
  
  *A2=p[i+1];              //A2存放E或者W

  for(i=0;i<(*t1);i++)
     UART0_senddata(s1[i]);     //发送s1存放的数字
   
	 UART0_senddata(*A1);           //发送N或者S
	 UART0_senddata(' ');
	
  for(i=0;i<(*t2);i++)            
     UART0_senddata(s2[i]);     //发送s2存放的数字
	   
  UART0_senddata(*A2);           //发送E或者W
  UART0_senddata(' ');
  UART0_senddata('\n');         //只有没有数据发送时发送的'\n'才有效    
}


 


/////////////////////////////////////////////////////////////////////////////////////////////////////////
void  sendm_p(char m[],char *p)
{
	int i=0,t=0;
	int j,h=0;
	int error[5]={'e','r','r','o','r'};
 /*  从数据报中提取出经纬度信息，16-27为维度信息，也就是将这个信息给提取出来发送出去；29-39为经度信息，也就是将这个信息给提取出来发送出去
    （所有发送的信息是m[16]-m[39]）   */	
	
	while( m[i]!='*' )
	{
	  if(m[i]==',')
          t++;
	  
		if(t==2)
		if(m[i+1]=='V')
			 for(j=0;j<5;j++)
			 {
				  UART0_senddata(error[j]);
				  UART0_senddata('\n');
			    h=1;
			 }
		
		if(h==1)   //发送数据出错的话就跳出这个while循环不再发送
			 break;
		
		if(t==3)
		{   
			 i++; t=0; 
			 while( !(m[i]=='E'||m[i]=='W')  )
			 {
			     p[t++]=m[i++];	
				}				
        p[t]=m[i];	
			  break; 
 		 }
         
         i++;		 
  	}
		   
	 
}

 
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
   char m[70];     char p[20];  char s1[20],s2[20]; int t1=0,t2=0;
   char A1,A2;
   
   SystemInit();                                                      
   GPIOInit(); 
   UART0_int();
	
   LPC_GPIO0->FIOCLR=1<<7;	

   while(1)
   {
		if(1)    //t1与t2是一直加加的，要加上这一句使t1和t2为0，否则发送的过程中可能出现意想不到的错误
		{
			t1=0;  t2=0;
		    A1=0;  A2=0;
		}  
	 UART0_GetStr(m,70);  //每次接收70个字符，即每次接收一个GPS完整的数据报放到数组m中
	 myDelay(1);
	 sendm_p(m,p);     //取出那一串字符放到p中
	 myDelay(1);
	 sendp_s1_s2(p,s1,s2,&A1,&A2,&t1,&t2);  //将p中的那一串字符的数字部分分开放到s1和s2中
	 myDelay(1);
	 zhuanhuan(s1,t1,s2,t2,A1,A2);
	 myDelay(1);  
   }		
   return 0;
}		