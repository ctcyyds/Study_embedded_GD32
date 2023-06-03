#include "led.h"

//控制LED2亮灭
void led_control(void){
	//1.配置时钟
	RCU_GPIOD |= 0x08;
	//2.配置工作模式（输入输出）15 16变成01
	GPIOD_CTL &= ~(0x03 << 14);
	GPIOD_CTL |= (0x01 << 14);
	//3.上拉和下拉配置 浮空模式 15 16变成00
	GPIOD_PUD &= ~(0x03 << 14);
	//4.输出模式配置（推挽、开漏）
	GPIOD_OMODE &= ~(1 << 7);
	//5.输出速度15 16变成00
	GPIOD_OSPD &= ~(0x03 << 14);
	//6.输出高低电平 8变成1
	GPIOD_OCTL |= (1 << 7);
	//7.睡一秒
	delay_1ms(1000);
	//8.输出低电平 8变成0
	GPIOD_OCTL &= ~(1 << 7);
	delay_1ms(1000);
}