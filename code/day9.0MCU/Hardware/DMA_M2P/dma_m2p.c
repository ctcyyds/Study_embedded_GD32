#include "dma_m2p.h"

uint8_t temp[1] = {0};

//*************************[USART����GPIO������]*************************
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
//===================================================

//*************************[USART���ڲ���������]*************************
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
    //������������
    usart_receive_config(USART_NUM,USART_RECEIVE_ENABLE);

    //�����ж�����
    nvic_irq_enable(USART0_IRQn, 2, 2);
    //���ô�����Щ�ж�
    //USART_INT_RBNE:���յ�һ���ַ��ͻᴥ��,��Ҫ������յ��ַ�
    usart_interrupt_enable(USART_NUM, USART_INT_RBNE); // �����ݻ������ǿ��жϺ͹��ش����ж�
    //USART_INT_IDLE:һ֡���ݽ������ hello ���Դ�ӡ��Ӧ���ַ���
    usart_interrupt_enable(USART_NUM, USART_INT_IDLE); // DLE�߼���ж�

    /* ʹ�ܴ���DMA���� */
    usart_dma_transmit_config(USART_NUM, USART_TRANSMIT_DMA_ENABLE);
}
//===================================================

//*************************[DMA��������]*************************
int32_t usart_dma_tx_init(uint32_t periph_addr)
{
    /* ʹ��DMAʱ�� */
    rcu_periph_clock_enable(BSP_DMA_RCU);
    /* ��λDMA������DMA���� */
    dma_deinit(BSP_DMA, BSP_DMA_CHANNEL_TX);
    /* ����DMA����ṹ�� */
    dma_single_data_parameter_struct dma_init_struct;
    //DMA���䷽�򣺴��ڴ浽����
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    //�������ݵ��ڴ��ַ
//	dma_init_struct.memory0_addr = (uint32_t)g_send_data;
    //�ڴ��ַ����
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    //�������ݵĴ�С
//	dma_init_struct.number = sizeof(g_send_data);
    //�����ַ
    dma_init_struct.periph_addr = periph_addr;
    //�����ַ������
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    //�����ַ�����ݿ��Ϊ8λ
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    //DMA�������ȼ�Ϊ���
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    /* ��ֹDMA����ѭ��ģʽ */
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    /* ��ʼ��DMA���� */
    dma_single_data_mode_init(BSP_DMA, BSP_DMA_CHANNEL_TX, &dma_init_struct);
    /* ����DMAͨ������������ */
    dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_CHANNEL_TX, DMA_SUBPERI4);
    return 0;
}
//===================================================

//*************************[ͨ��DMA����]*************************
/**
  * @brief  ������Ϣ���ͽӿ�
  * @param  data����������ָ�룻\
  *	        len���������ݵ����ݳ���
  * @retval 0�������������
 **/
int32_t uart_dma_send(uint8_t *data,uint32_t len)
{
    /* �ر�DMAͨ�� */
    dma_channel_disable(BSP_DMA, BSP_DMA_CHANNEL_TX);
    /* ���DMA������ɱ�־ */
    dma_flag_clear(BSP_DMA, BSP_DMA_CHANNEL_TX, DMA_FLAG_FTF);
    /* ����DMA������ڴ��ַ�ʹ������ݴ�С */
    dma_memory_address_config(BSP_DMA, BSP_DMA_CHANNEL_TX, DMA_MEMORY_0, (uint32_t)data);
    dma_transfer_number_config(BSP_DMA, BSP_DMA_CHANNEL_TX, len);
    /* ʹ��DMAͨ�� */
    dma_channel_enable(BSP_DMA, BSP_DMA_CHANNEL_TX);
    return 0;
}
//===================================================

//*************************[��дfunc����]*************************
int fputc(int ch, FILE *f) {
    temp[0] = (uint8_t)ch;
    uart_dma_send(temp, 1);
    while (dma_flag_get(BSP_DMA,BSP_DMA_CHANNEL_TX,DMA_FLAG_FTF) == RESET);
    return 0;
}
//===================================================

//*************************[��������]*************************
void usart_dma_config(uint32_t baudval) {
    //gpio��ʼ��
    usart_gpio_config();
    //usart��������
    usart_param_config(baudval);
    //USARTͨ��DMA����
    usart_dma_tx_init((uint32_t)(&USART_DATA(USART_NUM)));
}
//===================================================