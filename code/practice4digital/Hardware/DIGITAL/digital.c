#include "digital.h"

//*************************[GPIO配置]*************************
void dig_gpio_config(void){
	//使能时钟
	rcu_periph_clock_enable(BSP_DI_RCU);
	rcu_periph_clock_enable(BSP_RCK_RCU);
	rcu_periph_clock_enable(BSP_SCK_RCU);
	
	//配置GPIO模式（输入/输出 上拉/下拉）
	gpio_mode_set(BSP_DI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_DI_PIN);
	gpio_mode_set(BSP_RCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_RCK_PIN);
	gpio_mode_set(BSP_SCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_SCK_PIN);
	
	//配置GPIO输出模式和输出速度
	gpio_output_options_set(BSP_DI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BSP_DI_PIN);
	gpio_output_options_set(BSP_RCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BSP_RCK_PIN);
	gpio_output_options_set(BSP_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BSP_SCK_PIN);
}
//===================================================

//*************************[数码管显示逻辑]*************************
void dig_config(void){
	uint8_t i, dat, idx;
	//dat = 0b01010101;
	dat = 0xA4;
	//idx = 0b00000001
	idx = 0x80;
	while(1) {
		
		for(i = 8; i >= 1; i--) {
			int data = (dat & (1 << (i - 1))) >> (i - 1);
			if(data){
				gpio_bit_set(BSP_DI_PORT,BSP_DI_PIN);
			}else{
				gpio_bit_reset(BSP_DI_PORT,BSP_DI_PIN);
			}
			// 移位操作
			gpio_bit_reset(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
			gpio_bit_set(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
		}
		
		for(i = 8; i >= 1; i--) {
			//位输入
			int data = (idx & (1 << (i - 1))) >> (i - 1);
			if(data){
				gpio_bit_set(BSP_DI_PORT,BSP_DI_PIN);
			}else{
				gpio_bit_reset(BSP_DI_PORT,BSP_DI_PIN);
			}
			// 移位操作
			gpio_bit_reset(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
			gpio_bit_set(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
		}
		
		// 锁存
		// 移位操作
		gpio_bit_reset(BSP_RCK_PORT,BSP_RCK_PIN);
		__NOP;
		gpio_bit_set(BSP_RCK_PORT,BSP_RCK_PIN);
		__NOP;
	}
}
//===================================================

void dig_run(void){
	dig_gpio_config();
		dig_config();
}