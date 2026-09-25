#include "stm32f10x.h"

void LED_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);  // GPIOA 初始化 Pin1 和 Pin2，之后高电平熄灭
}

void LED1_ON(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}	

void LED1_OFF(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

void LED1_Turn(void){
	
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0 ){  //注意，这里是读取输出寄存器某个位，而不是读取输入寄存器的
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else {
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
}


void LED2_Turn(void){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0){
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
	else {
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}


void LED2_ON(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}

void LED2_OFF(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}