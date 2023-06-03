#include "dac.h"


uint8_t audio_status = 0; // 音乐状态

audio currentAudio; // 当前音频 

void dac_audio_gpio_init(void)
{
		//使能时钟
    rcu_periph_clock_enable(DAC_AUDIO_RCU);
    rcu_periph_clock_enable(RCU_DAC);
		
	//设置模拟模式  
	gpio_mode_set(DAC_AUDIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_AUDIO_PIN);
		
	// 复位DAC
	dac_deinit();  
    //使用软件方式触发
	dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // 使用定时器3作为触发源
	//触发使能
    dac_trigger_enable(DAC1);

	//使能输出缓冲区
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // 使能DAC
	
//    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0);  // 8位数据右对齐
//		// 软件触发使能 输出数据
//		dac_software_trigger_enable(DAC1);  
}

/* 定时器配置 
T=（TIM_Period+1）*（TIM_Prescaler+1）/TIMxCLK
samplingRate:采样率
*/
void Timer3_config(uint16_t samplingRate)
{
    timer_parameter_struct timer_initpara;
	// 设置中断优先级
    nvic_irq_enable(DAC_TIMER_IRQ, 2,2); 
	//使能时钟
	rcu_periph_clock_enable(DAC_RCU_TIMER);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(DAC_TIMER);  // 定时器复位
	/* TIMER2 configuration */
    timer_initpara.prescaler         = 24 - 1;  // 预分频
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // 对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // 计数方向
    timer_initpara.period            = 240000000 / 24 / samplingRate -1; // 周期
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // 时钟分频
    timer_initpara.repetitioncounter = 0; // 重复计数器
    timer_init(DAC_TIMER,&timer_initpara);
	
	/* TIMER2 interrupt enable */
    timer_interrupt_enable(DAC_TIMER,TIMER_INT_UP);  // 中断使能
		
	timer_disable(DAC_TIMER);
}

void dac_audio_init(uint16_t samplingRate)
{
		// gpio初始化
		dac_audio_gpio_init(); 					
		// 配置音频采样率
		Timer3_config(samplingRate); 	  
}


void DAC_TIMER_IRQHandler(void)
{
	if (timer_interrupt_flag_get(DAC_TIMER, TIMER_INT_FLAG_UP))
	 { 
		 /* clear TIMER interrupt flag */
		 timer_interrupt_flag_clear(DAC_TIMER, TIMER_INT_FLAG_UP);
			
		 dac_data_set(DAC1, DAC_ALIGN_8B_R, currentAudio.audioCurrentPlayP[currentAudio.audioIndex]);  // 8位数据右对齐 
		 dac_software_trigger_enable(DAC1);  // 软件触发使能 输出数据
		 
		 currentAudio.audioIndex ++; // 等待下一次数据
		 
		
		 if(currentAudio.audioLength == currentAudio.audioIndex){
			  /* 判断，播放模式 */
			 if(currentAudio.audio_play_mode == 1)  // 循环播放
			 {
			    currentAudio.audioIndex = 0;            // 从头开始
			 }else   // 单次播放
			 {		
				 /* 播放完毕 停止播放 */
			   timer_disable(DAC_TIMER);  // 关闭定时器
			   audio_status = 2; // 显示停止
			 }

		 }	 
	 }
}

/* 开始播放音乐 */
void start_play_audio(const uint8_t *audio,uint32_t audio_length,uint8_t mode)
{
	currentAudio.audioIndex = 0;              // 从头开始
	currentAudio.audioLength = audio_length;  // 当前音频的长度 
	currentAudio.audioCurrentPlayP = audio;   // 指向播放的音频 
	currentAudio.audio_play_mode = mode;     // 播放模式
	timer_enable(DAC_TIMER);        // 开启定时器
}

/* 停止播放音乐 */
void stop_play_audio(void)
{
	timer_disable(DAC_TIMER);  // 关闭定时器
}