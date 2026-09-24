#include "LPC17xx.h"                                                     
#define LCD_RS    (1<<18) 		  
#define LCD_RW    (1<<19)		   
#define LCD_E     (1<<20)		  

#define write 1<<19    //对于rw，0表示写，1表示读
#define read  1<<19
#define command 1<<18  //对应rs，0为指令模式，1为数据模式
#define data 1<<18     



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

 void LCD_init()
{
	void LCD_WriteCommand(uint8_t Code);
	LCD_WriteCommand(LCD_DSPLAYMODE);  //38（指令6）
	 
	LCD_WriteCommand(LCD_DSPLAYOPEN);  //0c（指令4）
	 
	LCD_WriteCommand(LCD_CURSORMOVE);  //06（指令3）
	 
	LCD_WriteCommand(LCD_DSPLAYCLR);   //01（指令1）
   
}

void GPIOInit( void )
{  	
	LPC_GPIO2->FIODIR   |= (255<<0);      //设置P2.0---P2.7为GPIO输出（分别接D0----D7）      
	LPC_GPIO1->FIODIR   |= (7<<18);       //设置P1.18、P1.19、P1.10为GPIO输出（分别接RS,RW,E）
}

/*
   LCD1602中 

 1.rs与rw的搭配

   rs用来选择是指令模式（rs为0）还是数据模式（rs为1）      rw用来选择是写（rw为0）还是读（rw为1）
   注意：写可以是写指令或者是写数据；读是是只能读数据，而不能是读指令的
  
   rw=0; rs=0;  //写指令（发送指令）
   rw=0; rs=1;  //写数据（发送数据）
  
   rw=1; rs=1;  //读数据 （从DDRAM中读数据）

技巧：（1）为了方便观察，可以先写rw，然后再写rs，这样就是先判定是读还是写，然后再判定是数据还是指令，符合我们正常的思维（以读指令为例，rw=1;rs=0;
           即是读指令，刚好按照了“读指令”汉字的顺序）。
      （2）可以用符号常量来进行标识，提高程序的可读性，如对于rs（设定command=0，data=1），对于rw（设定wirte=0，read=1）
            rw=write   rs=command;   //写指令
            rw=wirte   rs=data;      //写数据
            
			rw=read    rs=data;     //读数据

  2.LCD1602指令系统中11条指令与相关存储器（CGRAM和CGROM）的说明
  
  11条指令说明：
  0x38（指令6）   0x0c（指令4）  0x06（指令3）  0x01（指令1）
  说明：上面的这四条指令是用来进行初始化的，因此我们真正需要关心的只有第7、8、10这三条指令
  
  第7条指令则第7位（ 对应了D6 ）为1，这条指令的功能是打开字符发生存贮器CGRAM的（前面第8位为0，后面的第1、……、6位为1或者0都无任何影响）
      即是 0x40、0x41、0x45指的都是第7条指令，均使LCD1602执行打开字符发生存贮器CGRAM的功能，第7位4后面所对应的6位数据为何值无任何意义。
	
  第8条指令则第8位（ 对应了D7 ）为1，这条指令的功能是选中LCD中的某一个位置的，使其可以显示数据（后面的第1、……、7为1或者0都无任何影响）
      
  第10条指令就是写数据或者是写地址的指令。

  
  存储器说明：
  CGRAM为一个随机存储器：是可以读可以写的，PPT中的表格中（PPT 50页）CGRAM的大小为16个存储空间（这16个存储空间所对应的地址分别为0x00-----0x0f），
  CGROM为一个只读存储器：是只能够读而不能够进行写的，CGROM中存放了LCD1602中的系统字符。
  我们可以向CGRAM中写数据，也可以从CGRAM中读数据；我们只能从CGROM中读数据，而不能向CGROM中写数据


  利用第8、10条指令显示系统字符：
  LCD_WriteCommand（ 显示的位置 | 0x80 ）  //第8条指令，控制在LCD1602中显示的位置
  LCD_WritaData（要显示的字符的地址）      //第10条指令，存储器里面的某一个是通过这个字符的地址来读出的，这条指令的意思就是给出某一个字符的地址
  首先确定LCD1602中要显示的位置，然后给出只读存储器CGROM的某一个地址就可以显示该地址处所对应的字符了。


  利用第7、8、10条指令与CGRAM自定义字符：
  <1>
  LCD_Write_Command（0x40）  //第7条指令，打开CGRAM随机存储器
  打开了CGRAM随机存储器后，就可以向其中写数据了，CDRAM有8个空间（对应有8个地址），也即是我们只能向CGRAM中写入8个字符,每个空间对应有两个地址。
  
  <2>
  LCD_Write_Data（data[i]）  //第10条指令，向随机存储器CGRAM中写数据
  每一个字符对应的是一个5列*8行的点阵，连续执行8次第10条指令，即是连续发送8次data（data为1个8位二进制数，两个两位16进制数，每次发送的data只有最
  低5位有效，即是0x1f是一行全亮）就可以点亮一个点阵，也即是定义完成了一个自定义字符，这一个自定义字符（8个8位二进制）会存放在CGRAM的第一个空间中
  （对应了地址为0x00）。依次类推，发送的第二个字符（8次data）会存放在CGRAM的第二个空间中（对应了地址0x01）。

  <3>
  LCD_WriteCommand（ 显示的位置 | 0x80 ）  //第8条指令，控制在LCD1602中显示的位置
  LCD_WritaData（要显示的字符的地址）      //第10条指令，存储器里面的某一个是通过这个字符的地址来读出的，这条指令的意思就是给出某一个字符的地址
  
  第8条指令是控制在LCD1602中显示的位置的，LCD1602总共有16*2=32个位置，这32个位置分别为
  第一行：0x00，0x01，0x02，……，0x0f（16个数）
  第二行：0x40，0x41，0x42，……，0x4f（16个数）
  选中了LCD中的哪个位置则就可以在哪个位置上显示字符了（这个位置上要能显示字符前提rw=read，rs=data，即是要是读数据的模式）

  第10条指令是用来确定已经确定要LCD中进行显示字符的那个位置上具体要显示什么字符，要显示的字符可以是自定义字符（存放在CGRAM中），也可以是系统字符
  （存放在CGROM中）。CGRAM地址对应了0x00---0x0f，CGROM地址对应了0x20-------？？？，两个存储器统一来进行编制，通过给定存储器的某一个地址就可以
   在LCD中要显示的位置上显示你想要的字符了（显示自定义字符就选CGRAM的地址，显示系统字符就选CGROM的地址）。
*/

//发指令
void LCD_WriteCommand(uint8_t Code)
{
	myDelay(5);
	LPC_GPIO1->FIOCLR=write;  //rs=0
	LPC_GPIO1->FIOCLR=command;  //rw=0
	 
	LPC_GPIO2->FIOSET=Code;
    LPC_GPIO1->FIOSET=LCD_E;  //e=1
 	LPC_GPIO1->FIOCLR=LCD_E;  //e=0
	 
	myDelay(5);
	//使默认情况下的
	LPC_GPIO1->FIOSET=read; //rw=1
	LPC_GPIO1->FIOSET=data; //rs=1
  
	LPC_GPIO2->FIOCLR=Code;
	myDelay(5);
}

//发数据
 void LCD_WriteData(uint8_t Data)
{
  myDelay(5);
  LPC_GPIO1->FIOCLR=write;//rw=0;
  LPC_GPIO1->FIOSET=data;//rs=1
  
 
  LPC_GPIO2->FIOSET=Data; 
  LPC_GPIO1->FIOSET=LCD_E;//e=1
  LPC_GPIO1->FIOCLR=LCD_E;//e=0
	 
  myDelay(5);
  LPC_GPIO1->FIOSET=read;//rw=1
  LPC_GPIO1->FIOSET=data;//rs=1
	
  LPC_GPIO2->FIOCLR=Data;
  myDelay(5);
}

void LCD_pos(unsigned char pos)
{
LCD_WriteCommand(pos|0x80);
}


 
int main (void)
 {    
	   unsigned char i=0;
	   uint8_t temp1[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};   //LCD第一行的16个位置
       uint8_t temp2[] = {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};   //LCD第二行的16个位置
	   
	   uint8_t num[10] = {0x31,0x39,0x31,0x30,0x38,0x31,0x38,0x31,0x30,0x31};    //1910818101
	   int name[64]=
	   {    //CGRAM随机存储器中存放的字符
		                 0x00,0x0e,0x0a,0x0e,0x0a,0x0e,0x00,0x00,   //晓的左部         0x00    
		                 0x10,0x1e,0x06,0x0A,0x1e,0x1e,0x0c,0x17,   //晓的右部         0x01
			             0x04,0x1f,0x12,0x0c,0x0c,0x12,0x00,0x00,   //娣的左部         0x02
		                 0x0a,0x1e,0x05,0x1e,0x14,0x1e,0x0d,0x15,   //娣的右部         0x03
			             0x00,0x00,0x08,0x04,0x00,0x08,0x10,0x00,   //冰的左部         0x04
			             0x00,0x04,0x16,0x0c,0x16,0x0d,0x04,0x00,   //冰的右部         0x05
			             0x1f,0x11,0x09,0x05,0x05,0x09,0x83,0x01,   //羽的左部         0x06
			 			 0x1f,0x11,0x09,0x05,0x05,0x09,0x83,0x01    //羽的右部         0x07
	 
	   };
       GPIOInit( );	
	   LCD_init();
	 	
	   //10次for循环显示1910818101
	   for(i=0;i<10;i++)
	   {
	   //显示系统字符，从CGROM只读存储器中读字符
	   LCD_pos(temp2[i]);         //选中LCD中要显示的位置   
	   LCD_WriteData(num[i]);     //选中这个位置上显示的字符（给出CGROM中的一个存储单元的地址，将这个存储单元中的内容给读出来）
	   }
		 
	   LCD_WriteCommand(0x40);    //打开CGRAM随机存储器
	   for(i=0;i<64;i++)
		   LCD_WriteData(name[i]);   //向这个随机存储器CGRAM中写字符（每个字符需要发送8次data，这8个data存放在CGRAM的一个存储单元中
	                                 //,最先发送的存放在CGRAM中地址为0x00的存储单元中、……、最后一个发送的存放在CGRAM中地址为0x0f的存储单元中）
	   //晓的左部
	   LCD_pos(0x00);            //选中LCD中某一个字符要显示的位置
	   LCD_WriteData(0x00);      //通过写随机存储器CGRAM的某一个存储单元的地址来决定要显示的位置上显示CGRAM中这个存储单元的内容
				
	   //晓的右部
	   LCD_pos(0x01);            //选中数据显示的位置
	   LCD_WriteData(0x01);   
	
	   //娣的左部
	   LCD_pos(0x02); 
	   LCD_WriteData(0x02);
		 
	   //娣的右部
	   LCD_pos(0x03); 
	   LCD_WriteData(0x03);
			 
	   //冰的左部
	   LCD_pos(0x04); 
	   LCD_WriteData(0x04);
			
	   //冰的右部
	   LCD_pos(0x05); 
	   LCD_WriteData(0x05);
		 
	   //羽的左部
	   LCD_pos(0x06); 
	   LCD_WriteData(0x06);
			
	   //羽的右部
	   LCD_pos(0x07); 
	   LCD_WriteData(0x07);
		 
	   return 0;
 }