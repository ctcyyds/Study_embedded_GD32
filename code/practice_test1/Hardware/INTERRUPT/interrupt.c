#include "interrupt.h"
#include "led.h"
/*�����жϵ��  �ⲿ�ж�*/

//��ʼ��
void key_interrupt_init(void) {
    /*1.����ʱ��*/
    //1.1 ����GPIO�˿ڵ�ʱ��
    rcu_periph_clock_enable(BSP_KEY_RCU);
    //1.2 ����ϵͳ����ʱ��
    rcu_periph_clock_enable(RCU_SYSCFG);
    /*2.����GPIO��ģʽ:����ģʽ������ģʽ*/
    gpio_mode_set(BSP_KEY_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BSP_KEY_PIN);
    /*3.ʹ��NVIC�жϲ��������ȼ�*/
    //3.1 �����ж����ȼ�����(ֻ��Ҫ����һ��  ���Է���main��)
    //nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //3.2 �����жϵ���ռ���ȼ�����Ӧ���ȼ� ��ռ���ȼ�3,�����ȼ�3
    nvic_irq_enable(BSP_KEY_EXTI_IRQN,3,3);
    /*4.����GPIO�ж� �ж�������Դ�˿ں��ж�����*/
    syscfg_exti_line_config(BSP_KEY_EXTI_PORT_SOURCE,BSP_KEY_EXTI_PIN_SOURCE);
    /*5.��ʼ���ж���  ����������ָ�������غ��½��ؾ�����*/
    exti_init(BSP_KEY_EXTI_LINE,EXTI_INTERRUPT,EXTI_TRIG_BOTH);


    /*6.ʹ���жϺ�����жϱ�־λ*/
    //����жϱ�־λ
    exti_interrupt_flag_clear(BSP_KEY_EXTI_LINE);
    //ʹ���ж�
    exti_interrupt_enable(BSP_KEY_EXTI_LINE);
    /*7.��д�жϷ�����*/
}
/************************************************
�������� �� BSP_KEY_EXTI_IRQHandler
��    �� �� �жϴ�����
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LCEDA
*************************************************/
void BSP_KEY_EXTI_IRQHANDLER(void) {
    int i = 0;
    if(exti_interrupt_flag_get(BSP_KEY_EXTI_LINE) == SET)   // �жϱ�־λΪ1����������
    {
        if(gpio_input_bit_get(BSP_KEY_PORT,BSP_KEY_PIN) == SET)  // ��������
        {
            /* �������²����Ĺ��� */
            gpio_bit_toggle(BSP_LED8_PORT,BSP_LED8_PIN);                   // led��ƽ״̬��ת
            printf("key press!\r\n");                              // ���ڴ�ӡkey press!
        } else {                                                  // �����ͷ�
            /* �����ɿ������Ĺ��� */
            printf("key release!\r\n");                            // ���ڴ�ӡkey release!
        }
        exti_interrupt_flag_clear(BSP_KEY_EXTI_LINE);
    }
}
//����ɨ��
void key_interrupt_run(void) {
    //led��ʼ��
    led_init();
    //������ʼ��
    key_interrupt_init();
}