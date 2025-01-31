#include <assert.h>
#include <stm32f0xx.h>
#include "hal_gpio.h"

void My_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  // Enable GPIOA clock

    assert(RCC->AHBENR & RCC_AHBENR_GPIOCEN);
    assert(RCC->AHBENR & RCC_AHBENR_GPIOAEN);
}

int lab1_main(void) {
    HAL_Init(); // Reset peripherals, initialize Flash and Systick
    SystemClock_Config(); // Configure system clock

    // Enable GPIOC and GPIOA clocks
    My_HAL_RCC_GPIOC_CLK_ENABLE();

    // Define GPIO configuration structure
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure Red LED (PC6)
    GPIO_InitStruct.Pin = 6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    My_HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Configure Blue LED (PC7)
    GPIO_InitStruct.Pin = 7;
    My_HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Configure USER button (PA0)
    GPIO_InitStruct.Pin = 0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    My_HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Set initial LED state (PC6 ON, PC7 OFF)
    My_HAL_GPIO_WritePin(GPIOC, 6, GPIO_PIN_SET);
    My_HAL_GPIO_WritePin(GPIOC, 7, GPIO_PIN_RESET);

    assert(My_HAL_GPIO_ReadPin(GPIOC, 6) == GPIO_PIN_SET);
    assert(My_HAL_GPIO_ReadPin(GPIOC, 7) == GPIO_PIN_RESET);

    uint32_t debouncer = 0;

    // LED & button interaction loop
    while (1) {
        debouncer = (debouncer << 1);

        // Read button state
        if (My_HAL_GPIO_ReadPin(GPIOA, 0) == GPIO_PIN_SET) {
            debouncer |= 0x01;
        }

        // Button is steadily pressed
        if (debouncer == 0x7FFFFFFF) { 
            // Keep blinking while button is held
            while (My_HAL_GPIO_ReadPin(GPIOA, 0) == GPIO_PIN_SET) {
                My_HAL_GPIO_TogglePin(GPIOC, 6);
                My_HAL_GPIO_TogglePin(GPIOC, 7);
                HAL_Delay(200);
            }
        }

        HAL_Delay(5);
    }
}
