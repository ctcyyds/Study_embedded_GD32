#include "adc.h"
#include "stdio.h"

//#define DEBUG   // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ

uint16_t 	adcXValue,adcYValue;


/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
static void adc_gpio_init(void)
{
		/* enable the clock */
    rcu_periph_clock_enable(ADCX_RCU);
    //GPIOģʽ��Ҫ����Ϊģ��ģʽ
    gpio_mode_set(ADCX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCX_PIN);
  
		/* enable the clock */
    rcu_periph_clock_enable(ADCY_RCU);
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG ģ���ź�*/ 
    gpio_mode_set(ADCY_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCY_PIN);  
}

void adc_config(rcu_periph_enum periph,uint32_t adc_periph)
{		
		/* adc ���ų�ʼ�� */
		adc_gpio_init();
		/* ʹ��ADCʱ�� */
    rcu_periph_clock_enable(periph);
		/* ����ʱ��������ADCʱ�ӷ�Ƶ */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
		/* ��λADC */
		adc_deinit();
	  /* ����ADC����ģʽ */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // ����ADC�������ڶ���ģʽ
		
    /* ����ADCת��ģʽ  �ر�����ת��ģʽ */
    adc_special_function_config(adc_periph, ADC_CONTINUOUS_MODE, DISABLE); // �ر�����ģʽ
    /* �����ɼ�ģʽ����Ҫscan */
    adc_special_function_config(adc_periph, ADC_SCAN_MODE, DISABLE); // �ر�ɨ��ģʽ
		
		/* ����ADC���ݶ��뷽ʽ RIGHT:��λ����  LEFT:��λ����*/
    adc_data_alignment_config(adc_periph,ADC_DATAALIGN_RIGHT); // LSB���룬��λ����
	
    /* ����ADCͨ������ Ĭ�ϵ� REGULER*/
    adc_channel_length_config(adc_periph,ADC_ROUTINE_CHANNEL,1U); //  ADC����ͨ�� ����Ϊ1

    /* ʹ��ADCģ�� */
    adc_enable(adc_periph);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADCУ׼ ����:���Ը��㶨��ѹ �������Ƿ�͸����ĵ�ѹ��ͬ*/
    adc_calibration_enable(adc_periph);  // ADCУ׼
    /* wait for ADC stability */
    delay_1ms(1);
}


/*!
    \brief      ADC channel sample  ADCͨ������
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t adc_channel_sample(uint8_t channel)
{
    /* ADC routine channel config */
    adc_routine_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15���������� 
    /* ADC software trigger enable */
		//�������adcת��
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); // ADC�������ʹ��

    //�ȴ�ת�����
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    //���ת����ɱ�־
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    //��ȡת����� 0-4096  (12�ֱ���)
    return (adc_routine_data_read(ADC0));
}


/* ҡ��ɨ�躯�� */
void adc_key_scan(void)
{	
	while(1){
		adcXValue = adc_channel_sample(ADC_CHANNEL_1);  // ����
		adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // ����
		printf("x:%d,y:%d\n",adcXValue,adcYValue);
		delay_1ms(500);
	}
	
}