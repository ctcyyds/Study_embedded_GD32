#ifndef __LVGL_DEMO_H
#define __LVGL_DEMO_H
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "lvgl.h"

void first_screen(void);

//��ʽ
void obj_style(void);
//�ı�
void obj_label(void);
//��ť
void obj_btn(void);
//��ť����
void obj_btn_matrix(void);
//�ı������
void obj_text_area(void);
//���뷨
void obj_keyboard(void);

#endif