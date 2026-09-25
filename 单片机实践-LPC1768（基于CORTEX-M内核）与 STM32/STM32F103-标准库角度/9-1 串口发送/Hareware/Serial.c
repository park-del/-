#include "stm32f10x.h"                  // Device header

void Serial_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //这里设置的寄存器就是设置多路选择器中的值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	       //发送模式
	USART_InitStructure.USART_Parity = USART_Parity_No;    //校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //数据位字长
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);  //对发送寄存器TDR的写操作触发清零
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//等待数据从TDR写入到移位寄存器
	
}

void Serial_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for(i=0;i<Length;i++){
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String){
	uint8_t i;
	for(i=0; String[i]!='\0'; i++){  // 空字符0的 转义字符表示形式为 '\0'，这里写0和写'\0'效果是一样的  
		Serial_SendByte(String[i]);  /*y因为一次发送8位数据位，而一个字符占8位，故对于字符串
		只能够拆分成字符的形式一位一位的发送，根据for循环终止条件可以字符0是不会发送的（没有进入循
		环）。*/
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y){
	uint32_t Result = 1;
	while(Y--){
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length){
	uint8_t i;
	for(i=0; i<Length; i++){
		Serial_SendByte(Number / Serial_Pow(10,Length-i-1)%10+'0');
	}
}