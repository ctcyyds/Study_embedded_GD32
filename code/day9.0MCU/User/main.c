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
    //NVIC优先级分组,配置一个全局就行  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //USART初始化
    usart_dma_config(115200);
    printf("hello\r\n");
    /* 8080 mcu屏 */
    LCD_Init(); //显示屏初始化代码
//POINT_COLOR=GREEN;
//BACK_COLOR=LBBLUE;
//LCD_Clear(LBBLUE);
//LCD_ShowString(30,50,480,80,24,1,"https://www.itcast.cn/");
//LCD_ShowString(30,80,480,110,24,1,lcd_id);
//LCD_ShowString(30,110,480,140,24,1,"touch test....");

    //显示图片
//    LCD_ShowPic(0,480,0,800,test,384000);
//    delay_1ms(3000);
    LCD_ShowPic(0,480,0,800,test3,384000);
    while(1) {
    }
}
