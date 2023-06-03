#ifndef _DAC_H
#define _DAC_H

#include "gd32f4xx.h"
#include "systick.h"
#include "audio_source.h"
#include "stdio.h"

/* PA5  ADC1 */
#define DAC_AUDIO_RCU  		RCU_GPIOA
#define DAC_AUDIO_PORT  	GPIOA
#define DAC_AUDIO_PIN    	GPIO_PIN_5 

//timer
#define DAC_RCU_TIMER RCU_TIMER3
#define DAC_TIMER_IRQ TIMER3_IRQn
#define DAC_TIMER TIMER3
#define DAC_TIMER_IRQHandler  TIMER3_IRQHandler

//#define DAC_RCU_TIMER RCU_TIMER5
//#define DAC_TIMER_IRQ TIMER5_DAC_IRQn
//#define DAC_TIMER TIMER5
//#define DAC_TIMER_IRQHandler  TIMER5_DAC_IRQHandler

typedef struct 
{
  uint32_t audioIndex; // ��¼��ǰ��Ƶ����λ��
  const uint8_t *audioCurrentPlayP;    // ָ��ǰ���ŵ���Ƶ
	uint32_t audioLength; // ��¼��ǰ��Ƶ�ĳ���
	uint8_t audio_play_mode; // 0�ǲ���һ��  1��ѭ������
}audio;

extern uint8_t audio_status; // ����״̬

void dac_audio_init(uint16_t samplingRate);
void start_play_audio(const uint8_t *audio,uint32_t audio_length,uint8_t mode);
void stop_play_audio(void);

#endif