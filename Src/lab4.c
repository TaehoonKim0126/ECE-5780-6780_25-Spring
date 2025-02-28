#include "stm32f072xb.h"
#include "system_setup.h"
#include <stdbool.h>
#include <stdio.h> 

// External Function
extern void SystemClock_Config(void);

// USART Function Prototypes
void USART3_Init(int part);
void USART3_4_IRQHandler(void);
char USART3_ReceiveChar(void);
bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout);
void USART_SendChar(char c);
void USART_SendString(const char *str);

// LED Control Functions
void Red_toggle(void);
void Blue_toggle(void);
void GPIO_Init(void);
void LED_Process(char led, char command);

// Global Variables
volatile uint8_t usart_mode = 1;  // 1: one character, 2: two characters
volatile char received_character = 0;
volatile bool bool_data = false;
volatile char buffer[2];
volatile uint8_t uart_rx_index = 0;

#define BAUD_RATE 115200
#define SYS_CLOCK 8000000  // 8 MHz Clock

// USART3 initialization (PC4: TX, PC5: RX)
void USART3_Init(int part) {
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // Set PC4 and PC5 to Alternate Function mode (AF1)
    GPIOC->MODER &= ~((3 << (4 * 2)) | (3 << (5 * 2)));
    GPIOC->MODER |=  ((2 << (4 * 2)) | (2 << (5 * 2)));
    
    GPIOC->AFR[0] &= ~((0xF << (4 * 4)) | (0xF << (5 * 4)));
    GPIOC->AFR[0] |=  ((1 << (4 * 4)) | (1 << (5 * 4)));
    
    // Set Baud Rate (8MHz/115200)
    USART3->BRR = SYS_CLOCK / BAUD_RATE; 
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    
    NVIC_EnableIRQ(USART3_4_IRQn);
    NVIC_SetPriority(USART3_4_IRQn, 1);
    
    usart_mode = part;  // Mode setting: 1 or 2
    USART3->CR1 |= USART_CR1_UE;
}

// USART3/4 IRQ Handler (handles both mode 1 and 2)
void USART3_4_IRQHandler(void) {
    if (USART3->ISR & USART_ISR_RXNE) {
        char temp_char = USART3->RDR;
        // Ignore carriage return and line feed (PuTTY)
        if (temp_char == '\r' || temp_char == '\n') return;
        
        if (usart_mode == 1) {
            // Process one character at a time
            if (!bool_data) {
                received_character = temp_char;
                bool_data = true;
            }
        } else {
            // Store two characters for simultaneous reception
            buffer[uart_rx_index] = temp_char;
            uart_rx_index++;
            if (uart_rx_index == 2) {
                bool_data = true;
                uart_rx_index = 0;
            }
        }
        // Clear UART error flags
        USART3->ICR |= USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_PECF;
    }
}

// Function to receive a character with timeout (for part 1)
bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout) {
    for (uint32_t i = 0; i < timeout; i++) {
        if (bool_data) {
            bool_data = false;  // Clear flag
            *c = received_character;
            return true;
        }
        __NOP();  // Short latency
    }
    return false;  // Timeout occurred
}

// Polling-based character reception
char USART3_ReceiveChar(void) {
    while (!(USART3->ISR & USART_ISR_RXNE))
        ;
    return USART3->RDR;
}

// USART transmit functions
void USART_SendChar(char c) {
    while (!(USART3->ISR & USART_ISR_TXE))
        ;
    USART3->TDR = c;
}

// Send a null-terminated string via USART
void USART_SendString(const char *str) {
    if (!str) return;
    while (*str) {
        USART_SendChar(*str++);
    }
    // Append carriage return and line feed to change the line
    USART_SendChar('\r');
    USART_SendChar('\n');
}

// LED control: Red (PC6) and Blue (PC7)
void Red_toggle(void) { GPIOC->ODR ^= (1 << 6); }
void Blue_toggle(void) { GPIOC->ODR ^= (1 << 7); }

// Initialize GPIO for LEDs (PC6: Red, PC7: Blue)
void GPIO_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // Set PC6 and PC7 as general purpose output (01)
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)));
    GPIOC->MODER |=  ((1 << (6 * 2)) | (1 << (7 * 2)));
    // Initial state: Turn off LEDs (using BSRR to clear pin)
    GPIOC->BSRR = (1 << (6 + 16)) | (1 << (7 + 16));
}

// LED_Process: Process LED control command where the first character is the LED identifier ('r' or 'b')
// and the second character is the command ('0': off, '1': on, '2': toggle)
void LED_Process(char led, char command) {
    uint16_t pin = (led == 'r') ? (1 << 6) : (led == 'b') ? (1 << 7) : 0;
    
    if (pin) {
        if (command == '0') {
            GPIOC->BSRR = pin << 16;  // Turn Off
            USART_SendString((led == 'r') ? "Red LED OFF!" : "Blue LED OFF!");
        } else if (command == '1') {
            GPIOC->BSRR = pin;        // Turn On
            USART_SendString((led == 'r') ? "Red LED ON!" : "Blue LED ON!");
        } else if (command == '2') {
            GPIOC->ODR ^= pin;        // Toggle LED
            USART_SendString((led == 'r') ? "Red LED Toggled!" : "Blue LED Toggled!");
        } else {
            USART_SendString("Error: Invalid command! Use 0, 1, or 2.");
        }
    } else {
        USART_SendString("Error: Use 'r' or 'b' for LED control.");
    }
}

void part1(void) {
    USART3_Init(1);
    GPIO_Init();
    USART_SendString("Commands key for LED operation:");

    while (1) {
        if (bool_data) {
            bool_data = false;  // Reset flag
            char firstCharacter = received_character;
            received_character = 0;         

            char msg[48];
            sprintf(msg, "First received: %c", firstCharacter);
            USART_SendString(msg);

            // If the first input is not an LED control character ('r' or 'b'), output an error message
            if (firstCharacter != 'r' && firstCharacter != 'b') {
                USART_SendString("Error: First input must be 'r' or 'b'.");
                continue;
            }

            // Attempt to receive the second character (command) within 2 seconds
            char secondCharacter;
            bool gotSecond = USART3_ReceiveCharTimeout(&secondCharacter, 2000000);

            // Reset reception-related variables
            received_character = 0;
            bool_data = false;

            // Print debug message: Second character received
            sprintf(msg, "Second received: %c", secondCharacter);
            USART_SendString(msg);

            // If the second character is not received or the command character is not '0' or '1', handle the error
            if (!gotSecond) {
                USART_SendString("Error: second input needed.");
                continue;
            }
            if (secondCharacter != '0' && secondCharacter != '1') {
                USART_SendString("Error: Second must be '0' or '1'.");
                continue;
            }

            // After confirming the input, print a confirmation message and call the LED control function
            sprintf(msg, "Got: %c%c", firstCharacter, secondCharacter);
            USART_SendString(msg);
            LED_Process(firstCharacter, secondCharacter);

            // Finally, clear the reception variables
            received_character = 0;
            bool_data = false;
        }
    }
}

// Part 2: Receive two characters (LED identifier and command) simultaneously via interrupt
void part2(void) {
    // Initialization: set simultaneous reception mode and initialize GPIO
    USART3_Init(2);
    GPIO_Init();
    
    USART_SendString("Enter 2-char LED command:");
    
    while (1) {
        // Check if two characters have been received
        if (bool_data) {
            bool_data = false;  // Reset flag to prevent reprocessing
            
            // Extract LED selection character and command character from buffer
            char ledSelect = buffer[0];
            char ledCmd    = buffer[1];
            
            // Verify LED selection: must be 'r' or 'b'
            if (ledSelect != 'r' && ledSelect != 'b') {
                USART_SendString("Error occurred: Choose among 'r' or 'b'.");
                continue;
            }
            
            // Verify command: must be '0', '1', or '2'
            if (ledCmd != '0' && ledCmd != '1' && ledCmd != '2') {
                USART_SendString("Error occurred: choose among '0', '1', or '2'.");
                continue;
            }
            
            // Print a message to confirm the received input
            char outMsg[40];
            sprintf(outMsg, "System has been received: %c%c", ledSelect, ledCmd);
            USART_SendString(outMsg);
            
            // Perform LED control
            LED_Process(ledSelect, ledCmd);
        }
    }
}
