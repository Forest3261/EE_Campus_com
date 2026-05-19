/*
 * st7735.h
 *
 *  Created on: May 13, 2026
 *      Author: forest
 */

#ifndef ST7735_ST7735_H_
#define ST7735_ST7735_H_


#include "main.h"

//     分辨率参数
#define ST7735_WIDTH   128
#define ST7735_HEIGHT  160

//     color define
#define ST7735_BLACK   0x0000
#define ST7735_WHITE   0xFFFF
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_BLUE    0x001F
#define ST7735_YELLOW  0xFFE0




//outside 函数

// 初始化
void ST7735_Init(void);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
//   draw a 矩形
void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_TestScreen(void);

#endif /* ST7735_ST7735_H_ */
