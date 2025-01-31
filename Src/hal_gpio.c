#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

 void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
    // Set pin mode
    GPIOx->MODER &= ~(3 << (GPIO_Init->Pin * 2));  // Clear mode bits
    GPIOx->MODER |= (GPIO_Init->Mode << (GPIO_Init->Pin * 2));

    // Configure output type (Push-Pull or Open-Drain)
    if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP || GPIO_Init->Mode == GPIO_MODE_AF_PP) {
        GPIOx->OTYPER &= ~(1 << GPIO_Init->Pin); // Push-pull
    } else if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD || GPIO_Init->Mode == GPIO_MODE_AF_OD) {
        GPIOx->OTYPER |= (1 << GPIO_Init->Pin); // Open-drain
    }

    // Configure pull-up/pull-down resistors
    GPIOx->PUPDR &= ~(3 << (GPIO_Init->Pin * 2)); // Clear existing pull-up/down
    GPIOx->PUPDR |= (GPIO_Init->Pull << (GPIO_Init->Pin * 2));

    // Configure speed
    GPIOx->OSPEEDR &= ~(3 << (GPIO_Init->Pin * 2)); // Clear speed bits
    GPIOx->OSPEEDR |= (GPIO_Init->Speed << (GPIO_Init->Pin * 2));
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
