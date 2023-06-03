#include "usart.h"

//���ڽ��ջ�������С
uint8_t 	g_recv_buff[USART_RECEIVE_LENGTH];   // ���ջ�����
//���յ��ַ���ŵ�λ��
int g_recv_length=0;

void usart_gpio_config(void){
//	�� ����ʱ�ӣ���������ʱ�Ӻ�GPIOʱ�ӣ�
	rcu_periph_clock_enable(USART_TX_RX_RCU);
	rcu_periph_clock_enable(USART_RCU);
//	�� ����GPIO����ģʽ
	gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_10);
	gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_9);
//	�� ����GPIO��ģʽ
	gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);
	gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);
//	�� ����GPIO�����
	/* ����TXΪ������� 50MHZ */
	gpio_output_options_set(USART_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
/* ����RXΪ������� 50MHZ */
	gpio_output_options_set(USART_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
}

void usart_param_config(uint32_t baudval){
	//USART��λ
	usart_deinit(USART_NUM);
	//������
	usart_baudrate_set(USART_NUM,baudval);
	//У��λ
	usart_parity_config(USART_NUM,USART_PM_NONE);
	//һ�����ڷ��͵�����λ��
	usart_word_length_set(USART_NUM,USART_WL_8BIT);
	//ֹͣλ
	usart_stop_bit_set(USART_NUM,USART_STB_1BIT);
	//�ȷ��͸�λ���ǵ�λ
	usart_data_first_config(USART_NUM,USART_MSBF_LSB);
	
	
	//����ʹ��
	usart_enable(USART_NUM);
	//���͹�������
	usart_transmit_config(USART_NUM,USART_TRANSMIT_ENABLE);
	//��������
	usart_receive_config(USART_NUM,USART_RECEIVE_ENABLE);
	//�����ж�����
	nvic_irq_enable(USART0_IRQn, 2, 2);
	//���ô�����Щ�ж�
	//USART_INT_RBNE:���յ�һ���ַ��ͻᴥ��,��Ҫ������յ��ַ�
	usart_interrupt_enable(USART_NUM, USART_INT_RBNE); // �����ݻ������ǿ��жϺ͹��ش����ж� 
	//USART_INT_IDLE:һ֡���ݽ������ hello ���Դ�ӡ��Ӧ���ַ���
	usart_interrupt_enable(USART_NUM, USART_INT_IDLE); // DLE�߼���ж�
}
//���ڽ����жϴ�����
void USART0_IRQHandler(void){
	if ((usart_interrupt_flag_get(USART_NUM, USART_INT_FLAG_RBNE))==SET) {
        uint16_t value = usart_data_receive(USART_NUM);
        g_recv_buff[g_recv_length] = value;		
        g_recv_length++;
    }
		if (usart_interrupt_flag_get(USART_NUM, USART_INT_FLAG_IDLE)==SET) {
        //��ȡ������,��ջ�����
				usart_data_receive(USART_NUM); 
				g_recv_buff[g_recv_length] = '\0';
				g_recv_length = 0;
				printf("data %s\n",g_recv_buff);
		}
}

void usart_all_config(uint32_t baudval){
	//gpio��ʼ��
	usart_gpio_config();
	//usart��������
	usart_param_config(115200);
}

//����һbyte����
void send_data(uint8_t data){
	//ͨ��USART����
	usart_data_transmit(USART_NUM,data);
	//�жϻ������Ƿ��Ѿ�����
//	FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
	while(RESET == usart_flag_get(USART_NUM,USART_FLAG_TBE));
}

//�����ַ���
void send_string(char *data){
	//����: 1.dataָ�벻Ϊ��  2.���͵����ݲ���\0�������
	while(data && *data){
		send_data((uint8_t)(*data));
		data++;
	}
}

//��дfputc����  ����printf,���Զ������������ʵ�ִ�ӡ
int fputc(int ch, FILE *f){
	send_data((uint8_t)ch);
	return 0;
}


void usart_run(void){
	//gpio��ʼ��
	usart_gpio_config();
	//usart��������
	usart_param_config(115200);
	//��������
//	send_data('a');
//	send_data('b');
//	send_data('a');
//	send_data('a');
//	send_string("hello");
	printf("hello");
}
