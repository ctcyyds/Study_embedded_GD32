#include "led.h"

void led_config(void){
	//使能时钟
	rcu_periph_clock_enable(LED1_RCU);
	rcu_periph_clock_enable(LED2_RCU);
	rcu_periph_clock_enable(LED3_RCU);
	rcu_periph_clock_enable(LED4_RCU);
	
	//配置GPIO模式（输入/输出	上拉/下拉）
	gpio_mode_set(LED1_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
	gpio_mode_set(LED2_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_PIN);
	gpio_mode_set(LED3_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_PIN);
	gpio_mode_set(LED4_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED4_PIN);
	
	//输出模式	速度
	gpio_output_options_set(LED1_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED1_PIN);
	gpio_output_options_set(LED2_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED2_PIN);
	gpio_output_options_set(LED3_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED3_PIN);
	gpio_output_options_set(LED4_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, LED4_PIN);
}
