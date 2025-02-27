#include <stm32f0xx_hal.h>
#include <assert.h>
#include "hal_gpio.h"
#include "main.h"

void SystemClock_Config(void);

// Configure EXTI for the user button on PA0.
void Configure_EXTI_UserButton(void);


static inline void My_HAL_RCC_GPIOA_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    assert((RCC->AHBENR & RCC_AHBENR_GPIOAEN) == RCC_AHBENR_GPIOAEN);
}

static inline void My_HAL_RCC_GPIOC_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    assert((RCC->AHBENR & RCC_AHBENR_GPIOCEN) == RCC_AHBENR_GPIOCEN);
}


int lab2_main(void)
{
    HAL_Init();
    SystemClock_Config();

    My_HAL_RCC_GPIOA_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();

    // PC6  - Red LED (toggled in main loop)
    // PC7  - Blue LED (toggled in SysTick interrupt)
    // PC8  - Orange LED
    // PC9  - Green LED 
    GPIO_InitTypeDef ledInit;
    ledInit.Pin   = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    ledInit.Mode  = GPIO_MODE_OUTPUT_PP;
    ledInit.Pull  = GPIO_NOPULL;
    ledInit.Speed = GPIO_SPEED_FREQ_LOW;
    My_HAL_GPIO_Init(GPIOC, &ledInit);

    // Set the green LED (PC9) high.
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    
    // (Expect EXTI line 0 to be masked and trigger not enabled.)
    assert((EXTI->IMR & (1 << 0)) == 0);
    assert((EXTI->RTSR & (1 << 0)) == 0);

    // Call the function to configure EXTI for PA0.
    Configure_EXTI_UserButton();

    assert((EXTI->IMR & (1 << 0)) != 0);
    assert((EXTI->RTSR & (1 << 0)) != 0);
    
     // SysTick priority: 2 
    NVIC_SetPriority(SysTick_IRQn, 2);
    NVIC_EnableIRQ(SysTick_IRQn);

    // Exit priority: 1 (high priority)
    NVIC_SetPriority(EXTI0_1_IRQn, 3);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    // Start with red LED on and blue LED off.
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // Red LED on
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // Blue LED off

    // Main loop: toggle red LED every 500 ms.
    while (1)
    {
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6); // Toggle red LED (PC6)
        HAL_Delay(500);                         // Delay about 500 ms
    }
}
