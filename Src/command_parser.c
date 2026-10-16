
#include "command_parser.h"
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "gpio.h"

#define COMMAND_BUFFER_SIZE 32U

static char command_buffer[COMMAND_BUFFER_SIZE];
static uint8_t command_index = 0U;
static uint8_t command_overflow = 0U;
static uint8_t discard_command = 0U;

static void CommandParser_Reset(void);
static void ProcessCommand(void);

void CommandParser_Init(void)
{
    CommandParser_Reset();
    discard_command = 0U;
}

static void CommandParser_Reset(void)
{
    command_index = 0U;
    command_buffer[0] = '\0';
    command_overflow = 0U;
}

void CommandParser_Update(char received)
{
    /* Ignore carriage return. */
    if (received == '\r')
    {
        return;
    }

    /* Recover at the next newline after a data-loss event. */
    if (discard_command == 1U)
    {
        if (received == '\n')
        {
            CommandParser_Reset();
            discard_command = 0U;
        }
        return;
    }

    /* Process or reject a complete command. */
    if (received == '\n')
    {
        if (command_overflow == 1U)
        {
            UART_SendString("Error: Command too long\r\n");
        }
        else if (command_index > 0U)
        {
            ProcessCommand();
        }

        CommandParser_Reset();
        return;
    }

    /* Stop collecting characters after command overflow. */
    if (command_overflow == 1U)
    {
        return;
    }

    /* Append a character only if there is space. */
    if (command_index < COMMAND_BUFFER_SIZE - 1U)
    {
        command_buffer[command_index] = received;
        command_index++;
        command_buffer[command_index] = '\0';
    }
    else
    {
        command_overflow = 1U;
    }
}

static void ProcessCommand(void)
{
    if (strcmp(command_buffer, "LED ON") == 0)
    {
        GPIO_Set();
        UART_SendString("LED is ON\r\n");
    }
    else if (strcmp(command_buffer, "LED OFF") == 0)
    {
        GPIO_Clear();
        UART_SendString("LED is OFF\r\n");
    }
    else
    {
        UART_SendString("Unknown command\r\n");
    }
}

void CommandParser_NotifyDataLoss(void)
{
    CommandParser_Reset();
    discard_command = 1U;
}
