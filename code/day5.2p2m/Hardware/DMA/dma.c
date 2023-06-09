#include "dma.h"

//uint8_t g_send_data[256];//发送数据
uint8_t g_recv_data[256];//接收数据
uint8_t temp[1] = {0};

//*************************[USART关于GPIO的配置]*************************
void usart_gpio_config(void) {
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
//===================================================

//*************************[USART参数配置]*************************
void usart_param_config(uint32_t baudval) {
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

    /* 使能串口DMA发送 */
    usart_dma_transmit_config(USART_NUM, USART_TRANSMIT_DMA_ENABLE);
    /* 使能串口DMA接收 */
    usart_dma_receive_config(USART_NUM, USART_RECEIVE_DMA_ENABLE);

    //接收中断配置
    nvic_irq_enable(USART0_IRQn, 2, 2);
    //USART_INT_IDLE:一帧数据接收完成 hello 可以打印对应的字符串
    usart_interrupt_enable(USART_NUM, USART_INT_IDLE); // DLE线检测中断
}
//===================================================

//*************************[USART关于DMA中断发送初始化]*************************
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

//*************************[USART关于DMA中断接收初始化]*************************
int32_t usart_dma_rx_init(uint32_t periph_addr)
{
    /* 使能DMA时钟 */
    rcu_periph_clock_enable(BSP_DMA_RCU);
    /* 复位DMA并禁用DMA传输 */
    dma_deinit(BSP_DMA, BSP_DMA_CHANNEL_RX);
    /* 配置DMA传输结构体 */
    dma_single_data_parameter_struct dma_parameter;
    // DMA传输方向：从外设到内存
    dma_parameter.direction = DMA_PERIPH_TO_MEMORY;
    // 外设地址，指向要接收数据的外设寄存器地址
    dma_parameter.periph_addr = periph_addr;
    // 外设地址不递增
    dma_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    // 外设地址的数据宽度为8位
    dma_parameter.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    // 传输数据的内存地址，指向接收数据的缓存区
    dma_parameter.memory0_addr = (uint32_t)g_recv_data;
    // 内存地址递增
    dma_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    // 传输数据的大小，单位为字节
    dma_parameter.number = sizeof(g_recv_data);
    // DMA传输优先级为最高
    dma_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    // 非循环传输模式
    dma_parameter.circular_mode = DMA_CIRCULAR_MODE_DISABLE;

    dma_single_data_mode_init(BSP_DMA, BSP_DMA_CHANNEL_RX, &dma_parameter);
    /* 配置DMA通道的外设连接 */
    dma_channel_subperipheral_select(BSP_DMA, BSP_DMA_CHANNEL_RX, DMA_SUBPERI4);
    /* 使能DMA传输 */
    dma_channel_enable(BSP_DMA, BSP_DMA_CHANNEL_RX);
    //9.配置中断优先级
    nvic_irq_enable(DMA1_Channel2_IRQn, 2, 2);
//	//8.使能DMA通道中断
//	dma_interrupt_enable(BSP_DMA,BSP_DMA_CHANNEL_RX,DMA_CHXCTL_FTFIE);
//
    dma_interrupt_flag_clear(BSP_DMA,BSP_DMA_CHANNEL_RX,DMA_INT_FLAG_FTF);
    return 0;
}
//===================================================

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

//*************************[重写fputc方法  调用printf,会自动调用这个方法实现打印]*************************
int fputc(int ch, FILE *f) {
    temp[0] = (uint8_t)ch;
    uart_dma_send(temp, 1);
    while (dma_flag_get(BSP_DMA,BSP_DMA_CHANNEL_TX,DMA_FLAG_FTF) == RESET);
    return 0;
}
//===================================================

//*************************[USART中断服务函数]*************************
void USART0_IRQHandler(void)
{
    printf("usart idle\n");
    uint32_t recv_len = 0;
    if (usart_interrupt_flag_get(USART_NUM, USART_INT_FLAG_IDLE) != RESET)
    {
        /* 清除串口空闲中断标志 */
        usart_interrupt_flag_clear(USART_NUM, USART_INT_FLAG_IDLE);
        //清空缓冲区
        usart_data_receive(USART_NUM);

        /* 关闭 DMA 接收通道 */
        dma_channel_disable(BSP_DMA, BSP_DMA_CHANNEL_RX);

        /* 获取接收到的数据长度 */
        recv_len = sizeof(g_recv_data) - dma_transfer_number_get(BSP_DMA, BSP_DMA_CHANNEL_RX);
        if (recv_len != 0 && recv_len < sizeof(g_recv_data))
        {
            /* 将接收到的数据用 DMA 传输发送出去 */
            g_recv_data[recv_len] = '\0';
            printf("%s\n",g_recv_data);
            /* 配置 DMA 接收传输的内存地址和传输数据大小 */
//            dma_memory_address_config(BSP_DMA, BSP_DMA_CHANNEL_RX, DMA_MEMORY_0, (uint32_t)g_recv_data);
//            dma_transfer_number_config(BSP_DMA, BSP_DMA_CHANNEL_RX, sizeof(g_recv_data));
            //清除DMA传输完成标志位，以便下一次DMA传输能够正常进行
            dma_flag_clear(BSP_DMA, BSP_DMA_CHANNEL_RX, DMA_FLAG_FTF);
            /* 重新开启 DMA 接收通道 */
            dma_channel_enable(BSP_DMA, BSP_DMA_CHANNEL_RX);
        } else {
            memset(g_recv_data, 0, sizeof(g_recv_data));
        }
    }
    return;
}
//===================================================

//*************************[USART关于DMA发送和接收数据实现]*************************
void usart_dma_config(uint32_t baudval) {
    //gpio初始化
    usart_gpio_config();
    //usart参数配置
    usart_param_config(baudval);
    //USART通过DMA发送
    usart_dma_tx_init((uint32_t)(&USART_DATA(USART_NUM)));
    //USART通过DMA接收
    usart_dma_rx_init((uint32_t)(&USART_DATA(USART_NUM)));
}
//===================================================