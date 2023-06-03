#include "motor.h"

static void motor_gpio_init(void){
	//ʹ��ʱ��
	rcu_periph_clock_enable(BSP_PWM_RCU);
	//����GPIOģʽ
	gpio_mode_set(BSP_PWM_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_PWM_PIN);
	//����GPIO����ٶ�
	gpio_output_options_set(BSP_PWM_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_PWM_PIN);
	//����GPIO����
	gpio_af_set(BSP_PWM_PORT, BSP_PWM_AF, BSP_PWM_PIN);
}

void motor_timer_init(){
	timer_parameter_struct Timer_ParamType;
	timer_oc_parameter_struct Timer_OC_ParamType;
	//������ʱ��ʱ��
	rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);
	//���ö�ʱ��ʱ��
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	//��λ��ʱ��
	timer_deinit(BSP_PWM_TIMER);
	timer_struct_para_init(&Timer_ParamType);
	
	Timer_ParamType.alignedmode = TIMER_COUNTER_EDGE;//����Ϊ���ض���ģʽ
	Timer_ParamType.clockdivision = TIMER_CKDIV_DIV1;//������ʱ��Դ�Ͷ�ʱ��ʱ��֮��ķ�Ƶϵ��
	Timer_ParamType.counterdirection = TIMER_COUNTER_UP;//���ü�������Ϊ���ϼ���
	Timer_ParamType.period = 7200-1;//����
	Timer_ParamType.prescaler = 200-1;//Ԥ��Ƶ
	Timer_ParamType.repetitioncounter = 0;
	timer_init(BSP_PWM_TIMER,&Timer_ParamType);
	
	timer_channel_output_struct_para_init(&Timer_OC_ParamType);
	
	Timer_OC_ParamType.ocpolarity = TIMER_OC_POLARITY_HIGH;//ͨ���������
	Timer_OC_ParamType.outputstate = TIMER_CCX_ENABLE;//ʹ��ͨ�����
	
	timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL0,&Timer_OC_ParamType);
	timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL1,&Timer_OC_ParamType);
	
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL0, 0);//�ȽϼĴ�������ֵ
	timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CHANNEL1, 0);//�ȽϼĴ�������ֵ
	
	timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL0,TIMER_OC_MODE_PWM0);//����ͨ�����ģʽ
	timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL1,TIMER_OC_MODE_PWM0);//����ͨ�����ģʽ
	
	timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL0,TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL1,TIMER_OC_SHADOW_DISABLE);
	
	timer_primary_output_config(BSP_PWM_TIMER,ENABLE);
	timer_enable(BSP_PWM_TIMER);
}

void motor_init(void){
	//gpio��ʼ��
	motor_gpio_init();
	//timer��ʼ��
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