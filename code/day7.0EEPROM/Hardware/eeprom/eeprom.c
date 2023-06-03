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

//��ʼ��EEPROM��I2C
void i2c_eeprom_init()
{
		//I2C����
		i2c_config(); 
		//I2C��ַ
    eeprom_address = EEPROM_BLOCK0_ADDRESS;
}

/*!
 *  \brief      д��һ���ֽڵ�EEPROM�в�ʹ�ó�ʱ����
 *  \param[in]  p_buffer: ָ�����Ҫд��EEPROM�����ݵĻ�����
 *  \param[in]  write_address: Ҫд���EEPROM�ڲ���ַ
 *  \param[out] none
 *  \retval     none
 */
uint8_t eeprom_byte_write_timeout(uint8_t *p_buffer, uint8_t write_address)
{
    uint8_t state = I2C_START;  // I2Cͨ��״̬
    uint16_t timeout = 0;  // ��ʱʱ��
    uint8_t i2c_timeout_flag = 0;  // I2C��ʱ��־

    while(!(i2c_timeout_flag)) {  // ��ʱ��־Ϊ0����ʾI2Cͨ������������ִ��
        switch(state) {
        case I2C_START:  // ������ʼ�ź�
            // �ȴ�I2C���߿���
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // I2C���߿��У����Է�����ʼ�ź�
                i2c_start_on_bus(I2CX);  // ��I2C�����Ϸ�����ʼ�ź�
                timeout = 0;
                state = I2C_SEND_ADDRESS;  // ������һ��״̬�����͵�ַ֡
            } else {  // I2C����æ��������ʼ�ź�ʧ��
                timeout = 0;
                state   = I2C_START;
                printf("i2c bus is busy in WRITE BYTE!\n");
            }
            break;
        case I2C_SEND_ADDRESS:  // ���͵�ַ֡
            // �ȴ�I2C���豸�ɹ�������ʼ�ź�
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // I2C���豸������ʼ�źųɹ������Է��͵�ַ֡
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);  // ����EEPROM��ַ֡
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;  // ������һ��״̬�������ַ��־λ
            } else {  // I2C���豸������ʼ�ź�ʧ��
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends start signal timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:  // �����ַ��־λ
            // I2C���豸�Ѿ�����ACK�źţ���ַ��־λ����λ
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // ��ַ��־λ�ѱ���λ�������ַ��־λ
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMIT_DATA;  // ������һ��״̬����������
            } else {  // ��ַ��־λδ����λ����ʾ���豸û��Ӧ�𣬷���ʧ��
                timeout = 0;
                state = I2C_START;
                printf("i2c master clears address flag timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:  // ��������
            // �ȴ��������ݻ�����Ϊ��
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // �������ݻ�����Ϊ�գ����Է���Ҫд���EEPROM��ַ
                // ����Ҫд���EEPROM��ַ
                i2c_data_transmit(I2CX, write_address);
                timeout = 0;
            } else {  // �������ݳ�ʱ������ʧ��
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }

            // �ȴ�BTC��־λ������
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // BTC��־λ�����ã����Է���Ҫд�������
                // ����Ҫд�������
                i2c_data_transmit(I2CX, *p_buffer);
                timeout = 0;
            } else {  // �������ݳ�ʱ������ʧ��
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }

            // �ȴ�BTC��־λ������
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // BTC��־λ�����ã����ݷ������
                state = I2C_STOP;  // ������һ��״̬������ֹͣ�ź�
                timeout = 0;
            } else {  // �������ݳ�ʱ������ʧ��
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_STOP:  // ����ֹͣ�ź�
            i2c_stop_on_bus(I2CX);  // ����ֹͣ�źŵ�I2C����
            // �ȴ�I2C�����ϵ�STOP�źű��ɹ�����
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {  // STOP�źű��ɹ����ͣ�I2Cͨ�Ž���
                timeout = 0;
                state = I2C_END;  // ������һ��״̬��ͨ�Ž���
                i2c_timeout_flag = I2C_OK;  // ��־I2Cͨ�ųɹ�
            } else {  // ����ֹͣ�ź�ʧ��
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends stop signal timeout in WRITE BYTE!\n");
            }
            break;
        default:  // I2Cͨ��״̬�쳣�����·�����ʼ�ź�
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c master sends start signal in WRITE BYTE.\n");
            break;
        }
    }
    return I2C_END;  // ����ͨ�Ž���״̬
}


/*!
 * \brief      �ڵ���д��������EEPROM��д�����ֽڵ�����
 * \param[in]  p_buffer: �洢Ҫд��EEPROM�����ݵĻ�����ָ��
 * \param[in]  write_address: Ҫд���EEPROM���ڲ���ַ
 * \param[in]  number_of_byte: Ҫд����ֽ���
 * \param[out] ��
 * \retval     ��
 */
uint8_t eeprom_page_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint8_t number_of_byte)
{
    uint8_t state = I2C_START;          // ����i2c״̬����
    uint16_t timeout = 0;               // ���峬ʱʱ��
    uint8_t i2c_timeout_flag = 0;       // ����i2c��ʱ��־

    while(!(i2c_timeout_flag)) {        // ѭ����ֱ��i2c������ɻ�ʱ
        switch(state) {                 // ����i2c״̬��������i2c����
        case I2C_START:
            /* i2c����ֻ�������߿���ʱ�ŷ�����ʼ�ź� */
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
                printf("i2c����æµ���޷�����д����!\n");
            }
            break;
        case I2C_SEND_ADDRESS:
            /* i2c����������ʼ�źųɹ� */
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
                printf("i2c����������ʼ�źų�ʱ!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            /* ��ַ��־λ�����ã�˵��i2c�ӻ��Ѿ�������ACK */
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
                printf("i2c���������ַ��־λ��ʱ!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:
            /* �ȴ��������ݻ�����Ϊ�� */
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                /* ����Ҫд���EEPROM�ڲ���ַ��ֻ��һ���ֽ� */
                i2c_data_transmit(I2CX, write_address);
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c��������EEPROM�ڲ���ַ��ʱ!\n");
            }
            /* �ȴ�BTC���ر����� */
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c�����������ݳ�ʱ!\n");
            }
            while(number_of_byte--) {
                i2c_data_transmit(I2CX, *p_buffer);   // ��������
                /* ָ����һ��Ҫд����ֽ� */
                p_buffer++;
                /* �ȴ�BTC���ر����� */
                while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    printf("i2c�����������ݳ�ʱ!\n");
                }
            }
            timeout = 0;
            state = I2C_STOP;
            break;
        case I2C_STOP:
            /* ����ֹͣ������i2c������ */
            i2c_stop_on_bus(I2CX);
            /* i2c��������ֹͣ�źųɹ� */
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
                printf("i2c��������ֹͣ�źų�ʱ!\n");
            }
            break;
        default:
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c����������ʼ�ź�ʱ����!\n");
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
	
    // ��ȡ��ַ��Ӧ��ҳ��ַ��ƫ�Ƶ�ַ
    address = write_address % I2C_PAGE_SIZE;//0x0f
    count = I2C_PAGE_SIZE - address;        //0x01
    // ������Ҫд���ҳ���͵���д����ֽ���
    number_of_page = number_of_byte / I2C_PAGE_SIZE; //0x0
    number_of_single = number_of_byte % I2C_PAGE_SIZE;//0x0f

    // ���д���ַ�Ѿ����뵽һҳ����ֱ�Ӱ�ҳд��
    if(0 == address) {//0x10
        while(number_of_page--) {
            // ��ҳд��
            eeprom_page_write_timeout(p_buffer, write_address, I2C_PAGE_SIZE);
            // �ȴ� EEPROM ���ھ���״̬
            eeprom_wait_standby_state_timeout();
            // ���µ�ַ�ͻ���ָ��
            write_address += I2C_PAGE_SIZE;
            p_buffer += I2C_PAGE_SIZE;
        }
        // д�뵥����ʣ���ֽ�
        if(0 != number_of_single) {
            eeprom_page_write_timeout(p_buffer, write_address, number_of_single);
            eeprom_wait_standby_state_timeout();
        }
    } else {
        // ���д���ַû�ж��뵽һҳ������д�����Ĳ��֣��ٰ�ҳд�룬���д��ʣ�ಿ��
        if(number_of_byte < count) {//0x08  3
            // д����벿��
            eeprom_page_write_timeout(p_buffer, write_address, number_of_byte);
            eeprom_wait_standby_state_timeout();
        } else {//0x0f 3
            // д����벿��
            number_of_byte -= count;
            eeprom_page_write_timeout(p_buffer, write_address, count);
            eeprom_wait_standby_state_timeout();
            // ���µ�ַ�ͻ���ָ��
            write_address += count;
            p_buffer += count;

            // ��ҳд��
            number_of_page = number_of_byte / I2C_PAGE_SIZE;
            while(number_of_page--) {
                eeprom_page_write_timeout(p_buffer, write_address, I2C_PAGE_SIZE);
                eeprom_wait_standby_state_timeout();
                write_address += I2C_PAGE_SIZE;
                p_buffer += I2C_PAGE_SIZE;
            }

            // д��ʣ�ಿ��
            number_of_single = number_of_byte % I2C_PAGE_SIZE;
            if(0 != number_of_single) {
                eeprom_page_write_timeout(p_buffer, write_address, number_of_single);
                eeprom_wait_standby_state_timeout();
            }
        }
    }
}

/*!
    \brief      ��EEPROM��ȡ����(���豸�ȷ����ڲ���ַ,�ٶ�ȡ)
    \param[in]  p_buffer: ָ�����EEPROM��ȡ���ݵĻ�����
    \param[in]  read_address: ��EEPROM��ʼ��ȡ���ڲ���ַ
    \param[in]  number_of_byte: ��EEPROM��ȡ���ֽ���
    \param[out] none
    \retval     none
*/
uint8_t eeprom_buffer_read_timeout(uint8_t *p_buffer, uint8_t read_address, uint16_t number_of_byte)
{
    uint8_t state = I2C_START;
    uint8_t read_cycle = 0;
    uint16_t timeout = 0;
		//��ʱ��־
    uint8_t i2c_timeout_flag = 0;

    while(!(i2c_timeout_flag)) {
        switch(state) {
				//��ʼ�ź�
        case I2C_START:
            if(RESET == read_cycle) {
                //// �����߿���ʱ��I2C���豸������ʼ�ź�
                while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    // ������һ���ֽ��Ƿ���ACK
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
            //��ʼ�ź�
            i2c_start_on_bus(I2CX);
            timeout = 0;
            state = I2C_SEND_ADDRESS;
            break;
        case I2C_SEND_ADDRESS://���͵�ַ
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
								
                if(RESET == read_cycle) {
										//// �Է�����ģʽ�����豸��ַ
                    i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                    state = I2C_CLEAR_ADDRESS_FLAG;
                }else {
										// �Խ�����ģʽ�����豸��ַ
                    i2c_master_addressing(I2CX, eeprom_address, I2C_RECEIVER);
                    if(number_of_byte < 3) {
                        //����Ӧ��
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
        case I2C_CLEAR_ADDRESS_FLAG://�����ַ��־λ
            //address flag������,����I2C���豸������Ӧ���ź�
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
        case I2C_TRANSMIT_DATA://��������
            if(RESET == read_cycle) {
                /* wait until the transmit data buffer is empty */
                while((! i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    // ����EEPROM���ڲ���ַ��ֻ��һ���ֽڵĵ�ַ��
                    i2c_data_transmit(I2CX, read_address);
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = 0;
                    printf("i2c master wait data buffer is empty timeout in READ!\n");
                }
                //�ȴ�BTC �ֽڴ������
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
