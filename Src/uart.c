/*
 * uart.c
 *
 * STM32F103C8T6 UART Driver
 * USART1: 9600 baud
 * TX: PA9
 * RX: PA10
 */

#include "uart.h"
#include "stm32f103_regs.h"

#define UART_RX_BUFFER_SIZE 64U

static volatile char rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0U;
static volatile uint8_t rx_tail = 0U;
static volatile uint32_t rx_dropped = 0U;
static volatile uint8_t rx_loss_pending = 0U;

/* UART Initialization */
void UART_Init(void)
{
    /* Enable GPIOA clock */
    RCC_APB2ENR |= (1U << 2);

    /* Enable USART1 clock */
    RCC_APB2ENR |= (1U << 14);

    /* Configure PA9 as USART1_TX */
    /* MODE9 = 11, CNF9 = 10: AF push-pull, 50 MHz */
    GPIOA_CRH &= ~(0xFU << 4);
    GPIOA_CRH |=  (0xBU << 4);

    /* Configure PA10 as USART1_RX */
    /* MODE10 = 00, CNF10 = 01: floating input */
    GPIOA_CRH &= ~(0xFU << 8);
    GPIOA_CRH |=  (0x4U << 8);

    /* PCLK2 = 72 MHz, baud rate = 9600 */
    USART1_BRR = 0x1D4C;

    /* Enable USART1 */
    USART1_CR1 |= (1U << 13);

    /* Enable transmitter */
    USART1_CR1 |= (1U << 3);

    /* Enable receiver */
    USART1_CR1 |= (1U << 2);

    /* Enable RXNE interrupt */
    USART1_CR1 |= (1U << 5);

    /* Enable USART1 interrupt in NVIC */
    NVIC_ISER1 |= (1U << 5);
}

/* Transmit one character */
void UART_SendChar(char c)
{
    /* Wait until transmit data register is empty */
    while ((USART1_SR & (1U << 7)) == 0U)
    {
    }

    USART1_DR = (uint8_t)c;
}

/* Transmit a null-terminated string */
void UART_SendString(const char *str)
{
    while (*str != '\0')
    {
        UART_SendChar(*str);
        str++;
    }
}

/* USART1 receive interrupt handler */
void USART1_IRQHandler(void)
{
    if (USART1_SR & (1U << 5))
    {
        /* Read received character */
        char c = (char)USART1_DR;

        /* Calculate next buffer position */
        uint8_t next =
            (uint8_t)((rx_head + 1U) % UART_RX_BUFFER_SIZE);

        /* Check whether buffer has space */
        if (next != rx_tail)
        {
            rx_buffer[rx_head] = c;
            rx_head = next;
        }
        else
        {
            /* Buffer full: character dropped */
            rx_dropped++;
            rx_loss_pending = 1U;
        }
    }
}

/* Read one character if available */
uint8_t UART_GetNextEvent(char *data)
{
    uint32_t primask;
    uint8_t event;

    if (data == 0)
    {
        return UART_EVENT_NONE;
    }

    /*
     * Save interrupt state and disable interrupts
     * for a short critical section.
     */
    __asm volatile ("MRS %0, PRIMASK" : "=r"(primask));
    __asm volatile ("CPSID i" ::: "memory");

    /* Report data loss before reading buffered characters */
    if (rx_loss_pending != 0U)
    {
        rx_loss_pending = 0U;
        event = UART_EVENT_DATA_LOSS;
    }
    else if (rx_head == rx_tail)
    {
        event = UART_EVENT_NONE;
    }
    else
    {
        /* Read one character from the ring buffer */
        *data = rx_buffer[rx_tail];

        rx_tail =
            (uint8_t)((rx_tail + 1U) % UART_RX_BUFFER_SIZE);

        event = UART_EVENT_CHAR;
    }

    /* Restore the previous interrupt state */
    if ((primask & 1U) == 0U)
    {
        __asm volatile ("CPSIE i" ::: "memory");
    }

    return event;
}
