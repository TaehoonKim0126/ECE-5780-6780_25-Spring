#include "main.h"
#include "hal_gpio.h"    
#include <stm32f0xx_hal.h>
#include <assert.h>


void My_HAL_TIM2_Init_4Hz(void)
{
    // Enable clock for TIM2 in RCC
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 

    // Optionally check if it's set
    assert((RCC->APB1ENR & RCC_APB1ENR_TIM2EN) == RCC_APB1ENR_TIM2EN);

    //    Configure Prescaler (PSC) and Auto-reload (ARR) for 4 Hz
    //    PSC + 1 = 8000 => 8 MHz / 8000 = 1 kHz
    //    ARR = 250 => 1 kHz / 250 = 4 Hz
    TIM2->PSC = 7999;     // (PSC + 1) = 8000
    TIM2->ARR = 250;      // 1000 Hz / 250 = 4 Hz

    // Enable update interrupt (UIE) in DIER
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable the counter (CEN) in CR1
    TIM2->CR1 |= TIM_CR1_CEN;

    // Enable TIM2 interrupt in NVIC
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
}

// interrupt handler
void TIM2_IRQHandler(void)
{
    // Check if update interrupt flag is set
    if (TIM2->SR & TIM_SR_UIF)
    {
        // Clear the update interrupt flag (UIF)
        TIM2->SR &= ~TIM_SR_UIF;
        
        // Toggle the on-board LEDs PC8 (orange) and PC9 (green)
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
    }
}
