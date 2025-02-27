#include "main.h"
#include <stdint.h>
#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include "hal_gpio.h"


void SystemClock_Config(void);

void USART_IRQHandler(void);

// Globals for received data and its flag
volatile char receivedData;
volatile uint8_t newDataFlag = 0;

void USART_IRQHandler(void)
{
    // Check RXNE flag, read data, set new data flag
    if (USART3->ISR & USART_ISR_RXNE)
    {
        receivedData = USART3->RDR;
        newDataFlag = 1;
    }
}

int lab4_main(void)
{
    HAL_Init();

    // Enable clocks for GPIOB, GPIOC, USART3
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // (We can leave GPIOB enabled if needed)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    SystemClock_Config();

    // Configure PC6-9 as outputs for LEDs
    GPIOC->MODER &= ~(1 << 19);
    GPIOC->MODER |=  (1 << 18);
    GPIOC->MODER &= ~(1 << 17);
    GPIOC->MODER |=  (1 << 16);
    GPIOC->MODER &= ~(1 << 15);
    GPIOC->MODER |=  (1 << 14);
    GPIOC->MODER &= ~(1 << 13);
    GPIOC->MODER |=  (1 << 12);

    // Push-pull, low-speed, no pull-up/down for PC6-9
    GPIOC->OTYPER  &= ~(1 << 6);
    GPIOC->OTYPER  &= ~(1 << 7);
    GPIOC->OTYPER  &= ~(1 << 8);
    GPIOC->OTYPER  &= ~(1 << 9);
    GPIOC->OSPEEDR &= ~(1 << 18);
    GPIOC->OSPEEDR &= ~(1 << 16);
    GPIOC->OSPEEDR &= ~(1 << 14);
    GPIOC->OSPEEDR &= ~(1 << 12);
    GPIOC->PUPDR   &= ~(1 << 19);
    GPIOC->PUPDR   &= ~(1 << 18);
    GPIOC->PUPDR   &= ~(1 << 17);
    GPIOC->PUPDR   &= ~(1 << 16);
    GPIOC->PUPDR   &= ~(1 << 15);
    GPIOC->PUPDR   &= ~(1 << 14);
    GPIOC->PUPDR   &= ~(1 << 13);
    GPIOC->PUPDR   &= ~(1 << 12);

    // Turn LEDs on
    GPIOC->ODR |= (1 << 9);
    GPIOC->ODR |= (1 << 8);
    GPIOC->ODR |= (1 << 7);
    GPIOC->ODR |= (1 << 6);

    // ================================
    // CHANGED PART: Use PC4 (TX) & PC5 (RX) for USART3
    // ================================
    // 1) Set PC4, PC5 to Alternate Function mode
    GPIOC->MODER &= ~((3 << (2 * 4)) | (3 << (2 * 5)));   // Clear mode bits for PC4, PC5
    GPIOC->MODER |=  ((2 << (2 * 4)) | (2 << (2 * 5)));   // AF mode = "10"

    // 2) Configure the AF registers for USART3 on PC4, PC5
    //    For STM32F0, the relevant AFR index is [0] for pins below 8.
    //    PC4 offset = 4*4 = 16 bits, PC5 offset = 5*4 = 20 bits.
    //    AF value depends on the specific MCU; for some it might be AF1 or AF4.
    //    Here we assume AF1 for USART3 on PC4/PC5 (check your datasheet!).
    GPIOC->AFR[0] &= ~((0xF << 16) | (0xF << 20)); 
    GPIOC->AFR[0] |=  ((1 << 16) | (1 << 20));      // AF1 for PC4/PC5 as USART3

    // Set baud rate to 115200
    USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;

    // Enable TX, RX, RX interrupt, USART
    USART3->CR1 |= (1 << 2);  // RE (Receiver enable)
    USART3->CR1 |= (1 << 3);  // TE (Transmitter enable)
    USART3->CR1 |= (1 << 5);  // RXNEIE (RX not empty int)
    USART3->CR1 |= (1 << 0);  // UE (USART enable)

    // Reconfirm baud rate
    USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200;

    // Configure NVIC for USART3 (USART3_4_IRQn)
    NVIC_SetPriority(USART3_4_IRQn, 3);
    NVIC_EnableIRQ(USART3_4_IRQn);

    char command[3] = {0};
    int commandIndex = 0;

    USART_Trans_String("CMD?\r\n");

    while (1)
    {
        char receivedChar = USART_Receive_Character();

        // Accept only letters a-z or digits 0-2
        if ((receivedChar >= 'a' && receivedChar <= 'z') || 
            (receivedChar >= '0' && receivedChar <= '2'))
        {
            command[commandIndex++] = receivedChar;

            // Process once two chars are received
            if (commandIndex == 2)
            {
                Working_Command(command[0], command[1]);
                commandIndex = 0;
                USART_Trans_String("CMD?\r\n");
            }
        }
        else
        {
            USART_Trans_String("Error: Unrecognized command\r\nCMD?\r\n");
            commandIndex = 0;
        }
    }
}



