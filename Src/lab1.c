#include <stm32f0xx_hal.h>
#include <assert.h>
#include "hal_gpio.h"

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

void SystemClock_Config(void);


static uint32_t debouncer = 0;
// 0 => Red on, Blue off
// 1 => Blue on, Red off
static int currentColor = 0;

int lab1_main(void)
{
    HAL_Init();
    SystemClock_Config();


    My_HAL_RCC_GPIOA_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();


    GPIO_InitTypeDef ledInit;
    ledInit.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
    ledInit.Mode  = GPIO_MODE_OUTPUT_PP;
    ledInit.Pull  = GPIO_NOPULL;
    ledInit.Speed = GPIO_SPEED_FREQ_LOW;
    My_HAL_GPIO_Init(GPIOC, &ledInit);

    // Configure PA0 as input + pull-down
    GPIO_InitTypeDef btnInit;
    btnInit.Pin   = GPIO_PIN_0;
    btnInit.Mode  = GPIO_MODE_INPUT;
    btnInit.Pull  = GPIO_PULLDOWN;
    btnInit.Speed = GPIO_SPEED_FREQ_LOW;
    My_HAL_GPIO_Init(GPIOA, &btnInit);

    // Initialize with RED on, BLUE off
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // Red on
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // Blue off
    currentColor = 0; 

    while (1)
    {
        HAL_Delay(5);       
        debouncer <<= 1;   

        if (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
        {
            debouncer |= 0x01;
        }


        if (debouncer == 0xFFFFFFFF)
        {
            // Toggle color
            if (currentColor == 0)
            {
                // Switch to Blue on, Red off
                My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
                My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
                currentColor = 1;
            }
            else
            {
                // Switch to Red on, Blue off
                My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
                My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
                currentColor = 0;
            }

            // Wait for release
            while (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
            {
                HAL_Delay(5);
            }
            // Reset debouncer
            debouncer = 0;
        }
    }
}
