#include "bluetooth.h"
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "string.h"
#include "protocol.h"
#include "config.h"
//#include "control.h"

#define BLUE_TX_PIN GPIO_PIN_7
#define BLUE_RX_PIN GPIO_PIN_6
#define BLUE_TX_PORT GPIOF
#define BLUE_RX_PORT GPIOF

#define BLUE_UART UART6

#define BLUE_BUFFER_LENGTH 2048

uint8_t 	g_blue_recv_buff[BLUE_BUFFER_LENGTH];   // ���ջ�����
uint16_t 	g_blue_recv_length = 0;	 // �������ݳ���
uint8_t		g_blue_recv_complete_flag = 0;



float g_blue_angle = 0;
float g_blue_velocity = 0;



void blue_usart_config(uint32_t band_rate){
	//1.��ʼʱ��
	//1.1 ����ʱ��
	rcu_periph_clock_enable(RCU_UART6);
	//1.2 PA9ʱ�Ӻ�PA10ʱ��
	rcu_periph_clock_enable(RCU_GPIOF);
	
	//2.����GPIO����ģʽ
	gpio_af_set(BLUE_TX_PORT,GPIO_AF_8,BLUE_TX_PIN); 
	gpio_af_set(BLUE_RX_PORT,GPIO_AF_8,BLUE_RX_PIN); 
	//3.����GPIO��ģʽ
	/* ����TXΪ����ģʽ ����ģʽ */
	gpio_mode_set(BLUE_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLUE_TX_PIN);
	/* ����RXΪ����ģʽ ����ģʽ */
	gpio_mode_set(BLUE_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLUE_RX_PIN); 
	//4.����GPIO�����
	/* ����TXΪ������� 50MHZ */
	gpio_output_options_set(BLUE_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUE_TX_PIN);
	/* ����RXΪ������� 50MHZ */
	gpio_output_options_set(BLUE_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUE_RX_PIN);
	//5.���ô���(���ò���)
	/* ��������*/
	usart_deinit(BLUE_UART); // ��λ����    
	usart_baudrate_set(BLUE_UART,band_rate); // ���ò�����    
	usart_parity_config(BLUE_UART,USART_PM_NONE); // û��У��λ    
	usart_word_length_set(BLUE_UART,USART_WL_8BIT); // 8λ����λ    
	usart_stop_bit_set(BLUE_UART,USART_STB_1BIT); // 1λֹͣλ
	//6.ʹ�ܴ���(����ʹ�ܺͷ���ʹ��)
	/* ʹ�ܴ��� */
	usart_enable(BLUE_UART);                          			 // ʹ�ܴ���
	usart_transmit_config(BLUE_UART,USART_TRANSMIT_ENABLE);  // ʹ�ܴ��ڷ���
	
	
	
	
	//���ڽ���
	usart_receive_config(BLUE_UART,USART_RECEIVE_ENABLE); 
	//7.�ж�����
	//�ж����ȼ�
	nvic_irq_enable(UART6_IRQn, 2, 2);
	//��ȡ���ݻ������ǿ��жϺ͹��ش����ж�
	usart_interrupt_enable(BLUE_UART,USART_INT_RBNE);
	//���м���ж�
	usart_interrupt_enable(BLUE_UART,USART_INT_IDLE);
	
}


//DMA����
void blue_dma_config(void){
	//1.DMA�����ݽṹ��
	dma_single_data_parameter_struct dma_init_struct;
	//2.����DMAʱ��
	rcu_periph_clock_enable(RCU_DMA0);
	//3.��ʼ��DMAͨ��
	dma_deinit(DMA0,DMA_CH3);
	//4.����DMA��ʼ������
	// �����ַ 
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(BLUE_UART);            
	// ��ʹ������ģʽ��Ϊ�̶�ģʽ
  dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;      
	// �ڴ��ַ(������������)
  dma_init_struct.memory0_addr = (uint32_t)g_blue_recv_buff;              
	// ����ģʽ
  dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;      	 
	// һ�δ��䳤��8bit
  dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;         		   
	// �ر�ѭ��ģʽ               
  dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;      		 
	// ���赽�ڴ� 
  dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;          		
	// Ҫ�����������
  dma_init_struct.number = BLUE_BUFFER_LENGTH;         			   
	// �������ȼ�
  dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH; 
	//5.��ʼ��DMA�ṹ��
	dma_single_data_mode_init(DMA0,DMA_CH3,&dma_init_struct);
	//6.ʹ��ͨ������
	dma_channel_subperipheral_select(DMA0,DMA_CH3,DMA_SUBPERI5);
	//7.ʹ��DMAͨ��
	dma_channel_enable(DMA0,DMA_CH3);
	//8.ʹ��DMAͨ���ж�
	dma_interrupt_enable(DMA0,DMA_CH3,DMA_CHXCTL_FTFIE);
	//9.�����ж����ȼ�
	nvic_irq_enable(DMA0_Channel3_IRQn, 2, 1); 
	//10.ʹ�ܴ���DMA����
	usart_dma_receive_config(BLUE_UART,USART_RECEIVE_DMA_ENABLE);
}




//DMA�жϷ�����
void DMA0_Channel3_IRQHandler(void){
	// ��������ж� 
	if(dma_interrupt_flag_get(DMA0,DMA_CH3,DMA_INT_FLAG_FTF) == SET)  
	{	
		// ���жϱ�־λ
	  dma_interrupt_flag_clear(DMA0,DMA_CH3,DMA_INT_FLAG_FTF);				
  }
}


//���ڽ����жϷ�����
void UART6_IRQHandler(void){
	//��⵽֡�ж�
	if(usart_interrupt_flag_get(BLUE_UART,USART_INT_FLAG_IDLE) == SET)												 // ��⵽֡�ж�
	{
		// ����Ҫ������������ֵ����Ҫ
		usart_data_receive(BLUE_UART);																											  // ����Ҫ������������ֵ����Ҫ

		/* ����DMA���յ������� */
		g_blue_recv_length = BLUE_BUFFER_LENGTH - dma_transfer_number_get(DMA1,DMA_CH1);	 // ����ʵ�ʽ��յ����ݳ���
		//g_blue_recv_buff[g_blue_recv_length] = '\0';																											 // ���ݽ�����ϣ����������־
		g_blue_recv_complete_flag = 1;																																 // ������� 
		
		uint8_t pid_type = 0;
		float KP,KI,KD=0;
		float x,y,z;
		printf("protocol start parse: %X %x %d\r\n",g_blue_recv_buff[0],g_blue_recv_buff[1],g_blue_recv_length);
		// ���ݱ�־λȥ����
		if(g_blue_recv_buff[0] == RX_FRAME_HEADER){
			switch(g_blue_recv_buff[1]){
					case 0x00:
						protocol_parse_xyz(g_blue_recv_buff,g_blue_recv_length,&x,&y,&z);
						break;
					case 0x01:	
						
						protocol_parse_rx_pid(g_blue_recv_buff,g_blue_recv_length,&pid_type,&KP,&KI,&KD);
						switch(pid_type){
							case 0x00:
								g_balanceKP = KP;
								g_balanceKI = KI;
								g_balanceKD = KD;
								break;
							case 0x01:
								g_velocityKP = KP;
								g_velocityKI = KI;
								g_velocityKD = KD;
								break;
						}
						break;
					default:
						printf("protocol not parse: %x\r\n",g_blue_recv_buff[1]);
						break;
				
				}
		}
		
		
		
		
		
		// ��������
		//protocol_parse_rx(g1_recv_buff,g1_recv_length);
		
		/* ��������DMA���� */
		// ʧ��DMAͨ��
		dma_channel_disable(DMA0,DMA_CH3);	 																					
		// ��������DMA���д���
		blue_dma_config();  																																					 
		
	}
}



void bluetooth_init(){
//	rcu_periph_clock_enable(RCU_GPIOA);
//	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
//	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
//	
//	gpio_bit_write(GPIOA,GPIO_PIN_4,1);
	
	
	// ���ڳ�ʼ��PF6 RX   PF7 TX
	delay_1ms(72);
	blue_usart_config(9600);
	blue_dma_config();
	
}
void blue_usart_send_data(uint8_t ucch){
	//��������
	usart_data_transmit(BLUE_UART, (uint8_t)ucch);  
	
	//usart_flag_get �ȴ��������ݻ�������־λ ���ݷ�����ɻ᷵��SET��־λ
	while(RESET == usart_flag_get(BLUE_UART, USART_FLAG_TBE)); 
}

void blue_usart_send_string(uint8_t* ucstr){
	while(ucstr && *ucstr){
		blue_usart_send_data(*ucstr++);
	}
}






void blue_test(){	
	bluetooth_init();
	
	// blue_usart_send_string("AT\r\n");
	// delay_1ms(500);	
	
	// blue_usart_send_string("AT+NAME<MrZhuo>\r\n");
	// delay_1ms(500);
	
	// blue_usart_send_string("AT+NAME?\r\n");
	// delay_1ms(500);
	
	// blue_usart_send_string("AT+ADDR?\r\n");
	// delay_1ms(500);
	
	// blue_usart_send_string("AT+RESET\r\n");
	// delay_1ms(1000);
	// printf("init success \r\n");
	
	while(1){
		//blue_usart_send_data(100);
		//blue_usart_send_string((uint8_t*)"hello from gd32\r\n");
		//printf("12");
		if(g_blue_recv_complete_flag){
			 g_blue_recv_complete_flag = 0;
		
				printf("rx_buff:");
				for(int i=0; i<g_blue_recv_length;i++){
					printf(" %d=%d ",i,g_blue_recv_buff[i]);
				}
				printf("\n");
			//printf("receive blue:%u %x %d\n",g_blue_recv_buff[0],g_blue_recv_buff[1],g_blue_recv_length);
			memset(g_blue_recv_buff,0,g_blue_recv_length);
		}
		//delay_1ms(500);
	}
}
