/*
 * ad9834.c
 *
 *  Created on: May 10, 2026
 *      Author: forest
 */


#include "ad9834.h"
#include "spi.h"

static void AD9834_Write16(uint16_t data)
{
    uint8_t tx[2];

    tx[0] = (data >> 8) & 0xFF;
    tx[1] = data & 0xFF;

    HAL_GPIO_WritePin(AD9834_FSYNC_GPIO_Port, AD9834_FSYNC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(AD9834_FSYNC_GPIO_Port, AD9834_FSYNC_Pin, GPIO_PIN_SET);
}

void AD9834_Init(void)
{
    HAL_GPIO_WritePin(AD9834_FSYNC_GPIO_Port, AD9834_FSYNC_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(AD9834_RESET_GPIO_Port, AD9834_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(AD9834_RESET_GPIO_Port, AD9834_RESET_Pin, GPIO_PIN_RESET);

    AD9834_Write16(0x2100);   // B28 = 1, software reset = 1
    AD9834_SetFrequency(1000.0f);
    AD9834_Write16(0xC000);   // phase0 = 0
    AD9834_SetSine();
}

void AD9834_SetFrequency(float freq_hz)
{
    uint32_t freq_word;
    uint16_t freq_lsb;
    uint16_t freq_msb;

    freq_word = (uint32_t)((freq_hz * 268435456.0f) / AD9834_MCLK_HZ);

    freq_lsb = 0x4000 | (freq_word & 0x3FFF);
    freq_msb = 0x4000 | ((freq_word >> 14) & 0x3FFF);

    AD9834_Write16(0x2100);   // reset while loading frequency
    AD9834_Write16(freq_lsb);
    AD9834_Write16(freq_msb);
}

void AD9834_SetSine(void)
{
    AD9834_Write16(0x2000);   // B28 = 1, reset = 0, sine output
}
