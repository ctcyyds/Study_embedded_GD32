#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "dma_m2p.h"
#include "lcd.h"
#include "touch.h"
#include "pic.h"

int main(void)
{
    systick_config();
    //NVIC���ȼ�����,����һ��ȫ�־���  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //USART��ʼ��
    usart_dma_config(115200);
    printf("hello\r\n");
    /* 8080 mcu�� */
    LCD_Init(); //��ʾ����ʼ������
//POINT_COLOR=GREEN;
//BACK_COLOR=LBBLUE;
//LCD_Clear(LBBLUE);
//LCD_ShowString(30,50,480,80,24,1,"https://www.itcast.cn/");
//LCD_ShowString(30,80,480,110,24,1,lcd_id);
//LCD_ShowString(30,110,480,140,24,1,"touch test....");

    //��ʾͼƬ
//    LCD_ShowPic(0,480,0,800,test,384000);
//    delay_1ms(3000);
    LCD_ShowPic(0,480,0,800,test3,384000);
    while(1) {
    }
}
