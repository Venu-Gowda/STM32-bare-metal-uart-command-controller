# STM32 UART Command Controller

Bare-metal firmware for **STM32F103C8T6 (Blue Pill)** written in **Embedded C with no HAL and no CubeMX** — direct register manipulation using a custom register map. The firmware implements a UART command-line interface over USART1, with interrupt-driven receive, a ring buffer, and a line-based command parser.

**Author:** Venu S  
**MCU:** STM32F103C8T6 (ARM Cortex-M3 @ 72 MHz)  
**Toolchain:** STM32CubeIDE, GCC ARM  
**Status:** 🚧 In Development — UART Command Controller stage complete

---

## 🎯 Project Goals

- Write **register-level firmware** without HAL or LL libraries
- Implement clean, modular drivers for each peripheral
- Build an **event-driven architecture** using interrupts, ring buffers, and a command parser
- Demonstrate understanding of ARM Cortex-M3 architecture, memory-mapped I/O, and linker scripts
- Serve as a reference implementation for bare-metal embedded development

---

## 🗺 Development Roadmap

| # | Feature | Status | Notes |
|---|---------|--------|-------|
| 1 | GPIO Driver | ✅ Complete | PC13 LED (active-low) |
| 2 | Clock Configuration | ✅ Complete | HSE → PLL → 72 MHz SYSCLK |
| 3 | UART TX | ✅ Complete | USART1 @ 9600 bps, PA9 |
| 4 | UART RX | ✅ Complete | PA10, interrupt-driven |
| 5 | UART Interrupts | ✅ Complete | RXNE ISR on NVIC |
| 6 | Ring Buffer | ✅ Complete | 64-byte, decouples ISR from main loop |
| 7 | Command Parser | ✅ Complete | Line-terminated, validates input |
| 8 | LED Command Interface | ✅ Complete | `LED ON` / `LED OFF` with ack |
| 9 | Error Handling | ✅ Complete | Unknown cmd, overflow, data-loss recovery |
| 10 | Timer (TIM2) | ⏳ Planned | Periodic interrupt |
| 11 | ADC | ⏳ Planned | Sensor monitoring (future stage) |
| 12 | Additional Commands | ⏳ Planned | `HELP`, `STATUS`, `RESET` |

---

## 🏗 System Architecture
Computer / Serial Terminal
↓ sends "LED ON" / "LED OFF"
USART1 Driver (PA10 RX, PA9 TX)
↓
UART RX Interrupt (RXNE)
↓ stores byte
Ring Buffer (64 bytes, lock-free)
↓
Main Loop → Command Parser
↓ recognizes \n-terminated commands
GPIO Driver (PC13)
↓
UART Response → back to terminal

---

## 🔌 Pin Connections

| STM32 Pin | Function | Connected To |
|-----------|----------|--------------|
| PC13 | GPIO Output | Onboard LED (active-low) |
| PA9 | USART1_TX | USB-UART RX |
| PA10 | USART1_RX | USB-UART TX |
| GND | Ground | USB-UART GND (common) |
| PA13 (SWDIO) | SWD Data | ST-Link V2 |
| PA14 (SWCLK) | SWD Clock | ST-Link V2 |

---

## ⚙️ Clock Configuration

| Clock | Source | Frequency |
|-------|--------|-----------|
| HSE | External crystal | 8 MHz |
| PLL | HSE × 9 | 72 MHz |
| SYSCLK | PLL output | 72 MHz |
| AHB | SYSCLK | 72 MHz |
| APB1 | AHB / 2 | 36 MHz |
| APB2 | AHB / 1 | 72 MHz |

---

## 💻 Command Interface

The firmware exposes a UART command-line interface over USART1 @ 9600 bps.

### Supported Commands

| Command | Action | Response |
|---------|--------|----------|
| `LED ON` | Turn PC13 LED on | `LED is ON` |
| `LED OFF` | Turn PC13 LED off | `LED is OFF` |
| Unknown | Reject | `Unknown command` |
| Too long | Reject | `Error: Command too long` |

### Example Session
UART Command Parser Ready

LED ON
LED is ON

LED OFF
LED is OFF

HELLO
Unknown command

text

### What Happens When You Type `LED ON`

1. Computer sends characters over serial
2. USART1 receives each character on PA10
3. RXNE interrupt fires — `USART1_IRQHandler()` reads `USART1_DR` and stores byte in ring buffer
4. `main()` calls `UART_GetNextEvent()` to retrieve received characters
5. `CommandParser_Update()` collects characters until `\n`
6. `ProcessCommand()` compares against `"LED ON"`
7. `GPIO_Set()` drives PC13 low → LED on
8. `UART_SendString()` sends `LED is ON` back to terminal

---

## 🧱 Register-Level Programming

This project intentionally avoids HAL and LL libraries. Instead, it uses:

- **Custom register map** (`stm32f103_regs.h`) — struct-based access to memory-mapped peripherals
- **Bitwise operations** — configure specific bits without affecting others
- **Manual clock gating** — RCC register configuration

### Example — Enable GPIOC clock and configure PC13 as output

```c
// Enable GPIOC clock (RCC_APB2ENR bit 4)
RCC->APB2ENR |= (1 << 4);

// Configure PC13 as output (2 bits per pin in CRH)
GPIOC->CRH &= ~(0xF << 20);     // Clear PC13 config
GPIOC->CRH |=  (0x2 << 20);     // Output push-pull, 2 MHz
```
## Example — USART1 initialization for 9600 bps @ 72 MHz
```c
// Enable USART1 clock (RCC_APB2ENR bit 14)
RCC->APB2ENR |= (1 << 14);

// Enable GPIOA clock for PA9/PA10
RCC->APB2ENR |= (1 << 2);

// Configure PA9 as alternate function push-pull (TX)
GPIOA->CRH &= ~(0xF << 4);
GPIOA->CRH |=  (0xB << 4);      // AF output, 50 MHz

// Configure PA10 as input floating (RX)
GPIOA->CRH &= ~(0xF << 8);
GPIOA->CRH |=  (0x4 << 8);      // Input floating

// Set baud rate: USARTDIV = 72000000 / (16 * 9600) = 468.75
USART1->BRR = (468 << 4) | 12;

// Enable USART1: TX (TE), RX (RE), RXNE interrupt, USART enable
USART1->CR1 |= (1 << 3);        // TE: Transmitter enable
USART1->CR1 |= (1 << 2);        // RE: Receiver enable
USART1->CR1 |= (1 << 5);        // RXNEIE: RX interrupt enable
USART1->CR1 |= (1 << 13);       // UE: USART enable

// Enable USART1 IRQ in NVIC (IRQ number 37)
NVIC->ISER[1] |= (1 << (37 - 32));

