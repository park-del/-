#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void){
	PWM_Init();
}

/*
CNT计数器更新频率为50Hz，即20ms，此时ARR重装寄存器值为 20 000。由此得出 500 对应0.5ms，
5*0.5 ms =2.5ms，对应 5*500 = 2500。

0度     对应 CCR = 500，
180度   对应 CCR = 2500,
将角度Angle映射到CCR，为 CCR初始500 + Angle角度占(180-0)比例 乘以 CCR的2500-500=2000 。
*/

void Servo_SetAngle(float Angle){
	PWM_SetCompare2(Angle / 180 * 2000 + 500);  //这里面设置的是CCR的值
}