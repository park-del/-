#include "stm32f10x.h"                  // Device header

void PWM_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*重映射部分的代码
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //选用复用推挽输出，此时输出控制权不由
	//输出数据寄存器，输出控制权转移给片上外设。本程序中这里片上外设引脚连接的就是TIM2_CH1，通过PA0输出。
	//即 TIM2_CH1 --- 片上外设引脚 --- PA0输出引脚。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //重映射端口：GPIO_Pin_15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;      //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;    //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//配置输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);  //给结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  				//设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  		//设置输出比较极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	//设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;   							//设置CCR，这里是为0，可以通过TIM_SetCompare1()函数来实时设置CCR的值
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);  //TIM2_CH1通道借用了PA0引脚
	
	//通过设置 ARR、PSC、CCR 产生一个频率为1KHz，占空比为50%，分辨率为1%的PWM波形，带入公式
	//求得 ARR = 100 - 1、CCR = 50、PSC = 720 - 1 。
	
	//启动定时器
	TIM_Cmd(TIM2,ENABLE);
}


void PWM_SetCompare1(uint16_t Compare){
	TIM_SetCompare1(TIM2,Compare);
}

