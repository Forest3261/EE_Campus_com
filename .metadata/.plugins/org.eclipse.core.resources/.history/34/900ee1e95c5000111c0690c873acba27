/*
 * st7735.c
 *
 *  Created on: May 13, 2026
 *      Author: forest
 */

#include "st7735.h"


#include "spi.h"


#define ST7735_SWRESET  0x01
#define ST7735_SLPOUT   0x11
#define ST7735_COLMOD   0x3A
#define ST7735_MADCTL   0x36
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_DISPON   0x29

//GPIO控制
	// 选屏幕
static void ST7735_Select(void)
{		//CS-->PB12
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
}
	//quxiaoxuanze
static void ST7735_Unselect(void)
{
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

	//RS拉低，发送命令
static void ST7735_CommandMode(void)
{
    HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_RESET);
}
	//RS拉高，发送数据
static void ST7735_DataMode(void)
{		//RS-->PB10
    HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_SET);
}

	//拉低再拉高，复位屏幕
static void ST7735_Reset(void)
{		//RST-->PB11
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(120);
}

	//命令、数据写入
static void ST7735_WriteCommand(uint8_t cmd)
{
    ST7735_Select();
    ST7735_CommandMode();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    ST7735_Unselect();
}

static void ST7735_WriteData(uint8_t *data, uint16_t size)
{
    ST7735_Select();
    ST7735_DataMode();
    HAL_SPI_Transmit(&hspi2, data, size, HAL_MAX_DELAY);
    ST7735_Unselect();
}

static void ST7735_WriteData8(uint8_t data)
{
    ST7735_WriteData(&data, 1);
}

static void ST7735_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    ST7735_WriteCommand(ST7735_CASET);
    data[0] = 0x00;
    data[1] = x0;
    data[2] = 0x00;
    data[3] = x1;
    ST7735_WriteData(data, 4);

    ST7735_WriteCommand(ST7735_RASET);
    data[0] = 0x00;
    data[1] = y0;
    data[2] = 0x00;
    data[3] = y1;
    ST7735_WriteData(data, 4);

    ST7735_WriteCommand(ST7735_RAMWR);
}

void ST7735_Init(void)
{
    ST7735_Unselect();
    ST7735_Reset();

    ST7735_WriteCommand(ST7735_SWRESET);
    HAL_Delay(150);

    ST7735_WriteCommand(ST7735_SLPOUT);
    HAL_Delay(150);

    ST7735_WriteCommand(ST7735_COLMOD);
    ST7735_WriteData8(0x05);     // RGB565, 16-bit color
    HAL_Delay(10);

    ST7735_WriteCommand(ST7735_MADCTL);
    ST7735_WriteData8(0x00);     // Default portrait direction

    ST7735_WriteCommand(ST7735_DISPON);
    HAL_Delay(100);

    ST7735_FillScreen(ST7735_BLACK);
}

//   draw a piont
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t data[2];

    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) {
        return;
    }

    ST7735_SetAddressWindow(x, y, x, y);

    data[0] = color >> 8;
    data[1] = color & 0xFF;
    ST7735_WriteData(data, 2);
}

void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint8_t data[2];

    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) {
        return;
    }

    if ((x + w - 1) >= ST7735_WIDTH) {
        w = ST7735_WIDTH - x;
    }

    if ((y + h - 1) >= ST7735_HEIGHT) {
        h = ST7735_HEIGHT - y;
    }

    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    data[0] = color >> 8;
    data[1] = color & 0xFF;

    ST7735_Select();
    ST7735_DataMode();

    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        HAL_SPI_Transmit(&hspi2, data, 2, HAL_MAX_DELAY);
    }

    ST7735_Unselect();
}

//   Fill my screen!
void ST7735_FillScreen(uint16_t color)
{
    ST7735_FillRect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

void ST7735_TestScreen(void)
{
    ST7735_FillScreen(ST7735_BLACK);

    ST7735_FillRect(0, 0, 128, 40, ST7735_RED);
    ST7735_FillRect(0, 40, 128, 40, ST7735_GREEN);
    ST7735_FillRect(0, 80, 128, 40, ST7735_BLUE);
    ST7735_FillRect(0, 120, 128, 40, ST7735_YELLOW);
}
