#include "dac.h"


uint8_t audio_status = 0; // ����״̬

audio currentAudio; // ��ǰ��Ƶ 

void dac_audio_gpio_init(void)
{
		//ʹ��ʱ��
    rcu_periph_clock_enable(DAC_AUDIO_RCU);
    rcu_periph_clock_enable(RCU_DAC);
		
	//����ģ��ģʽ  
	gpio_mode_set(DAC_AUDIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_AUDIO_PIN);
		
	// ��λDAC
	dac_deinit();  
    //ʹ�������ʽ����
	dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // ʹ�ö�ʱ��3��Ϊ����Դ
	//����ʹ��
    dac_trigger_enable(DAC1);

	//ʹ�����������
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // ʹ��DAC
	
//    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0);  // 8λ�����Ҷ���
//		// �������ʹ�� �������
//		dac_software_trigger_enable(DAC1);  
}

/* ��ʱ������ 
T=��TIM_Period+1��*��TIM_Prescaler+1��/TIMxCLK
samplingRate:������
*/
void Timer3_config(uint16_t samplingRate)
{
    timer_parameter_struct timer_initpara;
	// �����ж����ȼ�
    nvic_irq_enable(DAC_TIMER_IRQ, 2,2); 
	//ʹ��ʱ��
	rcu_periph_clock_enable(DAC_RCU_TIMER);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(DAC_TIMER);  // ��ʱ����λ
	/* TIMER2 configuration */
    timer_initpara.prescaler         = 24 - 1;  // Ԥ��Ƶ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // ����ģʽ
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // ��������
    timer_initpara.period            = 240000000 / 24 / samplingRate -1; // ����
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // ʱ�ӷ�Ƶ
    timer_initpara.repetitioncounter = 0; // �ظ�������
    timer_init(DAC_TIMER,&timer_initpara);
	
	/* TIMER2 interrupt enable */
    timer_interrupt_enable(DAC_TIMER,TIMER_INT_UP);  // �ж�ʹ��
		
	timer_disable(DAC_TIMER);
}

void dac_audio_init(uint16_t samplingRate)
{
		// gpio��ʼ��
		dac_audio_gpio_init(); 					
		// ������Ƶ������
		Timer3_config(samplingRate); 	  
}


void DAC_TIMER_IRQHandler(void)
{
	if (timer_interrupt_flag_get(DAC_TIMER, TIMER_INT_FLAG_UP))
	 { 
		 /* clear TIMER interrupt flag */
		 timer_interrupt_flag_clear(DAC_TIMER, TIMER_INT_FLAG_UP);
			
		 dac_data_set(DAC1, DAC_ALIGN_8B_R, currentAudio.audioCurrentPlayP[currentAudio.audioIndex]);  // 8λ�����Ҷ��� 
		 dac_software_trigger_enable(DAC1);  // �������ʹ�� �������
		 
		 currentAudio.audioIndex ++; // �ȴ���һ������
		 
		
		 if(currentAudio.audioLength == currentAudio.audioIndex){
			  /* �жϣ�����ģʽ */
			 if(currentAudio.audio_play_mode == 1)  // ѭ������
			 {
			    currentAudio.audioIndex = 0;            // ��ͷ��ʼ
			 }else   // ���β���
			 {		
				 /* ������� ֹͣ���� */
			   timer_disable(DAC_TIMER);  // �رն�ʱ��
			   audio_status = 2; // ��ʾֹͣ
			 }

		 }	 
	 }
}

/* ��ʼ�������� */
void start_play_audio(const uint8_t *audio,uint32_t audio_length,uint8_t mode)
{
	currentAudio.audioIndex = 0;              // ��ͷ��ʼ
	currentAudio.audioLength = audio_length;  // ��ǰ��Ƶ�ĳ��� 
	currentAudio.audioCurrentPlayP = audio;   // ָ�򲥷ŵ���Ƶ 
	currentAudio.audio_play_mode = mode;     // ����ģʽ
	timer_enable(DAC_TIMER);        // ������ʱ��
}

/* ֹͣ�������� */
void stop_play_audio(void)
{
	timer_disable(DAC_TIMER);  // �رն�ʱ��
}