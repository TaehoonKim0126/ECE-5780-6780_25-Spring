#include <stm32f0xx_hal.h>
#include "hal_gpio.h"
#include "assert.h"

static inline void My_HAL_RCC_GPIOC_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
    assert((RCC->AHBENR & RCC_AHBENR_GPIOCEN) == RCC_AHBENR_GPIOCEN);
}
void SystemClock_Config(void);

int lab1_main(void)
{
    HAL_Init();
    SystemClock_Config();

    My_HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitTypeDef initStr = {
        .Pin   = GPIO_PIN_6 | GPIO_PIN_7,
        .Mode  = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Pull  = GPIO_NOPULL
    };
    // Use custom My_HAL_GPIO_Init instead of HAL_GPIO_Init
    My_HAL_GPIO_Init(GPIOC, &initStr);

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // PC8 -> High
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // PC9 -> Low

    while (1)
    {
        HAL_Delay(200);
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
    }
}


