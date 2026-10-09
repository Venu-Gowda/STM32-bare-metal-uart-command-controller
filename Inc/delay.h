#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

void SysTick_Init(void);
void delay_ms(uint32_t ms);
uint32_t millis(void);

#endif
