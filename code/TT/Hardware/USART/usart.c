#include "usart.h"

//USRAT关于GPIO的配置
void usart_gpio_config(void) {
    //开启时钟（串口时钟和GPIO时钟）
    rcu_periph_clock_enable(BSP_USART_TX_RCU);
    rcu_periph_clock_enable(BSP_USART_RX_RCU);
    rcu_periph_clock_enable(BSP_USART_RCU);
    //配置GPIO复用模式
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    //配置GPIO模式
    gpio_mode_set(GPIOA, GPIO_AF_7, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_mode_set(GPIOA, GPIO_AF_7, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    //配置GPIO的输出
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    //配置串口
}

//USRAT关于参数的配置
void usart_parameter_config(uint32_t baudval) {
    //复位设备
    usart_deinit(USART0);//usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    //设置波特率
    usart_baudrate_set(USART0, baudval);//usart_periph,baudval
    //配置校验功能
    usart_parity_config(USART0, USART_PM_NONE);//usart_periph,(USART_PM_NONE无校验/USART_PM_EVEN偶校验/USART_PM_ODD奇校验)
    //设置数据长度
    usart_word_length_set(USART0, USART_WL_8BIT);//usart_periph,(USART_WL_8BIT: 8 bits/USART_WL_9BIT: 9 bits)
    //设置停止位的位数
    usart_stop_bit_set(USART0, USART_STB_1BIT);//usart_periph,(USART_STB_1BIT/USART_STB_0_5BIT: 0.5 bit(not available for UARTx(x=3,4,6,7))/USART_STB_2BIT:   2 bits/USART_STB_1_5BIT: 1.5 bits(not available for UARTx(x=3,4,6,7)))
    //设置先发送低位还是高位
    usart_data_first_config(USART0, USART_MSBF_LSB);//usart_periph,(USART_MSBF_MSB 先发高位/USART_MSBF_LSB  先发低位)
    //串口使能
    usart_enable(USART0);//usart_periph
    //发送数据配置
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//usart_periph,(USART_TRANSMIT_ENABLE: 使能 USART 发送/USART_TRANSMIT_DISABLE: 去使能 USART 发送)
}

//要发送的数据
void send_data(uint8_t data) {
    usart_data_transmit(USART0, data);//usart_periph,data
    //判断缓冲区是否已经空了
    FlagStatus state = usart_flag_get(USART0, USART_FLAG_TBE);
    while(RESET == state);
}

//usart运行
void usart_run(void) {
    usart_gpio_config();
    usart_parameter_config(115200);
    send_data('a');
}