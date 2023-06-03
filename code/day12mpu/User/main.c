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

void mpu6050_init(void);
//void mpu6050_get_data(void);


float pitch, roll, yaw;
void mpu6050_get_data(void)
{
  if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0)
  {
    printf("pitch:%.2f roll:%.2f  yaw:%.2f\n", pitch, roll, yaw);
		data_show_push(&pitch,1);
  }else {
		printf("get data fail\n");
		delay_1ms(5);
	}
}
int main(void)
{
  systick_config();
  // NVIC优先级分组,配置一个全局就行  0~3   0~3
  nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
  // USART初始化
  usart_dma_config(115200);
  // encoder_test();
  mpu6050_init();
	//bluetooh init
	bluetooth_init();
	//data_show init
	data_show_init();

  uint32_t last_mpu6050_get_data = get_system_ms();

  while (1)
  {
    uint32_t current_time = get_system_ms();
    if (current_time - last_mpu6050_get_data > 100)
    {
      last_mpu6050_get_data = current_time;
			printf("last:%d\n",last_mpu6050_get_data);
      mpu6050_get_data();
			
    }
  }
}

// mpu init
void mpu6050_init(void)
{
  uint8_t ret = 0;
  while((ret = mpu_dmp_init()))
  {
    printf("mpu6050 init fail	ret:%d\n", ret);
  }
  printf("mpu6050 init success\n");
}

// get mpu6050 data
/*
pitch:俯仰角
roll:横滚角
yaw:航向角
*/
