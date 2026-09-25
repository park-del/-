#include "stm32f10x.h"                  // Device header
#include "delay.h"


int main(void){

/*	寄存器实现点灯
	RCC->APB2ENR = 0x00000010;
	GPIOC->CRH = 0X00300000;
	GPIOC->ODR = 0X00002000;
*/
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_initStructure;
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_initStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB,&GPIO_initStructure);

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
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);  //低电平点亮，前面加一个取反即为GPIOA0点亮
		Delay_ms(500);
		
		GPIO_SetBits(GPIOB,GPIO_Pin_12);   
		Delay_ms(500);
	}
}