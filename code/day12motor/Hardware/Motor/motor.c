#include "motor.h"

static void motor_gpio_init(void){
	//使能时钟
	rcu_periph_clock_enable(BSP_PWM_RCU);
	//配置GPIO模式
	gpio_mode_set(BSP_PWM_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_PWM_PIN);
	//配置GPIO输出速度
	gpio_output_options_set(BSP_PWM_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_PWM_PIN);
	//配置GPIO复用
	gpio_af_set(BSP_PWM_PORT, BSP_PWM_AF, BSP_PWM_PIN);
}

void motor_timer_init(){
	timer_parameter_struct Timer_ParamType;
	timer_oc_parameter_struct Timer_OC_ParamType;
	//开启定时器时钟
	rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);
	//配置定时器时钟
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	//复位定时器
	timer_deinit(BSP_PWM_TIMER);
	timer_struct_para_init(&Timer_ParamType);
	
	Timer_ParamType.alignedmode = TIMER_COUNTER_EDGE;//配置为边沿对其模式
	Timer_ParamType.clockdivision = TIMER_CKDIV_DIV1;//配置主时钟源和定时器时钟之间的分频系数
	Timer_ParamType.counterdirection = TIMER_COUNTER_UP;//配置计数方向为向上计数
	Timer_ParamType.period = 7200-1;//周期
	Timer_ParamType.prescaler = 200-1;//预分频
	Timer_ParamType.repetitioncounter = 0;
	timer_init(BSP_PWM_TIMER,&Timer_ParamType);
	
	timer_channel_output_struct_para_init(&Timer_OC_ParamType);
	
	Timer_OC_ParamType.ocpolarity = TIMER_OC_POLARITY_HIGH;//通道输出极性
	Timer_OC_ParamType.outputstate = TIMER_CCX_ENABLE;//使能通道输出
	
	timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL0,&Timer_OC_ParamType);
	timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL1,&Timer_OC_ParamType);
	
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL0, 0);//比较寄存器的数值
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL1, 0);//比较寄存器的数值
	
	timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL0,TIMER_OC_MODE_PWM0);//配置通道输出模式
	timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL1,TIMER_OC_MODE_PWM0);//配置通道输出模式
	
	timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL0,TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL1,TIMER_OC_SHADOW_DISABLE);
	
	timer_primary_output_config(BSP_PWM_TIMER,ENABLE);
	timer_enable(BSP_PWM_TIMER);
}

void motor_init(void){
	//gpio初始化
	motor_gpio_init();
	//timer初始化
	motor_timer_init();
}

void motor_set_pwm(int pwm){
	if(pwm > 0){
		timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL0, pwm);
		timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL1, 0);
	}else{
		timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL0, 0);
		timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL1, pwm);
	}
	
}

void motor_test(void){
	int pwm = 7200;
	while(1){
		motor_set_pwm(pwm);
		pwm -= 1000;
		if(pwm < -7200){
			pwm = 7200;
		}
		delay_1ms(500);
	}
}