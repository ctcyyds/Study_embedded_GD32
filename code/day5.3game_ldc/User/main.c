#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "dma.h"

#include "Lcd.h"
#include "Gui.h"
#include "pic.h"

int main(void)
{
	systick_config();
	/* 扩展板 */
Lcd_Init();// spi lcd初始化
	//LCD_Fill(0,0,LCD_W,LCD_H,COLOR_RED);
//	LCD_DrawLine(50,50,100,100,COLOR_RED);
LCD_ShowPicture((LCD_W-240) / 2,0,240,280,gImage_b);         // 显示图片
	 //LCD_ShowString(100,100,"CTC",COLOR_RED,COLOR_BLACK,24,0);
    while(1) {
			
    }
}
