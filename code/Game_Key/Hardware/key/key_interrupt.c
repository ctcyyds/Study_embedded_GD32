#include "key_interrupt.h"

//����һ��key���ж�
void single_key_interrupt_config(rcu_periph_enum periph,uint32_t gpio_periph
																	,uint32_t pin,uint8_t nvic_irq,
																		uint8_t exti_port, uint8_t exti_pin,
																		exti_line_enum linex){
	//����GPIOʱ�Ӻ�SYSCFGʱ��
	rcu_periph_clock_enable(periph);
	rcu_periph_clock_enable(RCU_SYSCFG);
	//gpioģʽ����
	gpio_mode_set(gpio_periph,GPIO_MODE_INPUT,GPIO_PUPD_PULLDOWN,pin);
	//ʹ��NVIC���������ȼ� �������ȼ����� ֻ������0~3
	nvic_irq_enable(nvic_irq,2,2);  //2 1
	//ͨ��SYSCFG��PA0����������EXTI�����·
	syscfg_exti_line_config(exti_port,exti_pin);
	
	//exti����
	//exti��ʼ��
	exti_init(linex,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
	//ʹ���ж�
	exti_interrupt_enable(linex);
	//����жϱ�־λ
	exti_interrupt_flag_clear(linex);

}
																		
void all_key_interrupt_config(){
	single_key_interrupt_config(KEYA_RCU,KEYA_GPIO,KEYA_PIN,KEYA_NVIC_IRQ,
															KEYA_EXTI_SOURCE_GPIO,KEYA_EXTI_SOURCE_PIN,
															KEYA_EXTI_LINE);
	single_key_interrupt_config(KEYB_RCU,KEYB_GPIO,KEYB_PIN,KEYB_NVIC_IRQ,
															KEYB_EXTI_SOURCE_GPIO,KEYB_EXTI_SOURCE_PIN,
															KEYB_EXTI_LINE);
	single_key_interrupt_config(KEYL_RCU,KEYL_GPIO,KEYL_PIN,KEYL_NVIC_IRQ,
															KEYL_EXTI_SOURCE_GPIO,KEYL_EXTI_SOURCE_PIN,
															KEYL_EXTI_LINE);
	single_key_interrupt_config(KEYR_RCU,KEYR_GPIO,KEYR_PIN,KEYR_NVIC_IRQ,
															KEYR_EXTI_SOURCE_GPIO,KEYR_EXTI_SOURCE_PIN,
															KEYR_EXTI_LINE);
}

//EXTI0���жϴ�����
void KEYA_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYA_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYA_GPIO,KEYA_PIN)==SET){
			//��ת
			gpio_bit_toggle(LED1_GPIO,LED1_PIN);
		}
		//����жϱ�־λ
		exti_interrupt_flag_clear(KEYA_EXTI_LINE);
		delay_1ms(50);
	}
}

//EXTI0���жϴ����� 10-15�ж�  15 10
void KEYB_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYB_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYB_GPIO,KEYB_PIN)==SET){
			//��ת
			gpio_bit_toggle(LED2_GPIO,LED2_PIN);
		}
		//����жϱ�־λ
		exti_interrupt_flag_clear(KEYB_EXTI_LINE);
		delay_1ms(50);
	}
	//����ж���ж�,���Զ�����жϼ���
//	if(exti_interrupt_flag_get(KEYB_EXTI_LINE) == SET){
//		if(gpio_input_bit_get(KEYB_GPIO,KEYB_PIN)==SET){
//			//��ת
//			gpio_bit_toggle(LED2_GPIO,LED2_PIN);
//		}
//		//����жϱ�־λ
//		exti_interrupt_flag_clear(KEYB_EXTI_LINE);
//		delay_1ms(50);
//	}
}
//EXTI0���жϴ�����
void KEYL_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYL_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYL_GPIO,KEYL_PIN)==SET){
			//��ת
			gpio_bit_toggle(LED3_GPIO,LED3_PIN);
		}
		//����жϱ�־λ
		exti_interrupt_flag_clear(KEYL_EXTI_LINE);
		delay_1ms(50);
	}
}
//EXTI0���жϴ�����
void KEYR_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYR_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYR_GPIO,KEYR_PIN)==SET){
			//��ת
			printf("interrupt\n");
			gpio_bit_toggle(LED4_GPIO,LED4_PIN);
		}
		//����жϱ�־λ
		exti_interrupt_flag_clear(KEYR_EXTI_LINE);
		delay_1ms(50);
	}
}

void key_led_config(void){
	//led��ʼ��
	led_config();
	//������ʼ��
	all_key_interrupt_config();
}

