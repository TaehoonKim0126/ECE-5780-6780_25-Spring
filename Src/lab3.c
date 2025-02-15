#include "main.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>


void GPIO_Init_TIM3_PWM(void)
{
    // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set PC6, PC7 to Alternate Function mode (AF mode)
    GPIOC->MODER &= ~((3 << (2 * 6)) | (3 << (2 * 7))); // Clear mode bits
    GPIOC->MODER |= ((2 << (2 * 6)) | (2 << (2 * 7)));  // Set to AF mode

    // Set PC6 (TIM3_CH1) and PC7 (TIM3_CH2) to AF0
    GPIOC->AFR[0] &= ~((0xF << (4 * 6)) | (0xF << (4 * 7))); 
}

void TIM3_PWM_Init(void)
{
    // Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set Prescaler (PSC) and Auto-Reload Register (ARR) for 800 Hz PWM
    // PSC = 99 -> 8 MHz / (99+) = 80 kHz
    // ARR = 100 -> 80 kHz / (99+1) = 800 Hz PWM frequency
    TIM3->PSC = 99;
    TIM3->ARR = 100;

    // Configure TIM3 Channel 1 to PWM Mode 2 (OC1M = 111)
    // Configure TIM3 Channel 2 to PWM Mode 1 (OC2M = 110)
    TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M); // Clear bits
    TIM3->CCMR1 |= (7 << TIM_CCMR1_OC1M_Pos); // CH1: PWM Mode 2 (111)
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos); // CH2: PWM Mode 1 (110)

    // Enable Output Compare Preload for both channels
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;

    // Enable CH1 and CH2 outputs
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Enable TIM3 counter
    TIM3->CR1 |= TIM_CR1_CEN;
    
    // Set initial duty cycle to 20%
    TIM3->CCR1 = 90; // 20% duty cycle for CH1 (PWM Mode 2)
    TIM3->CCR2 = 10; // 20% duty cycle for CH2 (PWM Mode 1)
    
}

int lab3_main(void)
{
    // Initialize HAL and system clock
    HAL_Init();

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // PC6  - Red LED 
    // PC7  - Blue LED 
    // PC8  - Orange LED
    // PC9  - Green LED 
    GPIO_InitTypeDef ledInit;
    ledInit.Pin   = GPIO_PIN_8 | GPIO_PIN_9; 
    ledInit.Mode  = GPIO_MODE_OUTPUT_PP;
    ledInit.Pull  = GPIO_NOPULL;
    ledInit.Speed = GPIO_SPEED_LOW;
    My_HAL_GPIO_Init(GPIOC, &ledInit);

    My_HAL_TIM2_Init_4Hz();
    // Initialize GPIO for PWM
    GPIO_Init_TIM3_PWM();
    
    // Initialize TIM3 PWM
    TIM3_PWM_Init();
}