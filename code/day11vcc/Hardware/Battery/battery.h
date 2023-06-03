#ifndef __BATTERY_H_
#define __BATTERY_H_
#include "gd32f4xx.h"
#include "systick.h"
#include "dma_m2p.h"

void adc_config(void);

float adc_get_voltage();

void adc_run_test(void);


#endif