#include "battery.h"

static void gpio_init(void) {
    //使能时钟
    rcu_periph_clock_enable(RCU_GPIOC);
    //配置gpio模式
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_5);
}

void adc_config(void) {
    //使能ADC0时钟
    rcu_periph_clock_enable(RCU_ADC0);
    //配置ADC时钟
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
    adc_deinit();
    //设置ADC模式（独立模式）
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    //ADC连续功能禁用
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    //ADC关闭扫描模式
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    //ADC数据对齐配置
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    //ADC通道配置
    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,1U);
    adc_inserted_channel_config(ADC0, ADC_INSERTED_CHANNEL_0, ADC_CHANNEL_15, ADC_SAMPLETIME_144);
    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL,ENABLE);
    //使能ADC接口
    adc_enable(ADC0);
    delay_1ms(1);
    //ADC校准
    adc_calibration_enable(ADC0);
    delay_1ms(1);
    //gpio初始化
    gpio_init();
}

float adc_get_voltage() {
    adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
    uint16_t value = adc_inserted_data_read(ADC0,ADC_INSERTED_CHANNEL_0);
    float vref_value = (value * 3.3 / 4096*5);
    return vref_value;
}

void adc_run_test(void) {
    printf("start\n");
    adc_config();
    while(1) {
        float v = adc_get_voltage();
        printf("batteryValue=%f\n",v);
        delay_1ms(500);
    }
}