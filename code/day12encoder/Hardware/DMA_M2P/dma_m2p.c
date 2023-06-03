#include "dma_m2p.h"

uint8_t temp[1] = {0};

//*************************[USART关于GPIO的配置]*************************
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
//===================================================

//*************************[USART关于参数的配置]*************************
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
    //接收数据配置
    usart_receive_config(USART_NUM,USART_RECEIVE_ENABLE);

    //接收中断配置
    nvic_irq_enable(USART0_IRQn, 2, 2);
    //配置处理哪些中断
    //USART_INT_RBNE:接收到一个字符就会触发,需要保存接收的字符
    usart_interrupt_enable(USART_NUM, USART_INT_RBNE); // 读数据缓冲区非空中断和过载错误中断
    //USART_INT_IDLE:一帧数据接收完成 hello 可以打印对应的字符串
    usart_interrupt_enable(USART_NUM, USART_INT_IDLE); // DLE线检测中断

    /* 使能串口DMA发送 */
    usart_dma_transmit_config(USART_NUM, USART_TRANSMIT_DMA_ENABLE);
}
//===================================================

//*************************[DMA发送配置]*************************
int32_t usart_dma_tx_init(uint32_t periph_addr)
{
    /* 使能DMA时钟 */
    rcu_periph_clock_enable(BSP_DMA_RCU);
    /* 复位DMA并禁用DMA传输 */
    dma_deinit(BSP_DMA, BSP_DMA_CHANNEL_TX);
    /* 配置DMA传输结构体 */
    dma_single_data_parameter_struct dma_init_struct;
    //DMA传输方向：从内存到外设
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    //传输数据的内存地址
//	dma_init_struct.memory0_addr = (uint32_t)g_send_data;
    //内存地址递增
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    //传输数据的大小
//	dma_init_struct.number = sizeof(g_send_data);
    //外设地址
    dma_init_struct.periph_addr = periph_addr;
    //外设地址不递增
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    //外设地址的数据宽度为8位
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    //DMA传输优先级为最高
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    /* 禁止DMA传输循环模式 */
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    /* 初始化DMA传输 */
    dma_single_data_mode_init(BSP_DMA, BSP_DMA_CHANNEL_TX, &dma_init_struct);
    /* 配置DMA通道的外设连接 */
    dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_CHANNEL_TX, DMA_SUBPERI4);
    return 0;
}
//===================================================

//*************************[通过DMA发送]*************************
/**
  * @brief  调试信息发送接口
  * @param  data：调试数据指针；\
  *	        len：调试数据的数据长度
  * @retval 0或其他错误代码
 **/
int32_t uart_dma_send(uint8_t *data,uint32_t len)
{
    /* 关闭DMA通道 */
    dma_channel_disable(BSP_DMA, BSP_DMA_CHANNEL_TX);
    /* 清除DMA传输完成标志 */
    dma_flag_clear(BSP_DMA, BSP_DMA_CHANNEL_TX, DMA_FLAG_FTF);
    /* 配置DMA传输的内存地址和传输数据大小 */
    dma_memory_address_config(BSP_DMA, BSP_DMA_CHANNEL_TX, DMA_MEMORY_0, (uint32_t)data);
    dma_transfer_number_config(BSP_DMA, BSP_DMA_CHANNEL_TX, len);
    /* 使能DMA通道 */
    dma_channel_enable(BSP_DMA, BSP_DMA_CHANNEL_TX);
    return 0;
}
//===================================================

//*************************[重写func方法]*************************
int fputc(int ch, FILE *f) {
    temp[0] = (uint8_t)ch;
    uart_dma_send(temp, 1);
    while (dma_flag_get(BSP_DMA,BSP_DMA_CHANNEL_TX,DMA_FLAG_FTF) == RESET);
    return 0;
}
//===================================================

//*************************[程序运行]*************************
void usart_dma_config(uint32_t baudval) {
    //gpio初始化
    usart_gpio_config();
    //usart参数配置
    usart_param_config(baudval);
    //USART通过DMA发送
    usart_dma_tx_init((uint32_t)(&USART_DATA(USART_NUM)));
}
//===================================================