#include "Spi4.h"

void Spi4_Init(void)
{
	
	// 定义SPI初始化结构体
	spi_parameter_struct spi_init_struct; 

	// 使能GPIOF和SPI4时钟
	rcu_periph_clock_enable(RCU_GPIOF); 
	rcu_periph_clock_enable(RCU_SPI4);  

	// 将GPIOF的PIN7和PIN9设置为SPI4的功能引脚
	gpio_af_set(GPIOF, GPIO_AF_5, GPIO_PIN_7 | GPIO_PIN_9);

	// 配置GPIOF的PIN7和PIN9为复用功能、无上下拉电阻
	gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7 | GPIO_PIN_9);
		
	// 配置GPIOF的PIN7和PIN9为推挽输出、最大输出速率50MHz
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_9);
		
    // 将GPIOF的PIN7和PIN9置为高电平
    gpio_bit_set(GPIOF, GPIO_PIN_7 | GPIO_PIN_9);

    // 配置SPI的传输模式、设备模式、数据帧格式、时钟极性和相位、NSS信号类型（软件），以及预分频值和端序
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; 
//		spi_init_struct.trans_mode = SPI_TRANSMODE_RECEIVEONLY; 
    spi_init_struct.device_mode = SPI_MASTER;              
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;       
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT; // 软件cs
    spi_init_struct.prescale = SPI_PSC_2;
    spi_init_struct.endian = SPI_ENDIAN_MSB;

    // 初始化SPI4
    spi_init(SPI4, &spi_init_struct);

    // 使能SPI4
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
