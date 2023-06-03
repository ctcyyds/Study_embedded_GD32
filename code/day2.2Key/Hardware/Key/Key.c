#include "Key.h"

//��һ��״̬0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
int pre_state = 0;

void key_config(void) {
    //1.ʹ��GPIOʱ��
    rcu_periph_clock_enable(KEY_RCU);
    //2.����GPIOģʽ
    gpio_mode_set(KEY_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEY_PIN);
}

void key_control(void) {
    //LED����
    led_config();
    //KEY����
    key_config();
    while(1) {
        //��ȡ��ƽ�仯
        FlagStatus state = gpio_input_bit_get(KEY_GPIO, KEY_PIN);
        if(pre_state == 0 && state == SET) {//������
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