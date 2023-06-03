#ifndef __DMA_H_
#define __DMA_H_

#include "gd32f4xx.h"
#include "systick.h"
#include "stdio.h"

#define DMA_CHANNEL DMA_CH0
#define  DMA_CHANNEL_IRQn  DMA1_Channel0_IRQn

void dma_init(void);
void dma_config(void);
void dma_memory_to_memory_transfer(void);

#endif