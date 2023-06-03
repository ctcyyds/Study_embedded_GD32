#include "Spi4.h"

void Spi4_Init(void)
{
	
	// ����SPI��ʼ���ṹ��
	spi_parameter_struct spi_init_struct; 

	// ʹ��GPIOF��SPI4ʱ��
	rcu_periph_clock_enable(RCU_GPIOF); 
	rcu_periph_clock_enable(RCU_SPI4);  

	// ��GPIOF��PIN7��PIN9����ΪSPI4�Ĺ�������
	gpio_af_set(GPIOF, GPIO_AF_5, GPIO_PIN_7 | GPIO_PIN_9);

	// ����GPIOF��PIN7��PIN9Ϊ���ù��ܡ�������������
	gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7 | GPIO_PIN_9);
		
	// ����GPIOF��PIN7��PIN9Ϊ�������������������50MHz
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_9);
		
    // ��GPIOF��PIN7��PIN9��Ϊ�ߵ�ƽ
    gpio_bit_set(GPIOF, GPIO_PIN_7 | GPIO_PIN_9);

    // ����SPI�Ĵ���ģʽ���豸ģʽ������֡��ʽ��ʱ�Ӽ��Ժ���λ��NSS�ź����ͣ���������Լ�Ԥ��Ƶֵ�Ͷ���
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; 
//		spi_init_struct.trans_mode = SPI_TRANSMODE_RECEIVEONLY; 
    spi_init_struct.device_mode = SPI_MASTER;              
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;       
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT; // ���cs
    spi_init_struct.prescale = SPI_PSC_2;
    spi_init_struct.endian = SPI_ENDIAN_MSB;

    // ��ʼ��SPI4
    spi_init(SPI4, &spi_init_struct);

    // ʹ��SPI4
    spi_enable(SPI4);
}

//uint8_t Spi4_Read(void)
//{
//    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_RBNE));
//    return spi_i2s_data_receive(SPI4);
//    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_RBNE));
//}

void Spi4_Write(uint8_t dat)
{		
		/* loop while spi tx data register in not emplty */
    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_TBE)) ;
		/* send byte through the SPI2 peripheral */
    spi_i2s_data_transmit(SPI4, dat); 
    while (RESET == spi_i2s_flag_get(SPI4, SPI_FLAG_TBE)) ;
}
