#include "stm32f10x.h"                  // Device header
#include "delay.h"


int main(void){

/*	寄存器实现点灯
	RCC->APB2ENR = 0x00000010;
	GPIOC->CRH = 0X00300000;
	GPIOC->ODR = 0X00002000;
*/
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_initStructure;
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_initStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_initStructure);

/*	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);    //低电平点亮灯 
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	     //高电平熄灭灯
*/	


/*
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);	//低电平点亮灯 
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);	//高电平熄灭灯
*/

	//	库函数实现点灯	
	while(1){
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		Delay_ms(500);
	}
}