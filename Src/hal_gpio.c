#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    for (uint8_t pin = 0; pin < 16; pin++)
    {
        if (GPIO_Init->Pin & (1 << pin))  
        {
         
            if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP)
            {
                // "01" 
                GPIOx->MODER &= ~(3U << (2 * pin));
                GPIOx->MODER |=  (1U << (2 * pin)); 
                
                // Push-pull => OTYPER = 0
                GPIOx->OTYPER &= ~(1U << pin);
            }
            else if (GPIO_Init->Mode == GPIO_MODE_INPUT)
            {
                // "00" => input
                GPIOx->MODER &= ~(3U << (2 * pin));
            }


            // PUPDR
            GPIOx->PUPDR &= ~(3U << (2 * pin)); 
            GPIOx->PUPDR |=  (GPIO_Init->Pull << (2 * pin));

            // OSPEEDR
            GPIOx->OSPEEDR &= ~(3U << (2 * pin));
            GPIOx->OSPEEDR |=  (GPIO_Init->Speed << (2 * pin));
        }
    }
}

GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    return (GPIOx->IDR & GPIO_Pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET)
        GPIOx->ODR |= GPIO_Pin;
    else
        GPIOx->ODR &= ~GPIO_Pin;
}

void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}

void My_HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    GPIOx->MODER   &= ~(3U << (GPIO_Pin * 2));
    GPIOx->OTYPER  &= ~(1U << GPIO_Pin);
    GPIOx->OSPEEDR &= ~(3U << (GPIO_Pin * 2));
    GPIOx->PUPDR   &= ~(3U << (GPIO_Pin * 2));
}
