#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"


uint8_t KeyNum;

int main(void){

	LED_Init();
	Key_Init();
	
	
	
	while(1){
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			LED1_Turn();  //注意：按键相关GPIOB端口模式是上拉输入，led相关GPIOA端口是推挽输出
		}
		
		if(KeyNum == 2){
			LED2_Turn();
		}
	}
}