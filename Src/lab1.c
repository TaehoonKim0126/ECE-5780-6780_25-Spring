#include <assert.h>
#include <stm32f0xx.h>
#include <stm32f0xx_hal_gpio.h>

int lab1_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config(); // Configure the system clock

    // Enable GPIOC and GPIOA clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
    assert(RCC->AHBENR & RCC_AHBENR_GPIOCEN);
    assert(RCC->AHBENR & RCC_AHBENR_GPIOAEN);

    // Initialize LED Pins (PC6, PC7, PC8, PC9)
    My_HAL_GPIO_Init(GPIOC, 6);  // Red LED
    My_HAL_GPIO_Init(GPIOC, 7);  // Blue LED
    My_HAL_GPIO_Init(GPIOC, 8);  // Orange LED
    My_HAL_GPIO_Init(GPIOC, 9);  // Green LED

    // Initialize USER Button (PA0)
    My_HAL_GPIO_Init(GPIOA, 0);  // User Button

    // Set Initial LED States
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  // PC6 ON
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);  // PC8 ON
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);  // PC7 OFF
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);  // PC9 OFF

    // LED & button interaction loop
    while (1) {
        HAL_Delay(200); // Wait 200ms

        // Check if button is pressed
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) { // If PA0 (USER button) is HIGH
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
        }
    }
}
