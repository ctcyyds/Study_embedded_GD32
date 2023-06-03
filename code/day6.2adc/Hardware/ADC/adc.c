#include "adc.h"
#include "stdio.h"

//#define DEBUG   // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ

uint16_t 	adcXValue,adcYValue;


/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
static void adc_gpio_init(void)
{
    /* enable the clock */
    rcu_periph_clock_enable(ADCX_RCU);
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */
    gpio_mode_set(ADCX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCX_PIN);

    /* enable the clock */
    rcu_periph_clock_enable(ADCY_RCU);
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG ģ���ź�*/
    gpio_mode_set(ADCY_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCY_PIN);
}

void adc_config(void)
{
    /* adc ���ų�ʼ�� */
    adc_gpio_init();
    /* ʹ��ADCʱ�� */
    rcu_periph_clock_enable(RCU_ADC0);
    /* ����ʱ��������ADCʱ�ӷ�Ƶ */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
    /* ��λADC */
    adc_deinit();
    /* ����ADC����ģʽ */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // ����ADC�������ڶ���ģʽ

    /* ����ADCת��ģʽ */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); // �ر�����ģʽ
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE); // �ر�ɨ��ģʽ

    /* ����ADC���ݶ��뷽ʽ */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT); // LSB���룬��λ����

    /* ����ADCͨ������ */
    adc_channel_length_config(ADC0,ADC_ROUTINE_CHANNEL,1U); //  ADC����ͨ�� ����Ϊ1

    /* ʹ��ADCģ�� */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADCУ׼ */
    adc_calibration_enable(ADC0);  // ADCУ׼
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
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); // ADC�������ʹ��

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return routine channel sample value */
    return (adc_routine_data_read(ADC0));
}


/* ҡ��ɨ�躯�� */
void adc_key_scan(void)
{
    
        adcXValue = adc_channel_sample(ADC_CHANNEL_1);  // ����
			printf("x:%d\r",adcXValue);
        adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // ����
			printf("y:%d\r\n",adcYValue);
    

		
}