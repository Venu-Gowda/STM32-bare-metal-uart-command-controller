#ifndef STM32F103_REGS_H
#define STM32F103_REGS_H

#include <stdint.h>

/* =========================
   FLASH
   ========================= */

#define FLASH_ACR \
    (*(volatile uint32_t *)0x40022000U)


/* =========================
   RCC
   ========================= */

#define RCC_CR \
    (*(volatile uint32_t *)0x40021000U)

#define RCC_CFGR \
    (*(volatile uint32_t *)0x40021004U)

#define RCC_APB2ENR \
    (*(volatile uint32_t *)0x40021018U)


/* =========================
   GPIOA
   ========================= */

#define GPIOA_CRH \
    (*(volatile uint32_t *)0x40010804U)


/* =========================
   GPIOC
   ========================= */

#define GPIOC_CRH \
    (*(volatile uint32_t *)0x40011004U)

#define GPIOC_IDR \
    (*(volatile uint32_t *)0x40011008U)

#define GPIOC_ODR \
    (*(volatile uint32_t *)0x4001100CU)


/* =========================
   USART1
   ========================= */

#define USART1_SR \
    (*(volatile uint32_t *)0x40013800U)

#define USART1_DR \
    (*(volatile uint32_t *)0x40013804U)

#define USART1_BRR \
    (*(volatile uint32_t *)0x40013808U)

#define USART1_CR1 \
    (*(volatile uint32_t *)0x4001380CU)

#define NVIC_ISER1 \
    (*(volatile uint32_t *)0xE000E104U)

#define NVIC_ICER1 (*(volatile uint32_t *)0xE000E184U)

/* =========================
   Systick
   ========================= */
#define SYST_CSR \
    (*(volatile uint32_t *)0xE000E010U)

#define SYST_RVR \
    (*(volatile uint32_t *)0xE000E014U)

#define SYST_CVR \
    (*(volatile uint32_t *)0xE000E018U)

#endif
