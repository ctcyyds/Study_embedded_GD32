#include "led.h"

void led_config(void){
	//ʹ��ʱ��
	rcu_periph_clock_enable(LED1_RCU);
	rcu_periph_clock_enable(LED2_RCU);
	rcu_periph_clock_enable(LED3_RCU);
	rcu_periph_clock_enable(LED4_RCU);
	
	//����GPIOģʽ������/���	����/������
	gpio_mode_set(LED1_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
	gpio_mode_set(LED2_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_PIN);
	gpio_mode_set(LED3_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_PIN);
	gpio_mode_set(LED4_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED4_PIN);
	
	//���ģʽ	�ٶ�
	gpio_output_options_set(LED1_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED1_PIN);
	gpio_output_options_set(LED2_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED2_PIN);
	gpio_output_options_set(LED3_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED3_PIN);
	gpio_output_options_set(LED4_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED4_PIN);
}
