#include "main.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>


int lab2_main(void)
{
    HAL_Init();
    SystemClock_Config();

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // PC6  - Red LED (toggled in main loop)
    // PC7  - Blue LED (toggled in SysTick interrupt)
    // PC8  - Orange LED
    // PC9  - Green LED 
    GPIO_InitTypeDef ledInit;
    ledInit.Pin   = GPIO_PIN_8 | GPIO_PIN_9; 
    ledInit.Mode  = GPIO_MODE_OUTPUT_PP;
    ledInit.Pull  = GPIO_NOPULL;
    ledInit.Speed = GPIO_SPEED_FREQ_LOW;
    My_HAL_GPIO_Init(GPIOC, &ledInit);
   
    My_HAL_TIM2_Init_4Hz();

    while (1)
    {
        // TIM2_IRQHandler will be called every 250 ms automatically
    }
}
