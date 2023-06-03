#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "dma_m2p.h"
#include "motor.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "config.h"
#include "protocol.h"
#include "bluetooth.h"
#include "data_show.h"

float pitch, roll, yaw;

void mpu6050_init(void);
void mpu6050_get_data(void);
void control(void);

int main(void)
{
  systick_config();
  // NVIC优先级分组,配置一个全局就行  0~3   0~3
  nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
  // USART初始化
  usart_dma_config(115200);
	motor_init();
  encoder_init();
  // encoder_test();
//  mpu6050_init();
  // bluetooh init
  bluetooth_init();
  // data_show init
  data_show_init();
  
//motor_set_pwm(500);
  control();

  uint32_t last_mpu6050_get_data = get_system_ms();

  while (1)
  {
    uint32_t current_time = get_system_ms();
    if (current_time - last_mpu6050_get_data > 100)
    {
      last_mpu6050_get_data = current_time;
//      printf("last:%d\n", last_mpu6050_get_data);
      mpu6050_get_data();
    }
  }
}

// mpu init
//void mpu6050_init(void)
//{
//  uint8_t ret = 0;
//  while ((ret = mpu_dmp_init()))
//  {
////    printf("mpu6050 init fail	ret:%d\n", ret);
//  }
////  printf("mpu6050 init success\n");
//}

// get mpu6050 data
/*
pitch:俯仰角
roll:横滚角
yaw:航向角
*/
void mpu6050_get_data(void)
{
  if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0)
  {
//    printf("pitch:%.2f roll:%.2f  yaw:%.2f\n", pitch, roll, yaw);
    data_show_push(&pitch, 1);
  }
  else
  {
//    printf("get data fail\n");
    delay_1ms(5);
  }
}

// 电机旋转90度《==》编码器624
void control(void)
{
  // enter the target
  short target = 624;
  while (1)
  {
    motor_set_pwm(500);
    short current = encoder_get();
    if (current >= 624)
    {
      motor_set_pwm(0);
      delay_1ms(500);
      current = encoder_get();
      printf("end encoder=%d\n", current);
      break;
    }
  }
}