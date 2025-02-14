#include "main.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>
#include <assert.h>

/**
  * @brief  Configures TIM3 to generate PWM signals at 800 Hz.
  *         PC6 (TIM3_CH1) = Red LED, PC7 (TIM3_CH2) = Blue LED
  * @note   This function does NOT start the timer.
  */
void My_HAL_TIM3_PWM_Init(void)
{
    //Enable clock for TIM3 in RCC
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    assert((RCC->APB1ENR & RCC_APB1ENR_TIM3EN) == RCC_APB1ENR_TIM3EN);

    //    Configure TIM3 Prescaler (PSC) and Auto-reload Register (ARR)
    //    PSC = 99 => 8 MHz / (99+1) = 80 kHz
    //    ARR = 100 => 80 kHz / 100 = 800 Hz (PWM frequency)
    TIM3->PSC = 99;
    TIM3->ARR = 100;

    //    Configure CCMR1 register for PWM mode
    //    Set CH1 to PWM Mode 2, CH2 to PWM Mode 1
    TIM3->CCMR1 &= ~((TIM_CCMR1_OC1M | TIM_CCMR1_OC2M)); // Clear OC1M, OC2M
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // CH1: PWM Mode 2 (110)
    TIM3->CCMR1 |= (7 << TIM_CCMR1_OC2M_Pos); // CH2: PWM Mode 1 (111)

    //   Enable output compare preload for both channels
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    //    Enable CH1 and CH2 outputs in CCER
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    //   Set initial duty cycle to 20% of ARR
    TIM3->CCR1 = 20; // CH1 = 20% of 100
    TIM3->CCR2 = 20; // CH2 = 20% of 100

    //   Enable TIM3 counter
    TIM3->CR1 |= TIM_CR1_CEN; // start behavior
}

/**
  * @brief  Updates the PWM duty cycle for TIM3 CH1 or CH2.
  * @param  channel: 1 for CH1 (PC6), 2 for CH2 (PC7)
  * @param  duty_cycle: Value from 0 to 100 (percentage)
  */
void My_HAL_TIM3_SetDutyCycle(uint8_t channel, uint16_t duty_cycle)
{
    if (duty_cycle > 100) duty_cycle = 100; // Limit to 100%

    if (channel == 1)
    {
        TIM3->CCR1 = (duty_cycle * TIM3->ARR) / 100; // Update CH1 duty cycle
    }
    else if (channel == 2)
    {
        TIM3->CCR2 = (duty_cycle * TIM3->ARR) / 100; // Update CH2 duty cycle
    }
}
