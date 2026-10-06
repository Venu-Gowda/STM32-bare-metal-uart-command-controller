/*
 * clock.c
 *
 *  Created on: Oct 1, 2026
 *      Author: Venu
 */
#include "clock.h"
#include "stm32f103_regs.h"

void Clock_Init(void)
{
    /* Enable Flash prefetch */
    FLASH_ACR |= (1U << 4);

    /* Set Flash latency = 2 wait states */
    FLASH_ACR &= ~(7U << 0);
    FLASH_ACR |=  (2U << 0);


    /* Enable HSE */
    RCC_CR |= (1U << 16);

    /* Wait until HSE is ready */
    while ((RCC_CR & (1U << 17)) == 0U)
    {
    }


    /* PLL source = HSE */
    RCC_CFGR |= (1U << 16);

    /* PLL multiplier = x9 */
    RCC_CFGR &= ~(15U << 18);
    RCC_CFGR |=  (7U << 18);


    /* APB1 = HCLK / 2 */
    RCC_CFGR &= ~(7U << 8);
    RCC_CFGR |=  (4U << 8);


    /* Enable PLL */
    RCC_CR |= (1U << 24);

    /* Wait until PLL is ready */
    while ((RCC_CR & (1U << 25)) == 0U)
    {
    }


    /* Select PLL as system clock */
    RCC_CFGR &= ~(3U << 0);
    RCC_CFGR |=  (2U << 0);

    /* Wait until PLL becomes SYSCLK */
    while ((RCC_CFGR & (3U << 2)) != (2U << 2))
    {
    }
}

