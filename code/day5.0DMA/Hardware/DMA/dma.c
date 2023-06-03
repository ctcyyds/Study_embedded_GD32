#include "dma.h"

#define ARRAY_SIZE 32
uint8_t src_array[ARRAY_SIZE];
uint8_t dest_array[ARRAY_SIZE]={0};

void dma_init(void){
	printf("ddd\n");
	/* 配置DMA */
	dma_config();
	/* 使用DMA传输数据 */
	dma_memory_to_memory_transfer();
}

void dma_config(void)
{
		printf("eee\n");
    /* 使能DMA时钟 */
    rcu_periph_clock_enable(RCU_DMA1);
    
    /* 初始化DMA描述符 */
    dma_single_data_parameter_struct dma_init_struct;
    
    /* 配置DMA描述符 */
    dma_deinit(DMA1, DMA_CHANNEL);
    
    dma_init_struct.periph_addr = (uint32_t)src_array;
    dma_init_struct.memory0_addr = (uint32_t)dest_array;
    dma_init_struct.direction = DMA_MEMORY_TO_MEMORY;
    dma_init_struct.number = ARRAY_SIZE;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
		dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_single_data_mode_init(DMA1, DMA_CHANNEL, &dma_init_struct);
    
		/* 配置并使能中断 */
    nvic_irq_enable(DMA_CHANNEL_IRQn, 2, 2);
    /* 使能DMA传输完成中断 */  					
    dma_interrupt_enable(DMA1, DMA_CHANNEL, DMA_CHXCTL_FTFIE);
}

void dma_memory_to_memory_transfer(void)
{
		printf("fff\n");
    /* 初始化源数组 */
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        src_array[i] = i;
    }
    printf("start:%d\n",dest_array[0]);
    /* 开始DMA传输 */
    dma_channel_enable(DMA1, DMA_CHANNEL);
}

/* DMA中断服务函数 */
void DMA1_Channel0_IRQHandler(void)
{	
		printf("haha\n");
    if ((dma_interrupt_flag_get(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF) == SET)&&
			dma_flag_get(DMA1, DMA_CHANNEL,DMA_FLAG_FTF)==SET)
    {
				
        printf("end:%d\n", dest_array[5]);
        /* 清除中断标志 */
        dma_interrupt_flag_clear(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF);
        
        /* 关闭 DMA 接收通道 */
        dma_channel_disable(DMA1, DMA_CHANNEL);
    }
}