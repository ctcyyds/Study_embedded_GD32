#include "led.h"

//����LED2�� 2������
static void led_config_single(rcu_periph_enum rcu,uint32_t gpio_type,uint32_t pin){
	//1.ʹ��ʱ��
	rcu_periph_clock_enable(rcu);
	//2.����GPIO��ģʽ(����/���  ����/����)
	gpio_mode_set(gpio_type,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,pin);
	//4.���ģʽ
	//5.����ٶ�
	gpio_output_options_set(gpio_type,GPIO_OTYPE_PP,GPIO_OSPEED_2MHZ,pin);
}

//����LED2�� 2������
void led_config(void){
	led_config_single(LED1_RCU,LED1_GPIO,LED1_PIN);
	led_config_single(LED2_RCU,LED2_GPIO,LED2_PIN);
	led_config_single(LED3_RCU,LED3_GPIO,LED3_PIN);
	led_config_single(LED4_RCU,LED4_GPIO,LED4_PIN);
//	//1.ʹ��ʱ��
//	rcu_periph_clock_enable(LED1_RCU);
//	//2.����GPIO��ģʽ(����/���  ����/����)
//	gpio_mode_set(LED2_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED2_PIN);
//	//4.���ģʽ
//	//5.����ٶ�
//	gpio_output_options_set(LED2_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_2MHZ,LED2_PIN);

}