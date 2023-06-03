#ifndef __LVGL_DEMO_H
#define __LVGL_DEMO_H
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "lvgl.h"

void first_screen(void);

//样式
void obj_style(void);
//文本
void obj_label(void);
//按钮
void obj_btn(void);
//按钮矩阵
void obj_btn_matrix(void);
//文本输入框
void obj_text_area(void);
//输入法
void obj_keyboard(void);

#endif