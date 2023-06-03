#include "timer.h"

void timer_config(uint16_t pre,uint16_t per){
	timer_parameter_struct timere_initpara;
	//��ʼ��led
	led_config();
	//��ʼ��timer��ʱ��
	//ʹ�ܶ�ʱ��ʱ��
	rcu_periph_clock_enable(RCU_TIMER5);
	//���б�Ƶ(�����ֲ��е�ʱ��������)  ��ʱ��Ƶ��:240Mhz   240 000 000 
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	/*4.��λ��ʱ��*/
	timer_deinit(TIMER5);
	/*5.���ö�ʱ������  240Mhz   ��Ƶϵ��:240 ����:1000000*/
	//ֻ��Ҫ��ע��Ƶϵ�������ھͿ���
	timere_initpara.prescaler = pre-1; //Ԥ��Ƶֵ 0-65535
	timere_initpara.period = per-1;  // ����(��Ϸ�Ƶֵ����ʱ��) 
	
	timere_initpara.alignedmode = TIMER_COUNTER_EDGE; //����ģʽ(��ʱû��)
	timere_initpara.counterdirection = TIMER_COUNTER_UP;  // ���ϼ���(��ͨ��ʱ��ֻ�������ʽ)
	/* �����벶���ʱ��ʹ��  �����˲���ʹ�õĲ���Ƶ��֮��ķ�Ƶ���� */
	timere_initpara.clockdivision = TIMER_CKDIV_DIV1;     // ��Ƶ���� 
	/* ֻ�и߼���ʱ������ ����Ϊx�����ظ�x+1�ν����ж� */    
	timere_initpara.repetitioncounter = 0;	// �ظ������� 0-255
	/*6.��ʼ����ʱ��*/
	timer_init(TIMER5,&timere_initpara);
	
	/*7.�����ж����ȼ�*/
	nvic_irq_enable(TIMER5_DAC_IRQn,2,2);
	
	//�����ж�
	/*8.ʹ���ж�*/
	timer_interrupt_enable(TIMER5,TIMER_INT_UP);
	/*9.ʹ�ܶ�ʱ��*/
	timer_enable(TIMER5);
	
}

//��ʱ���жϴ�����
void TIMER5_DAC_IRQHandler(void){
	//�л�led2��״̬
	gpio_bit_toggle(LED2_GPIO,LED2_PIN);
	//����жϱ�־λ
	timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
}