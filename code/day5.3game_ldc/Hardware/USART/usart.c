#include "usart.h"
#include "dma.h"

//串口接收缓冲区大小
uint8_t 	g_recv_buff[USART_RECEIVE_LENGTH];   // 接收缓冲区
//接收到字符存放的位置
int g_recv_length=0;

//USART关于GPIO的配置
void usart_gpio_config(void) {
    //开启时钟（串口时钟和GPIO时钟）
    rcu_periph_clock_enable(BSP_USART_TX_RX_RCU);
    rcu_periph_clock_enable(BSP_USART_RCU);
    //配置GPIO复用模式
    gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_10);
    gpio_af_set(USART_GPIO,GPIO_AF_7,GPIO_PIN_9);
    //配置GPIO模式
    gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_mode_set(USART_GPIO,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);
    //配置GPIO的输出
    gpio_output_options_set(USART_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_output_options_set(USART_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);


}

//USART关于参数的配置
void usart_param_config(uint32_t baudval) {
    //复位设备
    usart_deinit(USART_NUM);//usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    //设置波特率
    usart_baudrate_set(USART_NUM,baudval);//usart_periph,baudval
    //配置校验功能
    usart_parity_config(USART_NUM,USART_PM_NONE);//usart_periph,(USART_PM_NONE无校验/USART_PM_EVEN偶校验/USART_PM_ODD奇校验)
    //设置数据长度
    usart_word_length_set(USART_NUM,USART_WL_8BIT);//usart_periph,(USART_WL_8BIT: 8 bits/USART_WL_9BIT: 9 bits)
    //设置停止位的位数
    usart_stop_bit_set(USART_NUM,USART_STB_1BIT);//usart_periph,(USART_STB_1BIT/USART_STB_0_5BIT: 0.5 bit(not available for UARTx(x=3,4,6,7))/USART_STB_2BIT:   2 bits/USART_STB_1_5BIT: 1.5 bits(not available for UARTx(x=3,4,6,7)))
    //设置先发送低位还是高位
    usart_data_first_config(USART_NUM,USART_MSBF_LSB);//usart_periph,(USART_MSBF_MSB 先发高位/USART_MSBF_LSB  先发低位)
    //串口使能
    usart_enable(USART_NUM);//usart_periph
    //发送数据配置
    usart_transmit_config(USART_NUM,USART_TRANSMIT_ENABLE);//usart_periph,(USART_TRANSMIT_ENABLE: 使能 USART 发送/USART_TRANSMIT_DISABLE: 去使能 USART 发送)
}

//串口接收中断处理函数
void USART0_IRQHandler(void){
	if(usart_interrupt_flag_get(USART_NUM,USART_INT_FLAG_RBNE)==SET){
		//1.有USART_INT_RBNE中断,说明有字符过来,保存下来
		uint16_t data = usart_data_receive(USART_NUM);
		g_recv_buff[g_recv_length] = usart_data_receive(USART_NUM);
		memcpy((g_recv_buff+g_recv_length),&data,sizeof(uint16_t));
		g_recv_length+=sizeof(uint16_t);
		usart_interrupt_flag_clear(USART_NUM,USART_INT_FLAG_RBNE);
		printf("rbne\n");	
	}
	if(usart_interrupt_flag_get(USART_NUM,USART_INT_FLAG_IDLE)==SET){
		//2.有USART_INT_IDLE中断,需要打印结果 hello
		//['h','e','l','l','0','\0']
		//清空缓冲区
		usart_data_receive(USART_NUM);
		//打印接收的数据
//		printf("idle\n");
		g_recv_buff[g_recv_length] = '\0';
		printf("g_recv %d\n",strlen((char*)g_recv_buff));
		//修改下一次添加的索引
		g_recv_length = 0;
		usart_interrupt_flag_clear(USART_NUM,USART_INT_FLAG_IDLE);
	}
}

void usart_all_config(uint32_t baudval){
	//gpio初始化
	usart_gpio_config();
	//usart参数配置
	usart_param_config(115200);
}

//要发送的数据
void send_data(uint8_t data) {
    usart_data_transmit(USART_NUM,data);//usart_periph,data
    //判断缓冲区是否已经空了
    while(RESET == usart_flag_get(USART_NUM,USART_FLAG_TBE));
}

//发送字符串
void send_string(char *data) {
    //满足data指针不为空，发送的数据不是\0结束标记
    while(data && *data) {
        send_data((uint8_t)(*data));
        data++;
    }
}

//printf重定向
int fputc(int ch, FILE *f) {
    send_data((uint8_t)ch);
    return 0;
}

//usart运行
void usart_run(void) {
    usart_gpio_config();
    usart_param_config(115200);
    printf("hello");
}