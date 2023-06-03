#include "interrupt.h"
#include "led.h"
/*按键中断点灯  外部中断*/

//初始化
void key_interrupt_init(void) {
    /*1.开启时钟*/
    //1.1 开启GPIO端口的时钟
    rcu_periph_clock_enable(BSP_KEY_RCU);
    //1.2 开启系统配置时钟
    rcu_periph_clock_enable(RCU_SYSCFG);
    /*2.配置GPIO的模式:输入模式和下拉模式*/
    gpio_mode_set(BSP_KEY_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BSP_KEY_PIN);
    /*3.使能NVIC中断并配置优先级*/
    //3.1 设置中断优先级分组(只需要配置一次  可以放在main中)
    //nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //3.2 配置中断的抢占优先级和响应优先级 抢占优先级3,子优先级3
    nvic_irq_enable(BSP_KEY_EXTI_IRQN,3,3);
    /*4.配置GPIO中断 中断引脚资源端口和中断引脚*/
    syscfg_exti_line_config(BSP_KEY_EXTI_PORT_SOURCE,BSP_KEY_EXTI_PIN_SOURCE);
    /*5.初始化中断线  第三个参数指定上升沿和下降沿均触发*/
    exti_init(BSP_KEY_EXTI_LINE,EXTI_INTERRUPT,EXTI_TRIG_BOTH);


    /*6.使能中断和清除中断标志位*/
    //清除中断标志位
    exti_interrupt_flag_clear(BSP_KEY_EXTI_LINE);
    //使能中断
    exti_interrupt_enable(BSP_KEY_EXTI_LINE);
    /*7.编写中断服务函数*/
}
/************************************************
函数名称 ： BSP_KEY_EXTI_IRQHandler
功    能 ： 中断处理函数
参    数 ： 无
返 回 值 ： 无
作    者 ： LCEDA
*************************************************/
void BSP_KEY_EXTI_IRQHANDLER(void) {
    int i = 0;
    if(exti_interrupt_flag_get(BSP_KEY_EXTI_LINE) == SET)   // 中断标志位为1，按键按下
    {
        if(gpio_input_bit_get(BSP_KEY_PORT,BSP_KEY_PIN) == SET)  // 按键按下
        {
            /* 按键按下操作的功能 */
            gpio_bit_toggle(BSP_LED8_PORT,BSP_LED8_PIN);                   // led电平状态翻转
            printf("key press!\r\n");                              // 串口打印key press!
        } else {                                                  // 按键释放
            /* 按键松开操作的功能 */
            printf("key release!\r\n");                            // 串口打印key release!
        }
        exti_interrupt_flag_clear(BSP_KEY_EXTI_LINE);
    }
}
//按键扫描
void key_interrupt_run(void) {
    //led初始化
    led_init();
    //按键初始化
    key_interrupt_init();
}