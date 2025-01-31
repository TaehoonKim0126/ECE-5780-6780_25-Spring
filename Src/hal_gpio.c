#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

// Initialize GPIO pin
void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    // Set MODER (Output mode for LEDs, Input mode for button)
    if (GPIOx == GPIOC) {
        GPIOx->MODER &= ~(3 << (GPIO_Pin * 2)); // Clear mode bits
        GPIOx->MODER |= (1 << (GPIO_Pin * 2));  // Set output mode
    } else if (GPIOx == GPIOA && GPIO_Pin == 0) {
        GPIOx->MODER &= ~(3 << (GPIO_Pin * 2)); // Set input mode
    }

    // Set OTYPER (Push-Pull mode for LEDs, not needed for input)
    if (GPIOx == GPIOC) {
        GPIOx->OTYPER &= ~(1 << GPIO_Pin);
    }

    // Set OSPEEDR (Low speed for LEDs and button)
    GPIOx->OSPEEDR &= ~(3 << (GPIO_Pin * 2));

    // Set PUPDR (No pull-up/down for LEDs, pull-down for button)
    if (GPIOx == GPIOC) {
        GPIOx->PUPDR &= ~(3 << (GPIO_Pin * 2)); // No pull-up/down
    } else if (GPIOx == GPIOA && GPIO_Pin == 0) {
        GPIOx->PUPDR &= ~(3 << (GPIO_Pin * 2)); // Clear
        GPIOx->PUPDR |= (2 << (GPIO_Pin * 2));  // Enable pull-down
    }
}

// Reset GPIO pin configurations
void My_HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {
    GPIOx->MODER &= ~(3 << (GPIO_Pin * 2));  // Default: Input mode
    GPIOx->OTYPER &= ~(1 << GPIO_Pin);       // Default: Push-Pull
    GPIOx->OSPEEDR &= ~(3 << (GPIO_Pin * 2));// Default: Low speed
    GPIOx->PUPDR &= ~(3 << (GPIO_Pin * 2));  // Default: No pull-up/down
}

// Read GPIO pin state
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    return (GPIOx->IDR & (1 << GPIO_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

// Set GPIO pin state (HIGH or LOW)
void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    if (PinState == GPIO_PIN_SET) {
        GPIOx->ODR |= (1 << GPIO_Pin);  // Set pin HIGH
    } else {
        GPIOx->ODR &= ~(1 << GPIO_Pin); // Set pin LOW
    }
}

// Toggle GPIO pin state
void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIOx->ODR ^= (1 << GPIO_Pin);  // Toggle pin state
}
