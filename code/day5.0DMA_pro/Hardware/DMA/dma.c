#include "dma.h"

#define ARRAY_SIZE 6
//uint8_t src_array[ARRAY_SIZE];
//uint8_t dest_array[ARRAY_SIZE]= {0};
char name1[ARRAY_SIZE] = {'h','e','l','l','o','\n'};
char name2[ARRAY_SIZE] = {0};

//*************************[DMA��������]*************************
void dma_config(uint32_t source_addr,uint32_t target_addr, int length)
{
    printf("eee\n");
    /* ʹ��DMAʱ�� */
    rcu_periph_clock_enable(RCU_DMA1);
    /* ��ʼ��DMA������ */
    dma_single_data_parameter_struct dma_init_struct;
    /* ����DMA������ */
    dma_deinit(DMA1, DMA_CHANNEL);
    /*ָ�������ַ*/
    dma_init_struct.periph_addr = (uint32_t)source_addr;
    /*ָ���洢����ַ*/
    dma_init_struct.memory0_addr = (uint32_t)target_addr;
    /*DMA���䷽��Ϊ�ڴ浽�ڴ�*/
    dma_init_struct.direction = DMA_MEMORY_TO_MEMORY;
    /*ָ���������ݵĳ���*/
    dma_init_struct.number = length;
    /*ָ�������ַ����*/
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    /*ָ���洢����ַ����*/
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    /*ָ�����ݿ��Ϊ8λ*/
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
    /*ָ��DMAͨ�������ȼ�*/
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    /*��ֹѭ������*/
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    /*��ʼ��DMA������ģʽ*/
    dma_single_data_mode_init(DMA1, DMA_CHANNEL, &dma_init_struct);

    /* ���ò�ʹ���ж�*/
    nvic_irq_enable(DMA_CHANNEL_IRQn, 2, 2);
    /* ʹ��DMA��������ж� */
    dma_interrupt_enable(DMA1, DMA_CHANNEL, DMA_CHXCTL_FTFIE);
}
//===================================================

//*************************[�ڴ浽�ڴ淢������]*************************
void dma_memory_to_memory_transfer(void)
{
    printf("fff\n");
//    /* ��ʼ��Դ���� */
//    for (int i = 0; i < ARRAY_SIZE; i++)
//    {
//        src_array[i] = i;
//    }
//    printf("start:%d\n",dest_array[0]);
    /* ��ʼDMA���� */
    dma_channel_enable(DMA1, DMA_CHANNEL);
}
//===================================================

//*************************[DMA�жϷ�����]*************************
void DMA1_Channel0_IRQHandler(void)
{
    printf("haha\n");
    if ((dma_interrupt_flag_get(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF) == SET)&&
						//����flag�������
            dma_flag_get(DMA1, DMA_CHANNEL,DMA_FLAG_FTF)==SET)
    {
			//��ӡ�����е�ÿ��Ԫ��
				printf("end: %s\r\n", name2);
        /* ����жϱ�־ */
        dma_interrupt_flag_clear(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF);

        /* �ر� DMA ����ͨ�� */
        dma_channel_disable(DMA1, DMA_CHANNEL);
    }
}
//===================================================

//*************************[��������]*************************
void dma_init(void) {
    printf("ddd\n");
    /* ����DMA */
    dma_config((uint32_t)name1, (uint32_t)name2, ARRAY_SIZE);
    /* ʹ��DMA�������� */
    dma_memory_to_memory_transfer();
}
//===================================================