#include "stm32f10x.h"

void Buzzer_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_12);  // GPIOA 初始化 Pin1 和 Pin2，之后高电平熄灭
}

void Buzzer_ON(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}	

void Buzzer_OFF(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void Buzzer_Turn(void){
	
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)==0 ){
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	else {
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
}