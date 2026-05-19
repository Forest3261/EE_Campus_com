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


//xie wenzi

static const uint8_t Font5x7[][5] = {
    // 0-9
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9

    // A-Z
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
};

static const uint8_t BlankChar[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t ColonChar[5] = {0x00, 0x36, 0x36, 0x00, 0x00};
static const uint8_t DashChar[5]  = {0x08, 0x08, 0x08, 0x08, 0x08};
static const uint8_t DotChar[5]   = {0x00, 0x60, 0x60, 0x00, 0x00};

static const uint8_t *ST7735_GetCharData(char ch)
{
	//小写转化
    if (ch >= 'a' && ch <= 'z') {
        ch = ch - 'a' + 'A';
    }
    //数字字符整型化

    if (ch >= '0' && ch <= '9') {
        return Font5x7[ch - '0'];
    }

    //大写输出
    if (ch >= 'A' && ch <= 'Z') {
        return Font5x7[10 + ch - 'A'];
    }

    if (ch == ':') {
        return ColonChar;
    }

    if (ch == '-') {
        return DashChar;
    }

    if (ch == '.') {
        return DotChar;
    }

    return BlankChar;
}

void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg)
{
    const uint8_t *bitmap = ST7735_GetCharData(ch);

    for (uint8_t col = 0; col < 5; col++) {
        uint8_t line = bitmap[col];

        for (uint8_t row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                ST7735_DrawPixel(x + col, y + row, color);
            } else {
                ST7735_DrawPixel(x + col, y + row, bg);
            }
        }
    }

    // 字符之间留 1 像素间隔
    for (uint8_t row = 0; row < 7; row++) {
        ST7735_DrawPixel(x + 5, y + row, bg);
    }
}

void ST7735_WriteString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg)
{
    while (*str) {
        ST7735_WriteChar(x, y, *str, color, bg);
        x += 6;

        if (x + 5 >= ST7735_WIDTH) {
            x = 0;
            y += 8;
        }

        if (y + 7 >= ST7735_HEIGHT) {
            break;
        }

        str++;
    }
}



