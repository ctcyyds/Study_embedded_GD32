#include "led.h"

//����LED2����
void led_control(void){
	//1.����ʱ��
	RCU_GPIOD |= 0x08;
	//2.���ù���ģʽ�����������15 16���01
	GPIOD_CTL &= ~(0x03 << 14);
	GPIOD_CTL |= (0x01 << 14);
	//3.�������������� ����ģʽ 15 16���00
	GPIOD_PUD &= ~(0x03 << 14);
	//4.���ģʽ���ã����졢��©��
	GPIOD_OMODE &= ~(1 << 7);
	//5.����ٶ�15 16���00
	GPIOD_OSPD &= ~(0x03 << 14);
	//6.����ߵ͵�ƽ 8���1
	//GPIOD_OCTL |= (1 << 7);
	GPIOD_BOP &= ~0x800000;
	GPIOD_BOP |= 0x80;
	//7.˯һ��
	delay_1ms(1000);
	//8.����͵�ƽ 8���0
	//GPIOD_OCTL &= ~(1 << 7);
	GPIOD_BOP &= ~0x80;
	GPIOD_BOP |= 0x800000;
	delay_1ms(1000);
}