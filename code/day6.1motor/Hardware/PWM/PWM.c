#include "PWM.h"
//��P01����pwm����
//P01=PA5

//���ȶ�PA5����GPIO����
void pwm_gpio_config(void) {
    /*1.ʹ��ʱ��*/
    rcu_periph_clock_enable(BSP_PWM_RCU);
    /*2.����GPIO��ģʽ*/
    gpio_mode_set(BSP_PWM_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,BSP_PWM_PIN);
    /*3.����GPIO�����*/
    gpio_output_options_set(BSP_PWM_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_PWM_PIN);

    /*4.����GPIO�ĸ���*/
    gpio_af_set(BSP_PWM_PORT,BSP_PWM_AF,BSP_PWM_PIN);
}
//pwm���� pre:��Ƶϵ��  per:����
void pwm_config(uint16_t pre,uint16_t per) {
    /*1.���嶨ʱ���ṹ��*/
    timer_parameter_struct timere_initpara;
    /*2.��ʱ���Ƚ�����ṹ��*/
    timer_oc_parameter_struct timer_ocintpara;
    /*3.PA5����GPIO����*/
    pwm_gpio_config();
    /*4.������ʱ��ʱ��*/
    rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);
    /*5.���ö�ʱ��ʱ��(4��)*/
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    /*6.��λ��ʱ��*/
    timer_deinit(BSP_PWM_TIMER);
    /*7.���ö�ʱ������*/
    timere_initpara.prescaler = pre-1; //Ԥ��Ƶֵ 0-65535
    timere_initpara.alignedmode = TIMER_COUNTER_EDGE; //����ģʽ(��ʱû��)
    timere_initpara.counterdirection = TIMER_COUNTER_UP;  // ���ϼ���(��ͨ��ʱ��ֻ�������ʽ)
    timere_initpara.period = per-1;  // ����(��Ϸ�Ƶֵ����ʱ��)
    /* �����벶���ʱ��ʹ��  �����˲���ʹ�õĲ���Ƶ��֮��ķ�Ƶ���� */
    timere_initpara.clockdivision = TIMER_CKDIV_DIV1;     // ��Ƶ����
    /* ֻ�и߼���ʱ������ ����Ϊx�����ظ�x+1�ν����ж� */
    timere_initpara.repetitioncounter = 0;	// �ظ������� 0-255
    /*8.��ʼ����ʱ��*/
    timer_init(BSP_PWM_TIMER,&timere_initpara);
    /*9.��������ṹ��*/
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;//��Ч��ƽ�ļ���   																	  // ��Ч��ƽ�ļ���
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;	//�Ƚ����ģʽ״̬,��ʹpwm������˿�
    /*10.���ö�ʱ���������*/
    timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,&timer_ocintpara);
    /*11.����ռ�ձ�*/
    //���ö�ʱ��ͨ���������ֵ
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,0);
    //���ö�ʱ��ͨ������Ƚ�ģʽ
    timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,TIMER_OC_MODE_PWM0);
    //���ö�ʱ��ͨ�����Ӱ�ӼĴ���
    timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,TIMER_OC_SHADOW_DISABLE);
    //ֻ�и߼���ʱ������Ҫ������
    //timer_primary_output_config(TIMER0,ENABLE);
    /*11.TIMEx�Զ�����Ӱ��ʹ��*/
    timer_auto_reload_shadow_enable(BSP_PWM_TIMER);
    /*12.ʹ�ܶ�ʱ��*/
    timer_enable(BSP_PWM_TIMER);
}

void pwm_breathing_lamp(void) {

    static uint16_t value = 0;
// ���ö�ʱ��ͨ���������ֵ
	for(int i = 0; i < 10; i++){
		timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,value);
		value += 2000;
//		delay_1ms(1000);
		if(value == 10000){
			value = 0;
		}
	}
//    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,2000);
////    delay_1ms(1000);
//    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,8000);
////    delay_1ms(5000);
//		timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,0);
	
}

void pwm_breathing_lamp_run(void) {
    //��ʼ��led
    pwm_gpio_config();
    //����pwm
    pwm_config(20000,10000);
    while(1) {
        pwm_breathing_lamp();
    }
}