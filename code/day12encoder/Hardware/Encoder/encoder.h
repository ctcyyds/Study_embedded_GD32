#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "gd32f4xx.h"
#include "systick.h"
#include "dma_m2p.h"

void encoder_init(void);
// get the data
short encoder_get(void);
// test fcun
void encoder_test(void);

#endif