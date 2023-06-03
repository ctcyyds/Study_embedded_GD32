#include "usart.h"

//串口接收缓冲区大小
uint8_t 	g_recv_buff[USART_RECEIVE_LENGTH];   // 接收缓冲区
//接收到字符存放的位置
int g_recv_length=0;

void usart_gpio_config(void){
//	● 开启时钟（包括串口时钟和GPIO时钟）
	rcu_periph_clock_enable(USART_TX_RX_RCU);
	rcu_periph_clock_enable(USART_RCU);
//	● 配置GPIO复用模式
	gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_10);
	gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_9);
//	● 配置GPIO的模式
	gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);
	gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);
//	● 配置GPIO的输出
	/* 配置TX为推挽输出 50MHZ */
	gpio_output_options_set(USART_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
/* 配置RX为推挽输出 50MHZ */
	gpio_output_options_set(USART_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
}

void usart_param_config(uint32_t baudval){
	//USART复位
	usart_deinit(USART_NUM);
	//波特率
	usart_baudrate_set(USART_NUM,baudval);
	//校验位
	usart_parity_config(USART_NUM,USART_PM_NONE);
	//一个周期发送的数据位数
	usart_word_length_set(USART_NUM,USART_WL_8BIT);
	//停止位
	usart_stop_bit_set(USART_NUM,USART_STB_1BIT);
	//先发送高位还是低位
	usart_data_first_config(USART_NUM,USART_MSBF_LSB);
	
	
	//串口使能
	usart_enable(USART_NUM);
	//发送功能配置
	usart_transmit_config(USART_NUM,USART_TRANSMIT_ENABLE);
	//接收配置
	usart_receive_config(USART_NUM,USART_RECEIVE_ENABLE);
	//接收中断配置
	nvic_irq_enable(USART0_IRQn, 2, 2);
	//配置处理哪些中断
	//USART_INT_RBNE:接收到一个字符就会触发,需要保存接收的字符
	usart_interrupt_enable(USART_NUM, USART_INT_RBNE); // 读数据缓冲区非空中断和过载错误中断 
	//USART_INT_IDLE:一帧数据接收完成 hello 可以打印对应的字符串
	usart_interrupt_enable(USART_NUM, USART_INT_IDLE); // DLE线检测中断
}
//串口接收中断处理函数
void USART0_IRQHandler(void){
	if ((usart_interrupt_flag_get(USART_NUM, USART_INT_FLAG_RBNE))==SET) {
        uint16_t value = usart_data_receive(USART_NUM);
        g_recv_buff[g_recv_length] = value;		
        g_recv_length++;
    }
		if (usart_interrupt_flag_get(USART_NUM, USART_INT_FLAG_IDLE)==SET) {
        //读取缓冲区,清空缓冲区
				usart_data_receive(USART_NUM); 
				g_recv_buff[g_recv_length] = '\0';
				g_recv_length = 0;
				printf("data %s\n",g_recv_buff);
		}
}

void usart_all_config(uint32_t baudval){
	//gpio初始化
	usart_gpio_config();
	//usart参数配置
	usart_param_config(115200);
}

//发送一byte数据
void send_data(uint8_t data){
	//通过USART发送
	usart_data_transmit(USART_NUM,data);
	//判断缓冲区是否已经空了
//	FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
	while(RESET == usart_flag_get(USART_NUM,USART_FLAG_TBE));
}

//发送字符串
void send_string(char *data){
	//满足: 1.data指针不为空  2.发送的数据不是\0结束标记
	while(data && *data){
		send_data((uint8_t)(*data));
		data++;
	}
}

//重写fputc方法  调用printf,会自动调用这个方法实现打印
int fputc(int ch, FILE *f){
	send_data((uint8_t)ch);
	return 0;
}


void usart_run(void){
	//gpio初始化
	usart_gpio_config();
	//usart参数配置
	usart_param_config(115200);
	//发送数据
//	send_data('a');
//	send_data('b');
//	send_data('a');
//	send_data('a');
//	send_string("hello");
	printf("hello");
}
