#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART_EVENT_NONE      0U
#define UART_EVENT_CHAR      1U
#define UART_EVENT_DATA_LOSS 2U

void UART_Init(void);
void UART_SendChar(char c);
void UART_SendString(const char *str);

uint8_t UART_GetNextEvent(char *data);

#endif
