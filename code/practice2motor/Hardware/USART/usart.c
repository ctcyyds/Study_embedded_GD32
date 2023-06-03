#include "usart.h"

//USART����GPIO������
void usart_gpio_config(void) {
    //����ʱ�ӣ�����ʱ�Ӻ�GPIOʱ�ӣ�
    rcu_periph_clock_enable(BSP_USART_TX_RX_RCU);
    rcu_periph_clock_enable(BSP_USART_RCU);
    //����GPIO����ģʽ
    gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_10);
    gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_9);
    //����GPIOģʽ
    gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);
    //����GPIO�����
    gpio_output_options_set(USART_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_output_options_set(USART_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);


}

//USART���ڲ���������
void usart_param_config(uint32_t baudval) {
    //��λ�豸
    usart_deinit(USART_NUM);//usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    //���ò�����
    usart_baudrate_set(USART_NUM,baudval);//usart_periph,baudval
    //����У�鹦��
    usart_parity_config(USART_NUM,USART_PM_NONE);//usart_periph,(USART_PM_NONE��У��/USART_PM_EVENżУ��/USART_PM_ODD��У��)
    //�������ݳ���
    usart_word_length_set(USART_NUM,USART_WL_8BIT);//usart_periph,(USART_WL_8BIT: 8 bits/USART_WL_9BIT: 9 bits)
    //����ֹͣλ��λ��
    usart_stop_bit_set(USART_NUM,USART_STB_1BIT);//usart_periph,(USART_STB_1BIT/USART_STB_0_5BIT: 0.5 bit(not available for UARTx(x=3,4,6,7))/USART_STB_2BIT:   2 bits/USART_STB_1_5BIT: 1.5 bits(not available for UARTx(x=3,4,6,7)))
    //�����ȷ��͵�λ���Ǹ�λ
    usart_data_first_config(USART_NUM,USART_MSBF_LSB);//usart_periph,(USART_MSBF_MSB �ȷ���λ/USART_MSBF_LSB  �ȷ���λ)
    //����ʹ��
    usart_enable(USART_NUM);//usart_periph
    //������������
    usart_transmit_config(USART_NUM,USART_TRANSMIT_ENABLE);//usart_periph,(USART_TRANSMIT_ENABLE: ʹ�� USART ����/USART_TRANSMIT_DISABLE: ȥʹ�� USART ����)
}

//Ҫ���͵�����
void send_data(uint8_t data) {
    usart_data_transmit(USART_NUM,data);//usart_periph,data
    //�жϻ������Ƿ��Ѿ�����
    while(RESET == usart_flag_get(USART_NUM,USART_FLAG_TBE));
}

//�����ַ���
void send_string(char *data) {
    //����dataָ�벻Ϊ�գ����͵����ݲ���\0�������
    while(data && *data) {
        send_data((uint8_t)(*data));
        data++;
    }
}

//printf�ض���
int fputc(int ch, FILE *f) {
    send_data((uint8_t)ch);
    return 0;
}

//usart����
void usart_run(void) {
    usart_gpio_config();
    usart_param_config(115200);
    printf("�����\n");
}