#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

// Configure pin(s) according to GPIO_Init->Mode, GPIO_Init->Pull, GPIO_Init->Speed.
void My_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
    // 1) Clear existing mode bits
    GPIOx->MODER &= ~(3 << (GPIO_Init->Pin * 2));
    // 2) Set new mode bits
    GPIOx->MODER |= (GPIO_Init->Mode << (GPIO_Init->Pin * 2));

    // 3) Configure output type for push-pull vs. open-drain
    if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP || GPIO_Init->Mode == GPIO_MODE_AF_PP) {
        GPIOx->OTYPER &= ~(1 << GPIO_Init->Pin);
    } else if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD || GPIO_Init->Mode == GPIO_MODE_AF_OD) {
        GPIOx->OTYPER |= (1 << GPIO_Init->Pin);
    }

    // 4) Pull-up/pull-down
    GPIOx->PUPDR &= ~(3 << (GPIO_Init->Pin * 2));
    GPIOx->PUPDR |= (GPIO_Init->Pull << (GPIO_Init->Pin * 2));

    // 5) Speed
    GPIOx->OSPEEDR &= ~(3 << (GPIO_Init->Pin * 2));
    GPIOx->OSPEEDR |= (GPIO_Init->Speed << (GPIO_Init->Pin * 2));
}

// Optionally, you have DeInit, ReadPin, WritePin, TogglePin below.
// We'll leave these as-is from your snippet, since they aren't changing 
// for this step of the lab.
void My_HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {
    GPIOx->MODER &= ~(3 << (GPIO_Pin * 2));
    GPIOx->OTYPER &= ~(1 << GPIO_Pin);
    GPIOx->OSPEEDR &= ~(3 << (GPIO_Pin * 2));
    GPIOx->PUPDR &= ~(3 << (GPIO_Pin * 2));
}

GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    return (GPIOx->IDR & (1 << GPIO_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    if (PinState == GPIO_PIN_SET) {
        GPIOx->ODR |= (1 << GPIO_Pin);
    } else {
        GPIOx->ODR &= ~(1 << GPIO_Pin);
    }
}

void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIOx->ODR ^= GPIO_Pin;
}
