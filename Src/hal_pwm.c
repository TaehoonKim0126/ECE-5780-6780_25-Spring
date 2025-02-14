// #include "main.h"
// #include "hal_gpio.h"
// #include <stm32f0xx_hal.h>



  
// /**
//  * @brief  Set the duty cycle for TIM2 PWM channels.
//  * @param  channel: 1 for CH1 (PC8), 2 for CH2 (PC9)
//  * @param  duty_cycle: Value from 0 to 100 (percentage)
//  */
// void TIM3_SetDutyCycle(uint8_t channel, uint16_t duty_cycle)
// {
//     if (duty_cycle > 100) duty_cycle = 100; // Limit to 100%

//     if (channel == 1)
//     {
//         TIM3->CCR1 = (duty_cycle * TIM3->ARR) / 100; // Update CH1 duty cycle
//     }
//     else if (channel == 2)
//     {
//         TIM3->CCR2 = (duty_cycle * TIM3->ARR) / 100; // Update CH2 duty cycle
//     }
// }


