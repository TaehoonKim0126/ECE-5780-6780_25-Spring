#include <assert.h>
#include <stm32f0xx.h>

int lab1_main(void) {

    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config(); //Configure the system clock
    /* This example uses HAL library calls to control
    e GPIOC peripheral. You’ll be redoing this code
    with hardware register access. */ 
    // ✅ 1. Enable GPIOC and GPIOA clocks (for LEDs and button)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
    assert(RCC->AHBENR & RCC_AHBENR_GPIOCEN);
    assert(RCC->AHBENR & RCC_AHBENR_GPIOAEN);

    // ✅ 2. Set PC6 (RED), PC7 (BLUE), PC8 (ORANGE), PC9 (GREEN) as output mode
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); // Clear previous mode
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));  // Set output mode
    assert((GPIOC->MODER & (3 << (6 * 2))) == (1 << (6 * 2)));
    assert((GPIOC->MODER & (3 << (7 * 2))) == (1 << (7 * 2)));
    assert((GPIOC->MODER & (3 << (8 * 2))) == (1 << (8 * 2)));
    assert((GPIOC->MODER & (3 << (9 * 2))) == (1 << (9 * 2)));

    // ✅ 3. Set Push-Pull mode (OTYPER)
    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    assert((GPIOC->OTYPER & ((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9))) == 0);

    // ✅ 4. Set low-speed mode (OSPEEDR)
    GPIOC->OSPEEDR &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    assert((GPIOC->OSPEEDR & ((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)))) == 0);

    // ✅ 5. No pull-up/down resistors (PUPDR)
    GPIOC->PUPDR &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    assert((GPIOC->PUPDR & ((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)))) == 0);

    // ✅ 6. Set PA0 (USER button) as input mode
    GPIOA->MODER &= ~(3 << (0 * 2)); // Set to input mode
    assert((GPIOA->MODER & (3 << (0 * 2))) == 0);

    // ✅ 7. Set PA0 button pull-down resistor (PUPDR)
    GPIOA->PUPDR &= ~(3 << (0 * 2)); // Clear previous settings
    GPIOA->PUPDR |= (2 << (0 * 2));  // Enable pull-down resistor
    assert((GPIOA->PUPDR & (3 << (0 * 2))) == (2 << (0 * 2)));

    // ✅ 8. Set initial LED state (PC6 & PC8 ON, PC7 & PC9 OFF)
    GPIOC->ODR |= (1 << 6) | (1 << 8);  // Turn ON PC6(RED) and PC8(ORANGE)
    GPIOC->ODR &= ~((1 << 7) | (1 << 9)); // Turn OFF PC7(BLUE) and PC9(GREEN)
    assert(GPIOC->ODR & (1 << 6));
    assert(GPIOC->ODR & (1 << 8));
    assert(!(GPIOC->ODR & (1 << 7)));
    assert(!(GPIOC->ODR & (1 << 9)));

    // ✅ 9. LED & button interaction loop
    while (1) {
        HAL_Delay(200); // Wait 200ms

        // Check if button is pressed
        if (GPIOA->IDR & (1 << 0)) { // If PA0 (USER button) is HIGH
            GPIOC->ODR ^= (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9); // Toggle all LEDs
        }
    }
}
