#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"


uint8_t KeyNum;

int main(void){

	OLED_Init();
	Serial_Init();
//	Serial_SendByte(0x41);
	
//	uint8_t MyArray[] = {0x42,0x43,0x44,0x45};
//	Serial_SendArray(MyArray,4);
	
//	Serial_SendString("HelloWorld!\r\n");  //编译器会自动补上结束标志位,故字符串的存储空间会比字符的个数大1

	Serial_SendNumber(12345,5);
	//OLED_Clear();
	while(1){
		
	}
}