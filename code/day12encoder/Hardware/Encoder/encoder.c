#include "encoder.h"

// PA15 M1A Timer1  CH0 AF1
// PB3  M1B Timer1  CH1 AF1
static void encoder_gpio_init()
{
    // open clock
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_15);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_3);
}

void encoder_timer_init()
{
    rcu_periph_clock_enable(RCU_TIMER1);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_parameter_struct timer_init_param;
    timer_struct_para_init(&timer_init_param);

    timer_deinit(TIMER1);
    timer_init(TIMER1, &timer_init_param);

    // timer intput config
    timer_ic_parameter_struct timer_ic_param;
    timer_channel_input_struct_para_init(&timer_ic_param);
    // Configure the filter for the input(Make data smoother)
    timer_ic_param.icfilter = 0x50;
    timer_input_capture_config(TIMER1, TIMER_CH_0, &timer_ic_param);
    timer_input_capture_config(TIMER1, TIMER_CH_1, &timer_ic_param);

    // config encoder mode(The program decodes automatically)
    timer_quadrature_decoder_mode_config(TIMER1, TIMER_QUAD_DECODER_MODE2, TIMER_IC_POLARITY_FALLING, TIMER_IC_POLARITY_RISING);
    timer_enable(TIMER1);
}

//*************************[encoder init]*************************
void encoder_init(void)
{
    encoder_gpio_init();
    encoder_timer_init();
}
//===================================================

//*************************[encoder get data]*************************
short encoder_get(void)
{
    short enc = TIMER_CNT(TIMER1);
    return enc;
}
//===================================================

//*************************[test func]*************************
void encoder_test(void)
{
    encoder_init();
    while (1)
    {
        short enc = encoder_get();
        printf("enc = %d\n", enc);
        delay_1ms(500);
    }
}
//===================================================