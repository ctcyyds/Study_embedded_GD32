#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "Lcd.h"
#include "Gui.h"
#include "pic.h"
#include "dac.h"
#include "eeprom.h"


int main(void)
{
//*************************[优先级&串口通信]*************************
		systick_config();
    //NVIC优先级分组d ,配置一个全局就行  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	
    //USART初始化
    usart_all_config(115200);
//===================================================
    
//*************************[EEPROM]*************************
    // eeprom初始化
    i2c_eeprom_init();
	
		// 先读出来keyTimes
    uint8_t value = 0;
		uint8_t a =90;
    eeprom_buffer_read_timeout(&value,0x04,1);
    printf("value=%d\r\n",value);
	
		// 再写入keyTimes
		eeprom_buffer_write_timeout(&a,0x04,1);	
		eeprom_buffer_read_timeout(&value,0x04,1);
    printf("value2=%d\r\n",value);
//===================================================	

//*************************[LCD屏幕]*************************
		//LCD init
		Lcd_Init();
		//Show the picture
    LCD_ShowPicture((LCD_W-240) / 2,0,240,280,gImage_b);
//===================================================

//*************************[DAC]*************************
//		//DAC init
//		dac_audio_init(8000);
//		//Music playback
//		start_play_audio(bgm,sizeof(bgm),0);
//===================================================
    while(1) {
    }
}
