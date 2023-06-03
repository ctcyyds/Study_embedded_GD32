#include "digital.h"

//*************************[GPIO����]*************************
void dig_gpio_config(void){
	//ʹ��ʱ��
	rcu_periph_clock_enable(BSP_DI_RCU);
	rcu_periph_clock_enable(BSP_RCK_RCU);
	rcu_periph_clock_enable(BSP_SCK_RCU);
	
	//����GPIOģʽ������/��� ����/������
	gpio_mode_set(BSP_DI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_DI_PIN);
	gpio_mode_set(BSP_RCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_RCK_PIN);
	gpio_mode_set(BSP_SCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_SCK_PIN);
	
	//����GPIO���ģʽ������ٶ�
	gpio_output_options_set(BSP_DI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BSP_DI_PIN);
	gpio_output_options_set(BSP_RCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BSP_RCK_PIN);
	gpio_output_options_set(BSP_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BSP_SCK_PIN);
}
//===================================================

//*************************[�������ʾ�߼�]*************************
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
			// ��λ����
			gpio_bit_reset(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
			gpio_bit_set(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
		}
		
		for(i = 8; i >= 1; i--) {
			//λ����
			int data = (idx & (1 << (i - 1))) >> (i - 1);
			if(data){
				gpio_bit_set(BSP_DI_PORT,BSP_DI_PIN);
			}else{
				gpio_bit_reset(BSP_DI_PORT,BSP_DI_PIN);
			}
			// ��λ����
			gpio_bit_reset(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
			gpio_bit_set(BSP_SCK_PORT,BSP_SCK_PIN);
			__NOP;
		}
		
		// ����
		// ��λ����
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