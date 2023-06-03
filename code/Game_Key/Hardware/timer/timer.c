#include "timer.h"

void timer_config(uint16_t pre,uint16_t per){
	timer_parameter_struct timere_initpara;
	//初始化led
	led_config();
	//初始化timer定时器
	//使能定时器时钟
	rcu_periph_clock_enable(RCU_TIMER5);
	//进行倍频(数据手册中的时钟树配置)  定时器频率:240Mhz   240 000 000 
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	/*4.复位定时器*/
	timer_deinit(TIMER5);
	/*5.配置定时器参数  240Mhz   分频系数:240 周期:1000000*/
	//只需要关注分频系数和周期就可以
	timere_initpara.prescaler = pre-1; //预分频值 0-65535
	timere_initpara.period = per-1;  // 周期(结合分频值计算时间) 
	
	timere_initpara.alignedmode = TIMER_COUNTER_EDGE; //对齐模式(暂时没用)
	timere_initpara.counterdirection = TIMER_COUNTER_UP;  // 向上计数(普通定时器只有这个方式)
	/* 在输入捕获的时候使用  数字滤波器使用的采样频率之间的分频比例 */
	timere_initpara.clockdivision = TIMER_CKDIV_DIV1;     // 分频因子 
	/* 只有高级定时器才有 配置为x，就重复x+1次进入中断 */    
	timere_initpara.repetitioncounter = 0;	// 重复计数器 0-255
	/*6.初始化定时器*/
	timer_init(TIMER5,&timere_initpara);
	
	/*7.配置中断优先级*/
	nvic_irq_enable(TIMER5_DAC_IRQn,2,2);
	
	//开启中断
	/*8.使能中断*/
	timer_interrupt_enable(TIMER5,TIMER_INT_UP);
	/*9.使能定时器*/
	timer_enable(TIMER5);
	
}

//定时器中断处理函数
void TIMER5_DAC_IRQHandler(void){
	//切换led2的状态
	gpio_bit_toggle(LED2_GPIO,LED2_PIN);
	//清除中断标志位
	timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
}