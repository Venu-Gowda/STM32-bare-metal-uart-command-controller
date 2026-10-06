/*
 * gpio.c
 *
 *  Created on: Oct 1, 2026
 *      Author: Venu
 */
#include "gpio.h"
#include "stm32f103_regs.h"

void GPIO_Init(void)
{
    /* Enable GPIOC clock */
    RCC_APB2ENR |= (1U << 4);

    /*
     * PC13 is controlled by CRH
     * PC13 → bits 23:20
     */

    /* Clear PC13 configuration */
    GPIOC_CRH &= ~(0xFU << 20);

    /*
     * MODE13 = 01 → Output, 10 MHz
     * CNF13  = 00 → General purpose push-pull
     *
     * Combined = 0001
     */

    GPIOC_CRH |= (0x1U << 20);
}


void GPIO_Set(void)
{
    GPIOC_ODR |= (1U << LED_PIN);
}


void GPIO_Clear(void)
{
    GPIOC_ODR &= ~(1U << LED_PIN);
}


void GPIO_Toggle(void)
{
    GPIOC_ODR ^= (1U << LED_PIN);
}


uint8_t GPIO_Read(void)
{
    return (GPIOC_IDR >> LED_PIN) & 1U;
}

