#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define LED_PIN 13U

void GPIO_Init(void);
void GPIO_Set(void);
void GPIO_Clear(void);
void GPIO_Toggle(void);
uint8_t GPIO_Read(void);

#endif
