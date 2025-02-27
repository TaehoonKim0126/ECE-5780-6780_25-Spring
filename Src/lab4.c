#include "main.h"
#include <stdint.h>
#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include "hal_gpio.h"


void SystemClock_Config(void);


// Globals for received data and its flag
volatile char receivedData;
volatile uint8_t newDataFlag = 0;



int lab4_main(void)
{
    HAL_Init();

    // Enable clocks for GPIOB, GPIOC, USART3
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // (We can leave GPIOB enabled if needed)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    SystemClock_Config();

    // Configure PC6-9 as outputs for LEDs
    GPIOC->MODER &= ~(1 << 19);
    GPIOC->MODER |=  (1 << 18);
    GPIOC->MODER &= ~(1 << 17);
    GPIOC->MODER |=  (1 << 16);
    GPIOC->MODER &= ~(1 << 15);
    GPIOC->MODER |=  (1 << 14);
    GPIOC->MODER &= ~(1 << 13);
    GPIOC->MODER |=  (1 << 12);

    // Push-pull, low-speed, no pull-up/down for PC6-9
    GPIOC->OTYPER  &= ~(1 << 6);
    GPIOC->OTYPER  &= ~(1 << 7);
    GPIOC->OTYPER  &= ~(1 << 8);
    GPIOC->OTYPER  &= ~(1 << 9);
    GPIOC->OSPEEDR &= ~(1 << 18);
    GPIOC->OSPEEDR &= ~(1 << 16);
    GPIOC->OSPEEDR &= ~(1 << 14);
    GPIOC->OSPEEDR &= ~(1 << 12);
    GPIOC->PUPDR   &= ~(1 << 19);
    GPIOC->PUPDR   &= ~(1 << 18);
    GPIOC->PUPDR   &= ~(1 << 17);
    GPIOC->PUPDR   &= ~(1 << 16);
    GPIOC->PUPDR   &= ~(1 << 15);
    GPIOC->PUPDR   &= ~(1 << 14);
    GPIOC->PUPDR   &= ~(1 << 13);
    GPIOC->PUPDR   &= ~(1 << 12);

    // Turn LEDs on
    GPIOC->ODR |= (1 << 9);
    GPIOC->ODR |= (1 << 8);
    GPIOC->ODR |= (1 << 7);
    GPIOC->ODR |= (1 << 6);

   
}



