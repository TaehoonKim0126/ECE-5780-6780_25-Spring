#include <stm32f0xx_hal.h>
#include "my_hal_gpio.h"
// #include "my_hal_rcc.h"  // if you have a custom RCC function

void SystemClock_Config(void);

int lab1_main(void)
{
    HAL_Init();
    SystemClock_Config();

    // If you created a custom clock-enabling function, call it here:
    // My_HAL_RCC_GPIOC_CLK_ENABLE();
    // Otherwise do your bitwise operation to enable RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIO_InitTypeDef gpioInit;
    gpioInit.Pin   = GPIO_PIN_8;            // For PC8 (orange LED)
    gpioInit.Mode  = GPIO_MODE_OUTPUT_PP;   // Output push-pull
    gpioInit.Speed = GPIO_SPEED_FREQ_LOW;   // Low speed
    gpioInit.Pull  = GPIO_NOPULL;           // No pull-up/down

    // Configure PC8
    My_HAL_GPIO_Init(GPIOC, &gpioInit);

    // If you want PC9 as well, call the same function with Pin = GPIO_PIN_9
    gpioInit.Pin = GPIO_PIN_9; // For PC9 (green LED)
    My_HAL_GPIO_Init(GPIOC, &gpioInit);

    // Now PC8 and PC9 are both push-pull outputs at low speed, no pull
    while (1)
    {
        // Toggle them or set them
        My_HAL_GPIO_TogglePin(GPIOC, 8); // toggles PC8
        My_HAL_GPIO_TogglePin(GPIOC, 9); // toggles PC9
        HAL_Delay(200);
    }
}
