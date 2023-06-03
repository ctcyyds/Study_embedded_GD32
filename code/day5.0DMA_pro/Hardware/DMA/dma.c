#include "dma.h"

#define ARRAY_SIZE 6
//uint8_t src_array[ARRAY_SIZE];
//uint8_t dest_array[ARRAY_SIZE]= {0};
char name1[ARRAY_SIZE] = {'h','e','l','l','o','\n'};
char name2[ARRAY_SIZE] = {0};

//*************************[DMA参数配置]*************************
void dma_config(uint32_t source_addr,uint32_t target_addr, int length)
{
    printf("eee\n");
    /* 使能DMA时钟 */
    rcu_periph_clock_enable(RCU_DMA1);
    /* 初始化DMA描述符 */
    dma_single_data_parameter_struct dma_init_struct;
    /* 配置DMA描述符 */
    dma_deinit(DMA1, DMA_CHANNEL);
    /*指定外设地址*/
    dma_init_struct.periph_addr = (uint32_t)source_addr;
    /*指定存储器地址*/
    dma_init_struct.memory0_addr = (uint32_t)target_addr;
    /*DMA传输方向为内存到内存*/
    dma_init_struct.direction = DMA_MEMORY_TO_MEMORY;
    /*指定传输数据的长度*/
    dma_init_struct.number = length;
    /*指定外设地址自增*/
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    /*指定存储器地址自增*/
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    /*指定数据宽度为8位*/
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
    /*指定DMA通道的优先级*/
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    /*禁止循环传输*/
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    /*初始化DMA单数据模式*/
    dma_single_data_mode_init(DMA1, DMA_CHANNEL, &dma_init_struct);

    /* 配置并使能中断*/
    nvic_irq_enable(DMA_CHANNEL_IRQn, 2, 2);
    /* 使能DMA传输完成中断 */
    dma_interrupt_enable(DMA1, DMA_CHANNEL, DMA_CHXCTL_FTFIE);
}
//===================================================

//*************************[内存到内存发送数据]*************************
void dma_memory_to_memory_transfer(void)
{
    printf("fff\n");
//    /* 初始化源数组 */
//    for (int i = 0; i < ARRAY_SIZE; i++)
//    {
//        src_array[i] = i;
//    }
//    printf("start:%d\n",dest_array[0]);
    /* 开始DMA传输 */
    dma_channel_enable(DMA1, DMA_CHANNEL);
}
//===================================================

//*************************[DMA中断服务函数]*************************
void DMA1_Channel0_IRQHandler(void)
{
    printf("haha\n");
    if ((dma_interrupt_flag_get(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF) == SET)&&
						//发现flag传输完成
            dma_flag_get(DMA1, DMA_CHANNEL,DMA_FLAG_FTF)==SET)
    {
			//打印数组中的每个元素
				printf("end: %s\r\n", name2);
        /* 清除中断标志 */
        dma_interrupt_flag_clear(DMA1, DMA_CHANNEL, DMA_INT_FLAG_FTF);

        /* 关闭 DMA 接收通道 */
        dma_channel_disable(DMA1, DMA_CHANNEL);
    }
}
//===================================================

//*************************[程序运行]*************************
void dma_init(void) {
    printf("ddd\n");
    /* 配置DMA */
    dma_config((uint32_t)name1, (uint32_t)name2, ARRAY_SIZE);
    /* 使用DMA传输数据 */
    dma_memory_to_memory_transfer();
}
//===================================================