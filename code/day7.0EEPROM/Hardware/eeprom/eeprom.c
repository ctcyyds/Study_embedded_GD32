#include "eeprom.h"


#define EEPROM_BLOCK0_ADDRESS    0xA0
#define BUFFER_SIZE              256

uint16_t eeprom_address;

/*!
    \brief      I2C read and write functions
    \param[in]  none
    \param[out] none
    \retval     I2C_OK or I2C_FAIL
*/
uint8_t i2c_24c02_test(void)
{
    uint16_t i;
    uint8_t i2c_buffer_write[BUFFER_SIZE];
    uint8_t i2c_buffer_read[BUFFER_SIZE];

    printf("\r\nAT24C02 writing...\r\n");

    /* initialize i2c_buffer_write */
    for(i = 0; i < BUFFER_SIZE; i++) {
        i2c_buffer_write[i] = i;
        printf("0x%02X ", i2c_buffer_write[i]);
        if(15 == i % 16) {
            printf("\r\n");
        }
    }
    /* EEPROM data write */
    eeprom_buffer_write_timeout(i2c_buffer_write, EEP_FIRST_PAGE, BUFFER_SIZE);
    printf("AT24C02 reading...\r\n");
    /* EEPROM data read */
    eeprom_buffer_read_timeout(i2c_buffer_read, EEP_FIRST_PAGE, BUFFER_SIZE);
    /* compare the read buffer and write buffer */
    for(i = 0; i < BUFFER_SIZE; i++) {
        if(i2c_buffer_read[i] != i2c_buffer_write[i]) {
            printf("0x%02X ", i2c_buffer_read[i]);
            printf("Err:data read and write aren't matching.\n\r");
            return I2C_FAIL;
        }
        printf("0x%02X ", i2c_buffer_read[i]);
        if(15 == i % 16) {
            printf("\r\n");
        }
    }
    printf("I2C-AT24C02 test passed!\n\r");
    return I2C_OK;
}

//初始化EEPROM的I2C
void i2c_eeprom_init()
{
		//I2C配置
		i2c_config(); 
		//I2C地址
    eeprom_address = EEPROM_BLOCK0_ADDRESS;
}

/*!
 *  \brief      写入一个字节到EEPROM中并使用超时函数
 *  \param[in]  p_buffer: 指向包含要写入EEPROM的数据的缓冲区
 *  \param[in]  write_address: 要写入的EEPROM内部地址
 *  \param[out] none
 *  \retval     none
 */
uint8_t eeprom_byte_write_timeout(uint8_t *p_buffer, uint8_t write_address)
{
    uint8_t state = I2C_START;  // I2C通信状态
    uint16_t timeout = 0;  // 超时时间
    uint8_t i2c_timeout_flag = 0;  // I2C超时标志

    while(!(i2c_timeout_flag)) {  // 超时标志为0，表示I2C通信正常，继续执行
        switch(state) {
        case I2C_START:  // 发送起始信号
            // 等待I2C总线空闲
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // I2C总线空闲，可以发送起始信号
                i2c_start_on_bus(I2CX);  // 在I2C总线上发送起始信号
                timeout = 0;
                state = I2C_SEND_ADDRESS;  // 进入下一个状态：发送地址帧
            } else {  // I2C总线忙，发送起始信号失败
                timeout = 0;
                state   = I2C_START;
                printf("i2c bus is busy in WRITE BYTE!\n");
            }
            break;
        case I2C_SEND_ADDRESS:  // 发送地址帧
            // 等待I2C主设备成功发送起始信号
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // I2C主设备发送起始信号成功，可以发送地址帧
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);  // 发送EEPROM地址帧
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;  // 进入下一个状态：清除地址标志位
            } else {  // I2C主设备发送起始信号失败
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends start signal timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:  // 清除地址标志位
            // I2C从设备已经发送ACK信号，地址标志位被置位
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // 地址标志位已被置位，清除地址标志位
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMIT_DATA;  // 进入下一个状态：发送数据
            } else {  // 地址标志位未被置位，表示从设备没有应答，发送失败
                timeout = 0;
                state = I2C_START;
                printf("i2c master clears address flag timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:  // 发送数据
            // 等待发送数据缓冲区为空
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // 发送数据缓冲区为空，可以发送要写入的EEPROM地址
                // 发送要写入的EEPROM地址
                i2c_data_transmit(I2CX, write_address);
                timeout = 0;
            } else {  // 发送数据超时，发送失败
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }

            // 等待BTC标志位被设置
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // BTC标志位被设置，可以发送要写入的数据
                // 发送要写入的数据
                i2c_data_transmit(I2CX, *p_buffer);
                timeout = 0;
            } else {  // 发送数据超时，发送失败
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }

            // 等待BTC标志位被设置
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // BTC标志位被设置，数据发送完成
                state = I2C_STOP;  // 进入下一个状态：发送停止信号
                timeout = 0;
            } else {  // 发送数据超时，发送失败
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_STOP:  // 发送停止信号
            i2c_stop_on_bus(I2CX);  // 发送停止信号到I2C总线
            // 等待I2C总线上的STOP信号被成功发送
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // STOP信号被成功发送，I2C通信结束
                timeout = 0;
                state = I2C_END;  // 进入下一个状态：通信结束
                i2c_timeout_flag = I2C_OK;  // 标志I2C通信成功
            } else {  // 发送停止信号失败
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends stop signal timeout in WRITE BYTE!\n");
            }
            break;
        default:  // I2C通信状态异常，重新发送起始信号
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c master sends start signal in WRITE BYTE.\n");
            break;
        }
    }
    return I2C_END;  // 返回通信结束状态
}


/*!
 * \brief      在单个写周期中向EEPROM中写入多个字节的数据
 * \param[in]  p_buffer: 存储要写入EEPROM的数据的缓冲区指针
 * \param[in]  write_address: 要写入的EEPROM的内部地址
 * \param[in]  number_of_byte: 要写入的字节数
 * \param[out] 无
 * \retval     无
 */
uint8_t eeprom_page_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint8_t number_of_byte)
{
    uint8_t state = I2C_START;          // 定义i2c状态变量
    uint16_t timeout = 0;               // 定义超时时间
    uint8_t i2c_timeout_flag = 0;       // 定义i2c超时标志

    while(!(i2c_timeout_flag)) {        // 循环，直到i2c操作完成或超时
        switch(state) {                 // 根据i2c状态机，进行i2c操作
        case I2C_START:
            /* i2c主机只有在总线空闲时才发送起始信号 */
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_start_on_bus(I2CX);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            } else {
                i2c_bus_reset();
                timeout = 0;
                state = I2C_START;
                printf("i2c总线忙碌，无法进行写操作!\n");
            }
            break;
        case I2C_SEND_ADDRESS:
            /* i2c主机发送起始信号成功 */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c主机发送起始信号超时!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            /* 地址标志位被设置，说明i2c从机已经发送了ACK */
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMIT_DATA;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c主机清除地址标志位超时!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:
            /* 等待传输数据缓冲区为空 */
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                /* 发送要写入的EEPROM内部地址：只有一个字节 */
                i2c_data_transmit(I2CX, write_address);
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c主机发送EEPROM内部地址超时!\n");
            }
            /* 等待BTC比特被设置 */
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c主机发送数据超时!\n");
            }
            while(number_of_byte--) {
                i2c_data_transmit(I2CX, *p_buffer);   // 发送数据
                /* 指向下一个要写入的字节 */
                p_buffer++;
                /* 等待BTC比特被设置 */
                while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    printf("i2c主机发送数据超时!\n");
                }
            }
            timeout = 0;
            state = I2C_STOP;
            break;
        case I2C_STOP:
            /* 发送停止条件到i2c总线上 */
            i2c_stop_on_bus(I2CX);
            /* i2c主机发送停止信号成功 */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_END;
                i2c_timeout_flag = I2C_OK;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c主机发送停止信号超时!\n");
            }
            break;
        default:
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c主机发送起始信号时出错!\n");
            break;
        }
    }
    return I2C_END;
}

/*!
    \brief      write buffer of data to the EEPROM use timeout function
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void eeprom_buffer_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint16_t number_of_byte)
{
    uint8_t number_of_page = 0, number_of_single = 0, address = 0, count = 0;
	
    // 获取地址对应的页地址和偏移地址
    address = write_address % I2C_PAGE_SIZE;//0x0f
    count = I2C_PAGE_SIZE - address;        //0x01
    // 计算需要写入的页数和单独写入的字节数
    number_of_page = number_of_byte / I2C_PAGE_SIZE; //0x0
    number_of_single = number_of_byte % I2C_PAGE_SIZE;//0x0f

    // 如果写入地址已经对齐到一页，则直接按页写入
    if(0 == address) {//0x10
        while(number_of_page--) {
            // 按页写入
            eeprom_page_write_timeout(p_buffer, write_address, I2C_PAGE_SIZE);
            // 等待 EEPROM 处于就绪状态
            eeprom_wait_standby_state_timeout();
            // 更新地址和缓存指针
            write_address += I2C_PAGE_SIZE;
            p_buffer += I2C_PAGE_SIZE;
        }
        // 写入单独的剩余字节
        if(0 != number_of_single) {
            eeprom_page_write_timeout(p_buffer, write_address, number_of_single);
            eeprom_wait_standby_state_timeout();
        }
    } else {
        // 如果写入地址没有对齐到一页，则先写入对齐的部分，再按页写入，最后写入剩余部分
        if(number_of_byte < count) {//0x08  3
            // 写入对齐部分
            eeprom_page_write_timeout(p_buffer, write_address, number_of_byte);
            eeprom_wait_standby_state_timeout();
        } else {//0x0f 3
            // 写入对齐部分
            number_of_byte -= count;
            eeprom_page_write_timeout(p_buffer, write_address, count);
            eeprom_wait_standby_state_timeout();
            // 更新地址和缓存指针
            write_address += count;
            p_buffer += count;

            // 按页写入
            number_of_page = number_of_byte / I2C_PAGE_SIZE;
            while(number_of_page--) {
                eeprom_page_write_timeout(p_buffer, write_address, I2C_PAGE_SIZE);
                eeprom_wait_standby_state_timeout();
                write_address += I2C_PAGE_SIZE;
                p_buffer += I2C_PAGE_SIZE;
            }

            // 写入剩余部分
            number_of_single = number_of_byte % I2C_PAGE_SIZE;
            if(0 != number_of_single) {
                eeprom_page_write_timeout(p_buffer, write_address, number_of_single);
                eeprom_wait_standby_state_timeout();
            }
        }
    }
}

/*!
    \brief      从EEPROM读取数据(主设备先发送内部地址,再读取)
    \param[in]  p_buffer: 指向接收EEPROM读取数据的缓冲区
    \param[in]  read_address: 从EEPROM开始读取的内部地址
    \param[in]  number_of_byte: 从EEPROM读取的字节数
    \param[out] none
    \retval     none
*/
uint8_t eeprom_buffer_read_timeout(uint8_t *p_buffer, uint8_t read_address, uint16_t number_of_byte)
{
    uint8_t state = I2C_START;
    uint8_t read_cycle = 0;
    uint16_t timeout = 0;
		//超时标志
    uint8_t i2c_timeout_flag = 0;

    while(!(i2c_timeout_flag)) {
        switch(state) {
				//起始信号
        case I2C_START:
            if(RESET == read_cycle) {
                //// 当总线空闲时，I2C主设备发送起始信号
                while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    // 对于下一个字节是否发送ACK
                    if(2 == number_of_byte) {
                        i2c_ackpos_config(I2CX, I2C_ACKPOS_NEXT);
                    }
                } else {
                    i2c_bus_reset();
                    timeout = 0;
                    state = I2C_START;
                    printf("i2c bus is busy in READ!\n");
                }
            }
            //起始信号
            i2c_start_on_bus(I2CX);
            timeout = 0;
            state = I2C_SEND_ADDRESS;
            break;
        case I2C_SEND_ADDRESS://发送地址
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
								
                if(RESET == read_cycle) {
										//// 以发送器模式发送设备地址
                    i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                    state = I2C_CLEAR_ADDRESS_FLAG;
                }else {
										// 以接收器模式发送设备地址
                    i2c_master_addressing(I2CX, eeprom_address, I2C_RECEIVER);
                    if(number_of_byte < 3) {
                        //禁用应答
                        i2c_ack_config(I2CX, I2C_ACK_DISABLE);
                    }
                    state = I2C_CLEAR_ADDRESS_FLAG;
                }
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                read_cycle = 0;
                printf("i2c master sends start signal timeout in READ!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG://清除地址标志位
            //address flag设置了,代表I2C从设备发送了应答信号
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                if((SET == read_cycle) && (1 == number_of_byte)) {
                    /* send a stop condition to I2C bus */
                    i2c_stop_on_bus(I2CX);
                }
                timeout = 0;
                state = I2C_TRANSMIT_DATA;
            } else {
                timeout = 0;
                state = I2C_START;
                read_cycle = 0;
                printf("i2c master clears address flag timeout in READ!\n");
            }
            break;
        case I2C_TRANSMIT_DATA://发送数据
            if(RESET == read_cycle) {
                /* wait until the transmit data buffer is empty */
                while((! i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    // 发送EEPROM的内部地址（只有一个字节的地址）
                    i2c_data_transmit(I2CX, read_address);
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = 0;
                    printf("i2c master wait data buffer is empty timeout in READ!\n");
                }
                //等待BTC 字节传输完成
                while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle++;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = 0;
                    printf("i2c master sends EEPROM's internal address timeout in READ!\n");
                }
            } else {
                while(number_of_byte) {
                    timeout++;
                    if(3 == number_of_byte) {
                        /* wait until BTC bit is set */
                        while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));
                        /* disable acknowledge */
                        i2c_ack_config(I2CX, I2C_ACK_DISABLE);
                    }
                    if(2 == number_of_byte) {
                        /* wait until BTC bit is set */
                        while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));
                        /* send a stop condition to I2C bus */
                        i2c_stop_on_bus(I2CX);
                    }
                    /* wait until RBNE bit is set */
                    if(i2c_flag_get(I2CX, I2C_FLAG_RBNE)) {
                        /* read a byte from the EEPROM */
                        *p_buffer = i2c_data_receive(I2CX);
                        /* point to the next location where the byte read will be saved */
                        p_buffer++;
                        /* decrement the read bytes counter */
                        number_of_byte--;
                        timeout = 0;
                    }
                    if(timeout > I2C_TIME_OUT) {
                        timeout = 0;
                        state = I2C_START;
                        read_cycle = 0;
                        printf("i2c master sends data timeout in READ!\n");
                    }
                }
                timeout = 0;
                state = I2C_STOP;
            }
            break;
        case I2C_STOP:
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_END;
                i2c_timeout_flag = I2C_OK;
            } else {
                timeout = 0;
                state = I2C_START;
                read_cycle = 0;
                printf("i2c master sends stop signal timeout in READ!\n");
            }
            break;
        default:
            state = I2C_START;
            read_cycle = 0;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c master sends start signal in READ.\n");
            break;
        }
    }
    return I2C_END;
}

/*!
    \brief      wait for EEPROM standby state use timeout function
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t eeprom_wait_standby_state_timeout()
{
    uint8_t state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    while(!(i2c_timeout_flag)) {
        switch(state) {
        case I2C_START:
            /* i2c master sends start signal only when the bus is idle */
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_start_on_bus(I2CX);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            } else {
                i2c_bus_reset();
                timeout = 0;
                state = I2C_START;
                printf("i2c bus is busy in EEPROM standby!\n");
            }
            break;
        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((! i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends start signal timeout in EEPROM standby!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            while((!((i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) || (i2c_flag_get(I2CX, I2C_FLAG_AERR)))) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                if(i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) {
                    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                    timeout = 0;
                    /* send a stop condition to I2C bus */
                    i2c_stop_on_bus(I2CX);
                    i2c_timeout_flag = I2C_OK;
                    /* exit the function */
                    return I2C_END;
                } else {
                    /* clear the bit of AERR */
                    i2c_flag_clear(I2CX, I2C_FLAG_AERR);
                    timeout = 0;
                    state = I2C_STOP;
                }
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master clears address flag timeout in EEPROM standby!\n");
            }
            break;
        case I2C_STOP:
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2CX);
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_START;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends stop signal timeout in EEPROM standby!\n");
            }
            break;
        default:
            state = I2C_START;
            timeout = 0;
            printf("i2c master sends start signal end in EEPROM standby!\n");
            break;
        }
    }
    return I2C_END;
}
