/*
 * uart.c
 *
 *  Created on: Oct 1, 2026
 *      Author: Venu
 */
#include "uart.h"
#include "stm32f103_regs.h"

void UART_Init(void)
{
    /* Enable GPIOA clock */
    RCC_APB2ENR |= (1U << 2);

    /* Enable USART1 clock */
    RCC_APB2ENR |= (1U << 14);


    /*
     * Configure PA9 as USART1_TX
     *
     * PA9 → GPIOA_CRH bits 7:4
     *
     * MODE9 = 11 → Output 50 MHz
     * CNF9  = 10 → Alternate function push-pull
     *
     * 1011 = 0xB
     */

    GPIOA_CRH &= ~(0xFU << 4);
    GPIOA_CRH |=  (0xBU << 4);


    /*
     * PCLK2 = 72 MHz
     * Baud rate = 9600
     */

    USART1_BRR = 0x1D4C;


    /* Enable USART1 */
    USART1_CR1 |= (1U << 13);

    /* Enable transmitter */
    USART1_CR1 |= (1U << 3);

    /*Enable RX interrupt*/
    USART1_CR1 |= (1U << 5);

    NVIC_ISER1 |= (1U << 5);    // Enable USART1 interrupt
}



void UART_SendChar(char c)
{
    /*
     * Wait until transmit data register is empty
     *
     * TXE = bit 7
     */

    while ((USART1_SR & (1U << 7)) == 0U)
    {
    }

    /* Put character into USART data register */
    USART1_DR = c;
}


void UART_SendString(const char *str)
{
    while (*str != '\0')
    {
        UART_SendChar(*str);
        str++;
    }
}

void USART1_IRQHandler(void)
{
    if (USART1_SR & (1U << 5))
    {
        char c;

        c = (char)USART1_DR;

        UART_SendChar(c);
    }
}

