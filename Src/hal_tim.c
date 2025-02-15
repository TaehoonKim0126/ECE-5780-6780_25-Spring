#include "main.h"
#include "hal_gpio.h"    
#include <stm32f0xx_hal.h>
#include <assert.h>


void My_HAL_TIM2_Init_4Hz(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    // Enable TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    TIM2->PSC = 7999;  // 8 MHz / 8000 = 1 kHz (1 ms per tick)
    TIM2->ARR = 250;    // 1 kHz / 250 = 4 Hz (250 ms per interrupt)
    
    // Update Interrupt act
    TIM2->DIER |= TIM_DIER_UIE;
    
    // TIM2
    TIM2->CR1 |= TIM_CR1_CEN;
     
    // NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
}

// interrupt handler
void TIM2_IRQHandler(void)
{

     if (TIM2->SR & TIM_SR_UIF) 
     {

       HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
      TIM2->SR &= ~TIM_SR_UIF;
     }
   
    

static unsigned int count = 0;  // counts from 0 to 200
count = (count + 1) % 201;  // wrap around at 201

//The 'counter' variable counts up to 200, then resets. 
// If the counter is 0-100, 'value' matches the counter, 
// otherwise, it decreases back from 100 to 0. This creates a triangle wave, and *value' updates both
// 'CCR1' and 'CCR2' to adjust the PWM duty cycle.
unsigned int var = (count <= 100) ? count : (200 - count);

TIM3->CCR1 = var;
TIM3->CCR2 = var;

}
