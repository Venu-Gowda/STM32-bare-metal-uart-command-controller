#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"

int main(void)
{
    Clock_Init();
    GPIO_Init();
    UART_Init();
    SysTick_Init();

    UART_SendString("UART Interrupt Ready\r\n");

    while (1)
    {
        GPIO_Toggle();

        delay_ms(2000);
    }
}
