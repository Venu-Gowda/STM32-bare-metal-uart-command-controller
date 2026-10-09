/*
 *  delay.c
 *
 *  Created on: Oct 7, 2026
 *      Author: Venu
 */
#include "delay.h"
#include "stm32f103_regs.h"

static volatile uint32_t tick_ms = 0U;

void SysTick_Init(void)
{
    SYST_RVR = 72000U - 1U;
    SYST_CVR = 0U;

    /* Enable SysTick interrupt, processor clock, and counter. */
    SYST_CSR = (1U << 2) | (1U << 1) | (1U << 0);
}

void SysTick_Handler(void)
{
    tick_ms++;
}

uint32_t millis(void)
{
    return tick_ms;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = millis();

    while ((uint32_t)(millis() - start) < ms)
    {
    }
}

