#include "ws2812.h"
#include "gd32f4xx.h"

#define WS2812_CODE0 72
#define WS2812_CODE1 180

#define WS2812_RESETCODE 270
#define WS2812_NUM 2
#define WS2812_TRANSFER_NUM WS2812_NUM * 24 + WS2812_RESETCODE
uint32_t ws2812_buffer[WS2812_TRANSFER_NUM] = {0};

// gpio设置 PA8 输出PWM
void ws2812_gpio_init()
{
	// 开启时钟线
	rcu_periph_clock_enable(RCU_GPIOA);
	// 配置输出，配置AF1
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_8);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

// 定时器的设置 // TIMER0_ CH0
/**
	0：高电平 220 - 380
	 低电�? 580 - 1000

	1: 高电�? 580 - 1000
	 低电�? 580 - 1000

	0�? 高电�? 300ns  ---�? 72
  1�? 高电�? 750  ---�? 252 - 72 = 180

  假定周期�? 1050ns

	240mhz
  240 000 000

	1s 1000ms 1000 000 us 1000 000 000ns

	1000 000 000ns/240 000 000
	100ns/24 = 4.16ns

	计数值：252

	1050ns  ---> 280us/1.05
*/
void ws2812_timer_init()
{
	// 开启定时器的时钟线 // 240MHZ
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	rcu_periph_clock_enable(RCU_TIMER0);
	// 定时器配�?
	timer_parameter_struct timer_param;

	timer_struct_para_init(&timer_param);
	timer_param.prescaler = 0;
	timer_param.period = 252 - 1;

	timer_deinit(TIMER0);
	timer_init(TIMER0, &timer_param);

	// 定时器的输出配置
	timer_oc_parameter_struct timer_oc_param;
	// 设置基础参数
	timer_channel_output_struct_para_init(&timer_oc_param);
	// 使能CC寄存�?
	timer_oc_param.outputstate = TIMER_CCX_ENABLE;
	timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_oc_param);

	// 设置pwm模式
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 0);
	timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);

	// 启动定时器DMA的功�?
	timer_dma_enable(TIMER0, TIMER_DMA_CH0D);

	// 配置定时器输�?
	timer_primary_output_config(TIMER0, ENABLE);
	timer_enable(TIMER0);
}

/**
TIMER0_CH0  DMA功能
DMA1 通道1
*/
void ws2812_dma_init()
{
	dma_single_data_parameter_struct dma_init_struct;
	/* enable DMA1 */
	rcu_periph_clock_enable(RCU_DMA1);

	dma_single_data_para_struct_init(&dma_init_struct);
	dma_deinit(DMA1, DMA_CH1);
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
	dma_init_struct.memory0_addr = (uint32_t)ws2812_buffer;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
	dma_init_struct.number = WS2812_TRANSFER_NUM;
	dma_init_struct.periph_addr = TIMER0 + 0x34U; // TIMER _CH0 地址
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_single_data_mode_init(DMA1, DMA_CH1, &dma_init_struct);
	/* configure DMA mode */
	dma_circulation_enable(DMA1, DMA_CH1);
	dma_channel_subperipheral_select(DMA1, DMA_CH1, DMA_SUBPERI6);

	dma_channel_enable(DMA1, DMA_CH1);
}

// 初始�?
void ws2812_init()
{
	// gpio设置
	ws2812_gpio_init();
	// 定时器的设置
	ws2812_timer_init();
	// DMA配置
	ws2812_dma_init();
}

// 设置灯的颜色
void ws2812_set_color(uint8_t index, uint32_t color)
{
	// color : BGR
	// 练习�? 请按照RGB传入的顺序显示正确的灯光
	// RGB--- > BGB
	// R G B 调换顺序
	// ws2812_buffer[2 * 24 + 270]
	// bgr  0xFF0000 111111110000000000000000
	//	ws2812_buffer[0] = {1,1,1,1, 1,1,1,1 0000 0000 0000 0000}
	for (uint8_t i = 0; i < 24; i++)
	{
		// ws2812_buffer[i] =
		/**
		i=0:
			111111110000000000000000
			100000000000000000000000
		i=1
			111111110000000000000000
			000	...		0100000000000000000000000
		*/
		if ((color << i) & 0B100000000000000000000000)
		{
			ws2812_buffer[i + index * 24] = WS2812_CODE1;
		}
		else
		{
			ws2812_buffer[i + index * 24] = WS2812_CODE0;
		}
	}
}

void ws2812_test()
{
	// 初始�?
	ws2812_init();
	// RGB EA 36 80 --> GRB 36EA80
	ws2812_set_color(0, 0x36EA80);
}