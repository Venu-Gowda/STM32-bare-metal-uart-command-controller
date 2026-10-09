#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"

int main(void)
{
    char received;
    uint32_t previous_led_time = 0U;

    Clock_Init();
    GPIO_Init();
    UART_Init();
    SysTick_Init();

    UART_SendString("UART RX Buffer Ready\r\n");

    while (1)
    {
        if (UART_TryReadChar(&received))
        {
            UART_SendChar(received);
        }

        if ((uint32_t)(millis() - previous_led_time) >= 2000U)
        {
            previous_led_time = millis();
            GPIO_Toggle();
        }
    }
}
