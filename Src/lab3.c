#include "main.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>

/**
 * @brief  Initialize GPIO for TIM3 PWM on PC6 and PC7.
 */
void GPIO_Init_TIM3_PWM(void)
{
    // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set PC6, PC7 to Alternate Function mode (AF mode)
    GPIOC->MODER &= ~((3 << (2 * 6)) | (3 << (2 * 7))); // Clear mode bits
    GPIOC->MODER |= ((2 << (2 * 6)) | (2 << (2 * 7)));  // Set to AF mode

    // Set PC6 (TIM3_CH1) and PC7 (TIM3_CH2) to AF0
    GPIOC->AFR[0] &= ~((0xF << (4 * 6)) | (0xF << (4 * 7))); // AF0 (TIM3)
}

/**
 * @brief  Initialize TIM3 to generate PWM at 800 Hz.
 */
void TIM3_PWM_Init(void)
{
    // Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set Prescaler (PSC) and Auto-Reload Register (ARR) for 800 Hz PWM
    // PSC = 99 -> 8 MHz / (99+1) = 80 kHz
    // ARR = 99 -> 80 kHz / (99+1) = 800 Hz PWM frequency
    TIM3->PSC = 99;
    TIM3->ARR = 99;

    // Configure TIM3 Channel 1 and 2 for PWM Mode 1 (OCxM = 110)
    TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M); // Clear bits
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // CH1: PWM Mode 1
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos); // CH2: PWM Mode 1

    // Enable Output Compare Preload for both channels
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    // Enable CH1 and CH2 outputs
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Set initial duty cycle to 20%
    TIM3->CCR1 = TIM3->ARR / 50; // 20% duty cycle for CH1
    TIM3->CCR2 = TIM3->ARR / 50; // 20% duty cycle for CH2

    // Enable TIM3 counter
    TIM3->CR1 |= TIM_CR1_CEN;
}

int lab3_main(void)
{
    // Initialize HAL and system clock
    HAL_Init();
    SystemClock_Config();
    
    // Initialize GPIO for PWM
    GPIO_Init_TIM3_PWM();
    
    // Initialize TIM3 PWM
    TIM3_PWM_Init();

    while (1)
    {  
        // Smoothly increase and decrease brightness of the LED
        for (uint16_t i = 0; i <= 100; i += 5)
        {
            TIM3->CCR1 = (i * TIM3->ARR) / 100;  // CH1 (Red LED)
            TIM3->CCR2 = ((100 - i) * TIM3->ARR) / 100; // CH2 (Blue LED)
            HAL_Delay(50);
        }
        for (uint16_t i = 100; i >= 0; i -= 5)
        {
            TIM3->CCR1 = (i * TIM3->ARR) / 100;  // CH1 (Red LED)
            TIM3->CCR2 = ((100 - i) * TIM3->ARR) / 100; // CH2 (Blue LED)
            HAL_Delay(50);
        }
    }
}
