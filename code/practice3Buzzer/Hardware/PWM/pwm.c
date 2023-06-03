#include "pwm.h"

//GPIO初始化
void pwm_gpio_init(void) {
    /*1.使能时钟*/
    rcu_periph_clock_enable(BSP_PWM_RCU);
    /*2.配置GPIO的模式*/
    gpio_mode_set(BSP_PWM_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,BSP_PWM_PIN);
    /*3.配置GPIO的输出*/
    gpio_output_options_set(BSP_PWM_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BSP_PWM_PIN);

    /*4.配置GPIO的复用*/
    gpio_af_set(BSP_PWM_PORT,BSP_PWM_AF,BSP_PWM_PIN);
}

//初始化定时器
void pwm_config(uint16_t pre,uint16_t per) {

//*************************[定时器配置]*************************
    /*1.定义定时器结构体*/
    timer_parameter_struct timere_initpara;
    /*3.PA5进行GPIO配置*/
    pwm_gpio_init();
    /*4.开启定时器时钟*/
    rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);
    /*5.配置定时器时钟(4倍)*/
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    /*6.复位定时器*/
    timer_deinit(BSP_PWM_TIMER);
    /*7.配置定时器参数*/
    timere_initpara.prescaler = pre-1; //预分频值 0-65535
    timere_initpara.alignedmode = TIMER_COUNTER_EDGE; //对齐模式(暂时没用)
    timere_initpara.counterdirection = TIMER_COUNTER_UP;  // 向上计数(普通定时器只有这个方式)
    timere_initpara.period = per-1;  // 周期(结合分频值计算时间)
    /* 在输入捕获的时候使用  数字滤波器使用的采样频率之间的分频比例 */
    timere_initpara.clockdivision = TIMER_CKDIV_DIV1;     // 分频因子
    /* 只有高级定时器才有 配置为x，就重复x+1次进入中断 */
    timere_initpara.repetitioncounter = 0;	// 重复计数器 0-255
    /*8.初始化定时器*/
    timer_init(BSP_PWM_TIMER,&timere_initpara);
//===================================================

//*************************[PWM配置]*************************
    /*2.定时器比较输出结构体*/
    timer_oc_parameter_struct timer_ocintpara;
    /*9.配置输出结构体*/
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;//有效电平的极性   																	  // 有效电平的极性
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;	//比较输出模式状态,能使pwm输出到端口
    /*10.配置定时器输出功能*/
    timer_channel_output_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,&timer_ocintpara);
    /*11.配置占空比*/
    //配置定时器通道输出脉冲值
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,0);
    //配置定时器通道输出比较模式
    timer_channel_output_mode_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,TIMER_OC_MODE_PWM0);
    //配置定时器通道输出影子寄存器
    timer_channel_output_shadow_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,TIMER_OC_SHADOW_DISABLE);
    //只有高级定时器才需要此配置
    //timer_primary_output_config(TIMER0,ENABLE);
    /*11.TIMEx自动重载影子使能*/
    timer_auto_reload_shadow_enable(BSP_PWM_TIMER);
    /*12.使能定时器*/
    timer_enable(BSP_PWM_TIMER);
//===================================================
}

//*************************[蜂鸣器具体方法]*************************
void buzzer_func(void) {
    //产生脉冲
    //523, 587, 659, 698, 784, 880, 494 * 2, 523 * 2}
    //523
    TIMER_CAR(BSP_PWM_TIMER) = 19;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,9);
    delay_1ms(500);

    //587
    TIMER_CAR(BSP_PWM_TIMER) = 17;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,8);
    delay_1ms(500);

    //659
    TIMER_CAR(BSP_PWM_TIMER) = 15;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,7);
    delay_1ms(500);

    //698
    TIMER_CAR(BSP_PWM_TIMER) = 14;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,7);
    delay_1ms(500);

    //784
    TIMER_CAR(BSP_PWM_TIMER) = 12;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,6);
    delay_1ms(500);

    //880
    TIMER_CAR(BSP_PWM_TIMER) = 11;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,5);
    delay_1ms(500);

    //494
    TIMER_CAR(BSP_PWM_TIMER) = 10;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,5);
    delay_1ms(500);

    //523
    TIMER_CAR(BSP_PWM_TIMER) = 9;
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER,BSP_PWM_CHANNEL,4);
    delay_1ms(500);
}
//===================================================

//程序运行
void pwm_run(void) {
    pwm_gpio_init();
    pwm_config(24000,10000);
    buzzer_func();
}