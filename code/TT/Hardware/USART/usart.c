#include "usart.h"

//USRAT����GPIO������
void usart_gpio_config(void) {
    //����ʱ�ӣ�����ʱ�Ӻ�GPIOʱ�ӣ�
    rcu_periph_clock_enable(BSP_USART_TX_RCU);
    rcu_periph_clock_enable(BSP_USART_RX_RCU);
    rcu_periph_clock_enable(BSP_USART_RCU);
    //����GPIO����ģʽ
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    //����GPIOģʽ
    gpio_mode_set(GPIOA, GPIO_AF_7, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_mode_set(GPIOA, GPIO_AF_7, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    //����GPIO�����
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    //���ô���
}

//USRAT���ڲ���������
void usart_parameter_config(uint32_t baudval) {
    //��λ�豸
    usart_deinit(USART0);//usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    //���ò�����
    usart_baudrate_set(USART0, baudval);//usart_periph,baudval
    //����У�鹦��
    usart_parity_config(USART0, USART_PM_NONE);//usart_periph,(USART_PM_NONE��У��/USART_PM_EVENżУ��/USART_PM_ODD��У��)
    //�������ݳ���
    usart_word_length_set(USART0, USART_WL_8BIT);//usart_periph,(USART_WL_8BIT: 8 bits/USART_WL_9BIT: 9 bits)
    //����ֹͣλ��λ��
    usart_stop_bit_set(USART0, USART_STB_1BIT);//usart_periph,(USART_STB_1BIT/USART_STB_0_5BIT: 0.5 bit(not available for UARTx(x=3,4,6,7))/USART_STB_2BIT:   2 bits/USART_STB_1_5BIT: 1.5 bits(not available for UARTx(x=3,4,6,7)))
    //�����ȷ��͵�λ���Ǹ�λ
    usart_data_first_config(USART0, USART_MSBF_LSB);//usart_periph,(USART_MSBF_MSB �ȷ���λ/USART_MSBF_LSB  �ȷ���λ)
    //����ʹ��
    usart_enable(USART0);//usart_periph
    //������������
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//usart_periph,(USART_TRANSMIT_ENABLE: ʹ�� USART ����/USART_TRANSMIT_DISABLE: ȥʹ�� USART ����)
}

//Ҫ���͵�����
void send_data(uint8_t data) {
    usart_data_transmit(USART0, data);//usart_periph,data
    //�жϻ������Ƿ��Ѿ�����
    FlagStatus state = usart_flag_get(USART0, USART_FLAG_TBE);
    while(RESET == state);
}

//usart����
void usart_run(void) {
    usart_gpio_config();
    usart_parameter_config(115200);
    send_data('a');
}