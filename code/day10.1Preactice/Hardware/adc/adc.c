#include "adc.h"
#include "stdio.h"

//#define DEBUG   // 打开这个将会使能adc_key下面的打印信息

uint16_t 	adcXValue,adcYValue;


/* ADC的附加功能 优先于普通GPIO  */
static void adc_gpio_init(void)
{
		/* enable the clock */
    rcu_periph_clock_enable(ADCX_RCU);
    //GPIO模式需要设置为模拟模式
    gpio_mode_set(ADCX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCX_PIN);
  
		/* enable the clock */
    rcu_periph_clock_enable(ADCY_RCU);
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG 模拟信号*/ 
    gpio_mode_set(ADCY_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCY_PIN);  
}

void adc_config(rcu_periph_enum periph,uint32_t adc_periph)
{		
		/* adc 引脚初始化 */
		adc_gpio_init();
		/* 使能ADC时钟 */
    rcu_periph_clock_enable(periph);
		/* 根据时钟树配置ADC时钟分频 */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
		/* 复位ADC */
		adc_deinit();
	  /* 配置ADC工作模式 */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // 所有ADC都工作在独立模式
		
    /* 配置ADC转换模式  关闭连续转换模式 */
    adc_special_function_config(adc_periph, ADC_CONTINUOUS_MODE, DISABLE); // 关闭连续模式
    /* 独立采集模式不需要scan */
    adc_special_function_config(adc_periph, ADC_SCAN_MODE, DISABLE); // 关闭扫描模式
		
		/* 配置ADC数据对齐方式 RIGHT:低位对齐  LEFT:高位对齐*/
    adc_data_alignment_config(adc_periph,ADC_DATAALIGN_RIGHT); // LSB对齐，低位对齐
	
    /* 配置ADC通道长度 默认的 REGULER*/
    adc_channel_length_config(adc_periph,ADC_ROUTINE_CHANNEL,1U); //  ADC规则通道 长度为1

    /* 使能ADC模块 */
    adc_enable(adc_periph);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC校准 比如:可以给恒定电压 测量看是否和给定的电压相同*/
    adc_calibration_enable(adc_periph);  // ADC校准
    /* wait for ADC stability */
    delay_1ms(1);
}


/*!
    \brief      ADC channel sample  ADC通道采样
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t adc_channel_sample(uint8_t channel)
{
    /* ADC routine channel config */
    adc_routine_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15个采样周期 
    /* ADC software trigger enable */
		//软件触发adc转换
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); // ADC软件触发使能

    //等待转换完成
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    //清除转换完成标志
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    //获取转换结果 0-4096  (12分辨率)
    return (adc_routine_data_read(ADC0));
}


/* 摇杆扫描函数 */
void adc_key_scan(void)
{	
	while(1){
		adcXValue = adc_channel_sample(ADC_CHANNEL_1);  // 采样
		adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // 采样
		printf("x:%d,y:%d\n",adcXValue,adcYValue);
		delay_1ms(500);
	}
	
}