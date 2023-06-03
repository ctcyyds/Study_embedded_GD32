
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "timer.h"
#include "dma_m2p.h"

#include "lcd.h"
#include "touch.h"

#include "lv_conf.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
//音乐界面案例
#include "lv_demo_music.h"
#include "lvgl_demo.h"


int main(void)
{
    //NVIC优先级分组,配置一个全局就行  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    systick_config();
    //usart初始化
    usart_dma_config(115200);
    // 定时器初始化
    basic_timer_config(24000,1*10);

    LCD_Init(crosswise); //显示屏初始化
    //触摸屏初始化
    GT1151_Init();

    //lvgl初始化
    lv_init();
    //显示初始化
    lv_port_disp_init();
    //触摸初始化
    lv_port_indev_init();

    //展示音乐案例
    //lv_demo_music();
    //first_screen();
    //keyboard_1();
//		obj_style();
//    obj_label();
//		obj_btn();
//		obj_btn_matrix();
//    obj_text_area();
		obj_keyboard();

    while(1) {
        delay_1ms(5);
        lv_timer_handler();
    }
}
