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

uint8_t 	g_blue_recv_buff[BLUE_BUFFER_LENGTH];   // 接收缓冲区
uint16_t 	g_blue_recv_length = 0;	 // 接收数据长度
uint8_t		g_blue_recv_complete_flag = 0;



float g_blue_angle = 0;
float g_blue_velocity = 0;



void blue_usart_config(uint32_t band_rate){
	//1.开始时钟
	//1.1 串口时钟
	rcu_periph_clock_enable(RCU_UART6);
	//1.2 PA9时钟和PA10时钟
	rcu_periph_clock_enable(RCU_GPIOF);
	
	//2.配置GPIO复用模式
	gpio_af_set(BLUE_TX_PORT,GPIO_AF_8,BLUE_TX_PIN); 
	gpio_af_set(BLUE_RX_PORT,GPIO_AF_8,BLUE_RX_PIN); 
	//3.配置GPIO的模式
	/* 配置TX为复用模式 上拉模式 */
	gpio_mode_set(BLUE_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLUE_TX_PIN);
	/* 配置RX为复用模式 上拉模式 */
	gpio_mode_set(BLUE_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLUE_RX_PIN); 
	//4.配置GPIO的输出
	/* 配置TX为推挽输出 50MHZ */
	gpio_output_options_set(BLUE_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUE_TX_PIN);
	/* 配置RX为推挽输出 50MHZ */
	gpio_output_options_set(BLUE_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLUE_RX_PIN);
	//5.配置串口(配置参数)
	/* 串口配置*/
	usart_deinit(BLUE_UART); // 复位串口    
	usart_baudrate_set(BLUE_UART,band_rate); // 设置波特率    
	usart_parity_config(BLUE_UART,USART_PM_NONE); // 没有校验位    
	usart_word_length_set(BLUE_UART,USART_WL_8BIT); // 8位数据位    
	usart_stop_bit_set(BLUE_UART,USART_STB_1BIT); // 1位停止位
	//6.使能串口(串口使能和发送使能)
	/* 使能串口 */
	usart_enable(BLUE_UART);                          			 // 使能串口
	usart_transmit_config(BLUE_UART,USART_TRANSMIT_ENABLE);  // 使能串口发送
	
	
	
	
	//串口接收
	usart_receive_config(BLUE_UART,USART_RECEIVE_ENABLE); 
	//7.中断配置
	//中断优先级
	nvic_irq_enable(UART6_IRQn, 2, 2);
	//读取数据缓冲区非空中断和过载错误中断
	usart_interrupt_enable(BLUE_UART,USART_INT_RBNE);
	//空闲检测中断
	usart_interrupt_enable(BLUE_UART,USART_INT_IDLE);
	
}


//DMA配置
void blue_dma_config(void){
	//1.DMA单数据结构体
	dma_single_data_parameter_struct dma_init_struct;
	//2.开启DMA时钟
	rcu_periph_clock_enable(RCU_DMA0);
	//3.初始化DMA通道
	dma_deinit(DMA0,DMA_CH3);
	//4.配置DMA初始化参数
	// 外设地址 
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(BLUE_UART);            
	// 不使用增量模式，为固定模式
  dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;      
	// 内存地址(用来接收数据)
  dma_init_struct.memory0_addr = (uint32_t)g_blue_recv_buff;              
	// 增量模式
  dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;      	 
	// 一次传输长度8bit
  dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;         		   
	// 关闭循环模式               
  dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;      		 
	// 外设到内存 
  dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;          		
	// 要传输的数据量
  dma_init_struct.number = BLUE_BUFFER_LENGTH;         			   
	// 超高优先级
  dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH; 
	//5.初始化DMA结构体
	dma_single_data_mode_init(DMA0,DMA_CH3,&dma_init_struct);
	//6.使能通道外设
	dma_channel_subperipheral_select(DMA0,DMA_CH3,DMA_SUBPERI5);
	//7.使能DMA通道
	dma_channel_enable(DMA0,DMA_CH3);
	//8.使能DMA通道中断
	dma_interrupt_enable(DMA0,DMA_CH3,DMA_CHXCTL_FTFIE);
	//9.配置中断优先级
	nvic_irq_enable(DMA0_Channel3_IRQn, 2, 1); 
	//10.使能串口DMA接收
	usart_dma_receive_config(BLUE_UART,USART_RECEIVE_DMA_ENABLE);
}




//DMA中断服务函数
void DMA0_Channel3_IRQHandler(void){
	// 传输完成中断 
	if(dma_interrupt_flag_get(DMA0,DMA_CH3,DMA_INT_FLAG_FTF) == SET)  
	{	
		// 清中断标志位
	  dma_interrupt_flag_clear(DMA0,DMA_CH3,DMA_INT_FLAG_FTF);				
  }
}


//串口接收中断服务函数
void UART6_IRQHandler(void){
	//检测到帧中断
	if(usart_interrupt_flag_get(BLUE_UART,USART_INT_FLAG_IDLE) == SET)												 // 检测到帧中断
	{
		// 必须要读，读出来的值不能要
		usart_data_receive(BLUE_UART);																											  // 必须要读，读出来的值不能要

		/* 处理DMA接收到的数据 */
		g_blue_recv_length = BLUE_BUFFER_LENGTH - dma_transfer_number_get(DMA1,DMA_CH1);	 // 计算实际接收的数据长度
		//g_blue_recv_buff[g_blue_recv_length] = '\0';																											 // 数据接收完毕，数组结束标志
		g_blue_recv_complete_flag = 1;																																 // 接收完成 
		
		uint8_t pid_type = 0;
		float KP,KI,KD=0;
		float x,y,z;
		printf("protocol start parse: %X %x %d\r\n",g_blue_recv_buff[0],g_blue_recv_buff[1],g_blue_recv_length);
		// 根据标志位去解析
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
		
		
		
		
		
		// 解析数据
		//protocol_parse_rx(g1_recv_buff,g1_recv_length);
		
		/* 重新设置DMA传输 */
		// 失能DMA通道
		dma_channel_disable(DMA0,DMA_CH3);	 																					
		// 重新配置DMA进行传输
		blue_dma_config();  																																					 
		
	}
}



void bluetooth_init(){
//	rcu_periph_clock_enable(RCU_GPIOA);
//	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
//	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
//	
//	gpio_bit_write(GPIOA,GPIO_PIN_4,1);
	
	
	// 串口初始化PF6 RX   PF7 TX
	delay_1ms(72);
	blue_usart_config(9600);
	blue_dma_config();
	
}
void blue_usart_send_data(uint8_t ucch){
	//发送数据
	usart_data_transmit(BLUE_UART, (uint8_t)ucch);  
	
	//usart_flag_get 等待发送数据缓冲区标志位 数据发送完成会返回SET标志位
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
