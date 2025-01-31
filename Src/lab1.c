#include <assert.h>
#include <stm32f0xx.h>

int lab1_main(void)
{
    HAL_Init(); // Reset peripherals, initialize Flash and Systick
    SystemClock_Config(); // Configure system clock

    // Enable GPIOC and GPIOA clocks (for LEDs and button)
    RCC->AHBENR |= (RCC_AHBENR_GPIOCEN); // Enable GPIOC clock
    RCC->AHBENR |= (RCC_AHBENR_GPIOAEN); // Enable GPIOA clock
    assert(RCC->AHBENR & RCC_AHBENR_GPIOCEN);
    assert(RCC->AHBENR & RCC_AHBENR_GPIOAEN);

    // Initialize LED pins (PC6, PC7, PC8, PC9)
    My_HAL_GPIO_Init(GPIOC, 6);
    My_HAL_GPIO_Init(GPIOC, 7);
    My_HAL_GPIO_Init(GPIOC, 8);
    My_HAL_GPIO_Init(GPIOC, 9);

    // Initialize USER button (PA0)
    My_HAL_GPIO_Init(GPIOA, 0);

    // Set initial LED state (PC6 & PC8 ON, PC7 & PC9 OFF)
    My_HAL_GPIO_WritePin(GPIOC, 6, GPIO_PIN_SET);
    My_HAL_GPIO_WritePin(GPIOC, 8, GPIO_PIN_SET);
    My_HAL_GPIO_WritePin(GPIOC, 7, GPIO_PIN_RESET);
    My_HAL_GPIO_WritePin(GPIOC, 9, GPIO_PIN_RESET);

    assert(My_HAL_GPIO_ReadPin(GPIOC, 6) == GPIO_PIN_SET);
    assert(My_HAL_GPIO_ReadPin(GPIOC, 8) == GPIO_PIN_SET);
    assert(My_HAL_GPIO_ReadPin(GPIOC, 7) == GPIO_PIN_RESET);
    assert(My_HAL_GPIO_ReadPin(GPIOC, 9) == GPIO_PIN_RESET);

    // LED & button interaction loop
    while (1)
    {
        HAL_Delay(200); // Wait 200ms

        // Check if button is pressed
        if (My_HAL_GPIO_ReadPin(GPIOA, 0) == GPIO_PIN_SET)
        {
            My_HAL_GPIO_TogglePin(GPIOC, 6);
            My_HAL_GPIO_TogglePin(GPIOC, 7);
            My_HAL_GPIO_TogglePin(GPIOC, 8);
            My_HAL_GPIO_TogglePin(GPIOC, 9);
        }
    }
}
