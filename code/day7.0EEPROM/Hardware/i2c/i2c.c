#include "i2c.h"


//GPIO��������
static void gpio_config(void)
{
    //����ʱ��ʹ��
    rcu_periph_clock_enable(RCU_GPIO_I2C);

    //���ø��ù��ܱ��
    gpio_af_set(I2C_SCL_PORT, I2C_GPIO_AF, I2C_SCL_PIN);
    gpio_af_set(I2C_SDA_PORT, I2C_GPIO_AF, I2C_SDA_PIN);
    //�������Ÿ��ù���
    gpio_mode_set(I2C_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SCL_PIN);
		//�������ģʽ
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_mode_set(I2C_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SDA_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
}

/*!
    \brief      configure the I2CX interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void)
{
		//I2C��GPIO��������
		gpio_config();
	
    //ʹ��I2Cʱ��
    rcu_periph_clock_enable(RCU_I2C);
    //I2Cʱ������
		//����2:ʱ���ٶ�,������100 kHz��400 kHz
		//����3:����ģʽ�µ�ռ�ձ�
    i2c_clock_config(I2CX, I2C_SPEED, I2C_DTCY_2);
		//i2c_mode_addr_config(I2CX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2CX_SLAVE_ADDRESS7);
    //ʹ��I2C
    i2c_enable(I2CX);
    //I2C���豸Ӧ������
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
}

//I2CӲ����λ  
//�Ƚ� SCL �� SDA �������ͣ�Ȼ����ʱһ��ʱ�䣬���Ž� SCL �������ߣ�����ٽ� SDA ��������
void i2c_bus_reset(void)
{
		//����I2C
    i2c_deinit(I2CX);
    /* configure SDA/SCL for GPIO */
    GPIO_BC(I2C_SCL_PORT) |= I2C_SCL_PIN;
    GPIO_BC(I2C_SDA_PORT) |= I2C_SDA_PIN;
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(I2C_SCL_PORT) |= I2C_SCL_PIN;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(I2C_SDA_PORT) |= I2C_SDA_PIN;
    /* connect I2C_SCL_PIN to I2C_SCL */
    /* connect I2C_SDA_PIN to I2C_SDA */
    gpio_output_options_set(I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_output_options_set(I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    /* configure the I2CX interface */
    i2c_config();
}
