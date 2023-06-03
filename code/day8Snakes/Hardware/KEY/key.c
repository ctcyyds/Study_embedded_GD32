#include "key.h"
#include "dac.h"

uint8_t key_B_pressed = 0;

//*************************[按键中断配置]*************************
void single_key_interrupt_config(rcu_periph_enum periph, uint32_t gpio_periph, uint32_t pin, uint8_t nvic_irq, uint8_t exti_port, uint8_t exti_pin, exti_line_enum linex){
	//开启GPIO时钟和SYSCFG时钟
	rcu_periph_clock_enable(periph);
	rcu_periph_clock_enable(RCU_SYSCFG);
	//gpio模式配置
	gpio_mode_set(gpio_periph,GPIO_MODE_INPUT,GPIO_PUPD_PULLDOWN,pin);
	//使能NVIC并配置优先级 根据优先级分组 只能配置0~3
	nvic_irq_enable(nvic_irq,2,2);  //2 1
	//通过SYSCFG将PA0引脚连接上EXTI检测线路
	syscfg_exti_line_config(exti_port,exti_pin);
	
	//exti配置
	//exti初始化
	exti_init(linex,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
	//使能中断
	exti_interrupt_enable(linex);
	//清除中断标志位
	exti_interrupt_flag_clear(linex);
}
//===================================================

//*************************[所有按键中断初始化]*************************
void all_key_interrupt_config(void){
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
//===================================================

//*************************[KEY1中断处理函数]*************************
void KEYA_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYA_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYA_GPIO,KEYA_PIN)==SET){
			//翻转
			//gpio_bit_toggle(LED1_GPIO,LED1_PIN);
		}
		//清除中断标志位
		exti_interrupt_flag_clear(KEYA_EXTI_LINE);
		delay_1ms(50);
	}
}
//===================================================

//*************************[KEY2中断处理函数]*************************
void KEYB_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYB_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYB_GPIO,KEYB_PIN)==SET){
			printf("press\n");
			start_play_audio(bgm_key,sizeof(bgm_key),0);
			key_B_pressed = 1;
		}else{
			key_B_pressed = 0;
			printf("release\n");
		}
		//清除中断标志位
		exti_interrupt_flag_clear(KEYB_EXTI_LINE);
//		delay_1ms(50);
	}
}
//===================================================

//*************************[KEY3中断处理函数]*************************
void KEYL_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYL_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYL_GPIO,KEYL_PIN)==SET){
			//翻转
			//gpio_bit_toggle(LED3_GPIO,LED3_PIN);
		}
		//清除中断标志位
		exti_interrupt_flag_clear(KEYL_EXTI_LINE);
		delay_1ms(50);
	}
}
//===================================================

//*************************[KEY4中断处理函数]*************************
void KEYR_EXTI_IRQHandler(void){
	if(exti_interrupt_flag_get(KEYR_EXTI_LINE) == SET){
		if(gpio_input_bit_get(KEYR_GPIO,KEYR_PIN)==SET){
			//翻转
			//gpio_bit_toggle(LED4_GPIO,LED4_PIN);
		}
		//清除中断标志位
		exti_interrupt_flag_clear(KEYR_EXTI_LINE);
		delay_1ms(100);
	}
}

//===================================================

//*************************[程序运行]*************************
void key_led_run(void){
	//led初始化
	//led_config();
	//按键初始化
	all_key_interrupt_config();
}
//===================================================