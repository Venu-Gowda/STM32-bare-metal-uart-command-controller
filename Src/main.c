#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include <stdint.h>
#include "command_parser.h"

int main(void)
{
    char received;

    Clock_Init();
    GPIO_Init();
    UART_Init();
    CommandParser_Init();

    UART_SendString("UART Command Parser Ready\r\n");

    while (1)
    {
        uint8_t event = UART_GetNextEvent(&received);

        if (event == UART_EVENT_DATA_LOSS)
        {
            CommandParser_NotifyDataLoss();
        }
        else if (event == UART_EVENT_CHAR)
        {
            CommandParser_Update(received);
        }
    }

}

