#ifndef Lcd_H
#define Lcd_H

#include "gd32f4xx.h"
#include "systick.h"
#include "Spi4.h"

#define LCD_W 240
#define LCD_H 280

//PF10 LCD_RESET
#define LCD_RST_RCU    RCU_GPIOF
#define LCD_RST_PORT   GPIOF
#define LCD_RST_PIN    GPIO_PIN_10
//PA6 LCD_DC
#define LCD_DC_RCU     RCU_GPIOA
#define LCD_DC_PORT    GPIOA
#define LCD_DC_PIN     GPIO_PIN_6
//PA4 LCD_CS
#define LCD_CS_RCU     RCU_GPIOA
#define LCD_CS_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_4
//PA7	SPI_SCK
#define LCD_BLK_RCU    RCU_GPIOA
#define LCD_BLK_PORT   GPIOA
#define LCD_BLK_PIN    GPIO_PIN_7


#define LCD_RST_L  gpio_bit_reset(LCD_RST_PORT,LCD_RST_PIN)//RST  PF10
#define LCD_RST_H  gpio_bit_set(LCD_RST_PORT,LCD_RST_PIN)

#define LCD_DC_L   gpio_bit_reset(LCD_DC_PORT,LCD_DC_PIN)//DC  PA6
#define LCD_DC_H   gpio_bit_set(LCD_DC_PORT,LCD_DC_PIN)
 		     
#define LCD_CS_L   gpio_bit_reset(LCD_CS_PORT,LCD_CS_PIN)//CS  PA4
#define LCD_CS_H   gpio_bit_set(LCD_CS_PORT,LCD_CS_PIN)

#define LCD_BLK_L  gpio_bit_reset(LCD_BLK_PORT,LCD_BLK_PIN)//BLK  PA7
#define LCD_BLK_H  gpio_bit_set(LCD_BLK_PORT,LCD_BLK_PIN)


#define Lcd_SpiRead   Spi4_Read
#define Lcd_SpiWrite  Spi4_Write

typedef enum
{
    lcdOrientation_Portrait = 0x00,  //����
    lcdOrientation_APortrait = 0xC0, //�������ֻ�������
    lcdOrientation_Left = 0x70,      //��������Ļ��home����ߣ��Ƚϳ��ã�
    lcdOrientation_Right = 0xA0      //��������Ļ��home���ұ�
} LcdOrientation;


void Lcd_Init();
//����LCD��Ļ���ӽǷ���
void Lcd_SetOrientation(LcdOrientation lcdOrientation);
uint16_t Lcd_GetW();
uint16_t Lcd_GetH();
//ָ����Ļд�뷶Χ
void Lcd_PushStart(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
//д������	
void Lcd_WriteData(uint8_t data);
//д��16λ����
void Lcd_WriteData16(uint16_t data);
#endif
