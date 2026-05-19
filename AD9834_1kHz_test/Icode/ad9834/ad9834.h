/*
 * ad9834.h
 *
 *  Created on: May 10, 2026
 *      Author: forest
 */

#ifndef AD9834_AD9834_H_
#define AD9834_AD9834_H_

#include "main.h"

#define AD9834_MCLK_HZ 75000000.0f

void AD9834_Init(void);
void AD9834_SetFrequency(float freq_hz);
void AD9834_SetSine(void);


#endif /* AD9834_AD9834_H_ */
