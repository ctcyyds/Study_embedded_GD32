#ifndef __WS2812_H__
#define __WS2812_H__
#include <stdint.h>

// 初始�?
void ws2812_init();
// 设置灯的颜色
void ws2812_set_color(uint8_t index, uint32_t color);

// 测试函数
void ws2812_test();

#endif