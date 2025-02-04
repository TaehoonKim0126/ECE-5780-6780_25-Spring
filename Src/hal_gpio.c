#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

// Configure pin(s) according to Mode, Speed, Pull
void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    // We'll assume only bits 0..15 can be set in GPIO_Init->Pin
    // If multiple pins are set (e.g. PC6|PC7), loop through 0..15:
    for (uint8_t pin = 0; pin < 16; pin++)
    {
        if (GPIO_Init->Pin & (1 << pin))
        {
            // Mode: Output => "01"
            GPIOx->MODER &= ~(3U << (2 * pin));
            GPIOx->MODER |=  (1U << (2 * pin));

            // Output Type: Push-pull => bit = 0
            GPIOx->OTYPER &= ~(1U << pin);

            // Speed: Low => "00"
            GPIOx->OSPEEDR &= ~(3U << (2 * pin));

            // Pull: No pull => "00"
            GPIOx->PUPDR &= ~(3U << (2 * pin));
        }
    }
}

// Write pin(s) HIGH or LOW
void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET)
    {
        GPIOx->ODR |= GPIO_Pin;   // Set bits
    }
    else
    {
        GPIOx->ODR &= ~GPIO_Pin;  // Clear bits
    }
}

// Toggle pin(s)
void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}


void My_HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {
    GPIOx->MODER   &= ~(3U << (GPIO_Pin * 2));
    GPIOx->OTYPER  &= ~(1U << GPIO_Pin);
    GPIOx->OSPEEDR &= ~(3U << (GPIO_Pin * 2));
    GPIOx->PUPDR   &= ~(3U << (GPIO_Pin * 2));
}

GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    return (GPIOx->IDR & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

