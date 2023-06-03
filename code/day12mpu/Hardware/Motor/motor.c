#include "motor.h"

static void motor_gpio_init(void)
{
  rcu_periph_clock_enable(BSP_PWM_RCU);
  gpio_mode_set(BSP_PWM_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_PWM_PIN0 | BSP_PWM_PIN1 | BSP_PWM_PIN2 | BSP_PWM_PIN3);
  gpio_af_set(BSP_PWM_GPIO, BSP_PWM_AF, BSP_PWM_PIN0 | BSP_PWM_PIN1 | BSP_PWM_PIN2 | BSP_PWM_PIN3);
  gpio_output_options_set(BSP_PWM_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BSP_PWM_PIN0 | BSP_PWM_PIN1 | BSP_PWM_PIN2 | BSP_PWM_PIN3);
}

void motor_timer_init(void)
{
  rcu_periph_clock_enable(BSP_PWM_TIMER_RCU);
  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
  timer_parameter_struct timer_param;
  timer_struct_para_init(&timer_param);
  timer_param.prescaler = 200 - 1;
  timer_param.period = 7200 - 1;

  timer_deinit(BSP_PWM_TIMER);
  timer_init(BSP_PWM_TIMER, &timer_param);

  timer_oc_parameter_struct timer_oc_param;
  timer_channel_output_struct_para_init(&timer_oc_param);
  timer_oc_param.outputstate = TIMER_CCX_ENABLE;

  timer_channel_output_config(BSP_PWM_TIMER, BSP_PWM_CH0, &timer_oc_param);
  timer_channel_output_config(BSP_PWM_TIMER, BSP_PWM_CH1, &timer_oc_param);
  timer_channel_output_config(BSP_PWM_TIMER, BSP_PWM_CH2, &timer_oc_param);
  timer_channel_output_config(BSP_PWM_TIMER, BSP_PWM_CH3, &timer_oc_param);

  timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH0, 0);
  timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH1, 0);
  timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH2, 0);
  timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH3, 0);

  timer_channel_output_mode_config(BSP_PWM_TIMER, BSP_PWM_CH0, TIMER_OC_MODE_PWM0);
  timer_channel_output_mode_config(BSP_PWM_TIMER, BSP_PWM_CH1, TIMER_OC_MODE_PWM0);
  timer_channel_output_mode_config(BSP_PWM_TIMER, BSP_PWM_CH2, TIMER_OC_MODE_PWM0);
  timer_channel_output_mode_config(BSP_PWM_TIMER, BSP_PWM_CH3, TIMER_OC_MODE_PWM0);

  timer_primary_output_config(BSP_PWM_TIMER, ENABLE);
  timer_enable(BSP_PWM_TIMER);
}

void motor_init(void)
{
  motor_gpio_init();
  motor_timer_init();
}

void motor_set_pwm(int pwm)
{
  if (pwm > 0)
  {
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH0, pwm);
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH1, 0);
//    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH2, 0);
//    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH3, pwm);
  }
  else
  {
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH0, 0);
    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH1, pwm);
//    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH2, pwm);
//    timer_channel_output_pulse_value_config(BSP_PWM_TIMER, BSP_PWM_CH3, 0);
  }
}

void motor_test(void)
{
  motor_init();
  int pwm = 7200;
  while (1)
  {
    motor_set_pwm(pwm);
    pwm -= 200;
    if (pwm < -7200)
    {
      pwm = 7200;
    }
    delay_1ms(50);
  }
}