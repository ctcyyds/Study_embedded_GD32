#include "dma.h"

#define ARRAY_SIZE 32
uint8_t src_array[ARRAY_SIZE];
uint8_t dest_array[ARRAY_SIZE]={0};

void dma_init(void){
	printf("ddd\n");
	/* ����DMA */
	dma_config();
	/* ʹ��DMA�������� */
	dma_memory_to_memory_transfer();
}

void dma_config(void)
{
		printf("eee\n");
    /* ʹ��DMAʱ�� */
    rcu_periph_clock_enable(RCU_DMA1);
    
    /* ��ʼ��DMA������ */
    dma_single_data_parameter_struct dma_init_struct;
    
    /* ����DMA������ */
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
    
		/* ���ò�ʹ���ж� */
    nvic_irq_enable(DMA_CHANNEL_IRQn, 2, 2);
    /* ʹ��DMA��������ж� */  					
    dma_interrupt_enable(DMA1, DMA_CHANNEL, DMA_CHXCTL_FTFIE);
}

void dma_memory_to_memory_transfer(void)
{
		printf("fff\n");
    /* ��ʼ��Դ���� */
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        src_array[i] = i;
    }
    printf("start:%d\n",dest_array[0]);
    /* ��ʼDMA���� */
    dma_channel_enable(DMA1, DMA_CHANNEL);
}

/* DMA�жϷ����� */
void DMA1_Channel0_IRQHandler(void)
{	
		printf("haha\n");
    if ((dma_interrupt_flag_get(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF) == SET)&&
			dma_flag_get(DMA1, DMA_CHANNEL,DMA_FLAG_FTF)==SET)
    {
				
        printf("end:%d\n", dest_array[5]);
        /* ����жϱ�־ */
        dma_interrupt_flag_clear(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF);
        
        /* �ر� DMA ����ͨ�� */
        dma_channel_disable(DMA1, DMA_CHANNEL);
    }
}