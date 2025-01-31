#include <assert.h>
#include <stm32f0xx.h>
#include "hal_gpio.h"

void My_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  // Enable GPIOA clock
    
    // Assert to verify the clocks are enabled
    assert(RCC->AHBENR & RCC_AHBENR_GPIOCEN);
    assert(RCC->AHBENR & RCC_AHBENR_GPIOAEN);
}

int lab1_main(void) {
    HAL_Init(); // Reset peripherals, initialize Flash and Systick
    SystemClock_Config(); // Configure system clock

    // Enable GPIOC and GPIOA clocks
    My_HAL_RCC_GPIOC_CLK_ENABLE();

    // Initialize Red & Blue LED pins (PC6, PC7)
    My_HAL_GPIO_Init(GPIOC, 6);  // Red LED (PC6)
    My_HAL_GPIO_Init(GPIOC, 7);  // Blue LED (PC7)

    // Initialize USER button (PA0)
    My_HAL_GPIO_Init(GPIOA, 0);

    // Set initial LED state (PC6 ON, PC7 OFF)
    My_HAL_GPIO_WritePin(GPIOC, 6, GPIO_PIN_SET);
    My_HAL_GPIO_WritePin(GPIOC, 7, GPIO_PIN_RESET);

    // Assert to verify the initial LED states
    assert(My_HAL_GPIO_ReadPin(GPIOC, 6) == GPIO_PIN_SET);
    assert(My_HAL_GPIO_ReadPin(GPIOC, 7) == GPIO_PIN_RESET);

    uint32_t debouncer = 0;  // Button debounce variable
    uint8_t button_pressed = 0;  // Edge detection variable

    // LED & button interaction loop
    while (1) {
        // Shift the debouncer variable left
        debouncer = (debouncer << 1);

        // Read button state
        if (My_HAL_GPIO_ReadPin(GPIOA, 0) == GPIO_PIN_SET) {
            debouncer |= 0x01; // Set lowest bit if button is pressed
        }

        // Debouncing logic: Detect transition from LOW to HIGH (steady press)
        if ((debouncer == 0x7FFFFFFF) && (button_pressed == 0)) { 
            button_pressed = 1; // Prevent multiple toggles per press
            My_HAL_GPIO_TogglePin(GPIOC, 6);
            My_HAL_GPIO_TogglePin(GPIOC, 7);
        }

        // Reset edge detection when button is released
        if (debouncer == 0x00000000) {
            button_pressed = 0;
        }

        // Short delay for stability
        HAL_Delay(10);
    }
}
