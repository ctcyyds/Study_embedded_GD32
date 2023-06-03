#include "led.h"

//����LED8����(P45=PD4  P20=PD5)
//��ʼ��
void led_init(void) {
    //����GPIOD�Ķ˿�ʱ��
    rcu_periph_clock_enable(BSP_LEDSW_RCU);
    rcu_periph_clock_enable(BSP_LED8_RCU);

    //����GPIOD4��D5Ϊ����ģʽ
    gpio_mode_set(BSP_LEDSW_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,BSP_LEDSW_PIN);
    gpio_mode_set(BSP_LED8_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,BSP_LED8_PIN);

    //����GPIO���
    gpio_output_options_set(BSP_LEDSW_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_LEDSW_PIN);
    gpio_output_options_set(BSP_LED8_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_LED8_PIN);
    gpio_bit_reset(BSP_LEDSW_PORT,BSP_LEDSW_PIN);
    gpio_bit_set(BSP_LED8_PORT,BSP_LED8_PIN);
}
////�������Ÿߵ͵�ƽ
//void led_control(void) {
//    gpio_bit_set(BSP_LED8_PORT,BSP_LED8_PIN);
//    gpio_bit_reset(BSP_LEDSW_PORT,BSP_LEDSW_PIN);
//    //LED8
//    gpio_bit_reset(BSP_LED8_PORT,BSP_LED8_PIN);
//    delay_1ms(1000);
//}
//����
//void led_run(void) {
//    led_init();
//    led_control();
//}