#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

void Encoder_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入模式，默认为高电平的输入方式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //AFIO外设中断引脚选择配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	//AFIO这里为多路选择器的功能，输入端为GPIOB的PB14引脚,输出端固定连接EXIT的第14个中断线路（去查看视频）
	
	// 将EXTI的第14个线路配置为中断模式，下降沿触发，开启中断。这样PB14的电平信号就能够通过EXTI
	// 通向下一级NVIC了。
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组2: 2位抢占，2位响应
	//注意：中断分组方式整个芯片只能用一种（具体见视频）。
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;        //指定通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //指定所选通道抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //指定所选通道响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;        //指定通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //指定所选通道抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //指定所选通道响应优先级
	NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get(void){
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}

void EXTI0_IRQHandler(void){
	if (EXTI_GetITStatus(EXTI_Line0) == SET){
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0){
			Encoder_Count--;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void){
	if (EXTI_GetITStatus(EXTI_Line1) == SET){
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
			Encoder_Count++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}


