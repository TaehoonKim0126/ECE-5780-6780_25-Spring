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
    // ledInit.Pin   = GPIO_PIN_8 | GPIO_PIN_9;  3.1
    ledInit.Pin = GPIO_PIN_6 | GPIO_PIN_7; //
    ledInit.Mode  = GPIO_MODE_AF_PP;
    ledInit.Pull  = GPIO_NOPULL;
    ledInit.Speed = GPIO_SPEED_FREQ_LOW;
     // Set alternate function for TIM3_CH1, TIM3_CH2
    ledInit.Alternate = GPIO_AF1_TIM3; 
    My_HAL_GPIO_Init(GPIOC, &ledInit);
   
    // My_HAL_TIM2_Init_4Hz(); 3.1
    My_HAL_TIM3_PWM_Init();

    while (1)
    {
        // TIM2_IRQHandler will be called every 250 ms automatically 3.1

        // gradually increase and decrease brightness of led
        for (uint16_t i = 0; i <= 100; i += 5)
        {
            My_HAL_TIM3_SetDutyCycle(1, i); // Update CH1 duty cycle
            My_HAL_TIM3_SetDutyCycle(2, 100 - i); // Inverse for CH2
            HAL_Delay(50);
        }
    }
}
