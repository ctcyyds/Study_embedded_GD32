#include "Key.h"

//上一次状态0为低电平，1为高电平
int pre_state = 0;

void key_config(void) {
    //1.使能GPIO时钟
    rcu_periph_clock_enable(KEY_RCU);
    //2.配置GPIO模式
    gpio_mode_set(KEY_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEY_PIN);
}

void key_control(void) {
    //LED配置
    led_config();
    //KEY配置
    key_config();
    while(1) {
        //读取电平变化
        FlagStatus state = gpio_input_bit_get(KEY_GPIO, KEY_PIN);
        if(pre_state == 0 && state == SET) {//上升沿
					gpio_bit_set(LED4_GPIO, LED4_PIN);           
            //gpio_bit_toggle(LED4_GPIO, LED4_PIN);
            pre_state = 1;
            delay_1ms(10);
        } else if(pre_state == 1 && state == RESET) {
            gpio_bit_reset(LED4_GPIO, LED4_PIN);
            pre_state = 0;
            delay_1ms(10);
        }
    }
}