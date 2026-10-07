/*
 *  delay.c
 *
 *  Created on: Oct 7, 2026
 *      Author: Venu
 */
#include "delay.h"
#include "stm32f103_regs.h"

void SysTick_Init(void)
{
    SYST_RVR = 72000U - 1U;
    SYST_CVR = 0U;

    SYST_CSR = (1U << 2) | (1U << 0);
}

void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        while ((SYST_CSR & (1U << 16)) == 0U)
        {
        }
    }
}

