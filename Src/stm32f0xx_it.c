/**
  ******************************************************************************
  * @file    Templates/Src/stm32f0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "main.h"
#include <stm32f0xx_hal.h>
#include <stm32f0xx_it.h>
#include "hal_gpio.h"

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
    static volatile uint32_t tickCount = 0;
    tickCount++;

     if (tickCount >= 200)
    {
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);  // Toggle blue LED on PC7
        tickCount = 0;  // Reset the counter
    }
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void EXTI0_1_IRQHandler(void)
{
    // Check if EXTI line 0 is pending.
    if (EXTI->PR & (1 << 0))
    {
        /// PC8 (orange LED)PC9 (green LED)
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
        
        volatile uint32_t delaycount;
        for (delaycount = 0; delaycount < 1500000; delaycount++)
        {
          // DO nothing just delay on going
        }

        // Toggle the LEDs again after the delay to indicate the end of the long-running interrupt.
        // Add a second LED toggle so that the green and orange LEDs should exchange once before
        // and after the delay loop.
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);

        // Clear pending flag for EXTI line 0.
        EXTI->PR |= (1 << 0);
    }
}