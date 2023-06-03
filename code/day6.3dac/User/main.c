#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "Lcd.h"
#include "Gui.h"
#include "pic.h"
#include "dac.h"


int main(void)
{
    systick_config();
    //NVIC���ȼ�����d ,����һ��ȫ�־���  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //USART��ʼ��
    usart_all_config(115200);
//    Lcd_Init();
//    LCD_ShowPicture((LCD_W-240) / 2,0,240,280,gImage_b);
    dac_audio_init(8000);
    start_play_audio(bgm,sizeof(bgm),0);
    while(1) {
    }
}
