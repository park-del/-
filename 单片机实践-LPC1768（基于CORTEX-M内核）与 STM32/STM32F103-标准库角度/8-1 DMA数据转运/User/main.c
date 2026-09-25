#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

uint8_t aa1 = 0x66;         //存储在    RAM区
const uint8_t bb1 = 0x66;   //存储在    程序flash区
static uint8_t cc1 = 0x88;  //存储在    RAM区
/*
总结：见操作系统中内存映射部分，
只读代码区包含了  代码 和 常量（const全局 和 字符串常量）
读写数据段存放    全局变量 和 静态变量

在计算机中，上面都是存放在内存中的，而在stm32中，只读代码区为flash，其他在运行内存SRAM中。

注意：
1、使用 const修饰的局部变量 属于 函数栈的一部分，理解为 一个不会改变值的存储空间。
2、在C++中，由于存储完 .text只读机器代码后，接着存储 .rodata只读数据，这些是只读代码段，
   但是紧接着存储 .data（已经初始化的全局变量和静态变量）和.bss（未初始化的全局变量和静态变量）
   。因此，.rodata区应该是和读写数据段（存放全局和静态变量）紧挨着的，这也是为什么在黑马的视频
   中，const修饰的全局只读数据（常量）的存储地址和全局或静态的存储地址挨着非常近。
3、static修饰的局部变量也是存储在 读写数据段的。

4、.rodata .data .bss 这些事先都是可以知道的。

5、 #define 预定义的常量 不会在内存中分配空间，而是在 预处理阶段直接替换成对应的数字，然后进入
    编译阶段。
	
	预处理 ---> 编译 ---> 链接 ---> 运行（装入）

编译将各个模块变成机器指令，链接将各个模块组装成一个完整的模块（一个完整逻辑地址），装入（即运行
，装入时才会分配程序需要的内存的（包含栈区、堆区、只读代码段、读写数据段等），此时分配的就包含
.text .rodata .data .bss 这些运行前就已经知道要分配多大的，分配后一直保持不变，不再动态改变内存
，而栈区和堆区在程序运行时是可以动态改变大小。）
*/
uint8_t DataA[] = {0x01,0x02,0x03,0x04};
uint8_t DataB[] = {0,0,0,0};

int main(void){

/*
寄存器是存储器的一种，这样计组里面的寄存器和内存统一编址就解释的通了（此时可以将存储器看成一个整体，有些部分化为运行内存，有些部分划分为外设寄存器，这是为什么叫做存储器映像）。
对外设的操作就是对寄存器的操作，本质上也是对存储器的操作。

stm32中的寄存器一般是32位的，而一个外设一般是对应有多个寄存器的，故我们只要知道外设寄存器在存储器中的首地址，就可以对应到某一个寄存器的地址，接着对应到该寄存器中某一个字节的地址。寄存器是一种特殊的存储器，因为相比较于普通的内存单元，寄存器中的每一位背后都连接了一根导线，这些导线可以控制外设电路的状态，比如置引脚的高低电平、导通和断开开关、切换数据选择器 或者 寄存器中的多位结合让该寄存器成为一个计数器、数据寄存器等等。
寄存器是连接软件和硬件的桥梁，软件读写寄存器，就相当于是在控制硬件的执行。
DMA数据转运：从存储器中的某一个地址取内容再放到另外一个地址中去。
DMA通道：每个通道都可以独立互相不影响地完成数据转运，各个通道都是有一组配套的寄存器。
DMA请求：DMA请求就是DMA的硬件触发源，比如ADC转换完成，串口接收到数据。
AHB从设备：DMA框图中有个DMA从设备，由于DMA也是为一个外设，故也有其对应的外设寄存器，对该DMA外设寄存器中写入参数来控制DMA的相关属性配置，这里CPU是通过AHB总线来向DMA的AHB从设备这个寄存器中写入数据的，因为对于AHB来说，DMA是被动的，是从设备。
SRAM是运行内存，中存放的是临时变量的值（函数栈中数据），即函数里面的局部变量值会存放在SRAM里面，利用DMA实现外设寄存器到SRAM，或者SRAM到SRAM之间的数据转运，就是外设寄存器到我们main函数中定义的一个局部变量（如数组），局部变量到局部变量（如数组到数组）。
*/
	
	OLED_Init();
	
	const uint8_t aaa1 = 0x66;    //存储在RAM区
	static  uint8_t aaa2 = 0x66;  //存储在RAM区
	
	OLED_ShowHexNum(1,1,aa1,2);
	OLED_ShowHexNum(2,1,(uint32_t)&aaa2,8);
	
	
	/*
	OLED_ShowString(1,1,"DataA");
	OLED_ShowString(3,1,"DataB");
	OLED_ShowHexNum(1,8,(uint32_t)DataA,8);
	OLED_ShowHexNum(3,1,(uint32_t)DataB,8);
	
	OLED_ShowHexNum(1,1,DataA[0],2);
	OLED_ShowHexNum(1,4,DataA[1],2);
	OLED_ShowHexNum(1,7,DataA[2],2);
	OLED_ShowHexNum(1,10,DataA[3],2);
	
	OLED_ShowHexNum(2,1,DataB[0],2);
	OLED_ShowHexNum(2,4,DataB[1],2);
	OLED_ShowHexNum(2,7,DataB[2],2);
	OLED_ShowHexNum(2,10,DataB[3],2);
	
	MyDMA_Init((uint32_t)DataA,(uint32_t)DataB,4);
	
	OLED_ShowHexNum(3,1,DataA[0],2);
	OLED_ShowHexNum(3,4,DataA[1],2);
	OLED_ShowHexNum(3,7,DataA[2],2);
	OLED_ShowHexNum(3,10,DataA[3],2);
	
	OLED_ShowHexNum(4,1,DataB[0],2);
	OLED_ShowHexNum(4,4,DataB[1],2);
	OLED_ShowHexNum(4,7,DataB[2],2);
	OLED_ShowHexNum(4,10,DataB[3],2);
	*/
	
	
	OLED_Clear();
	while(1){
		
	}
}