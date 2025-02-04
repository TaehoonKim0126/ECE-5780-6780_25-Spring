#include <stm32f0xx_hal.h>
#include <assert.h>

void SystemClock_Config(void);
void My_HAL_RCC_GPIOC_CLK_ENABLE(void);

int lab1_main(void)
{
    HAL_Init();
    SystemClock_Config();

    //Removce __HAL_
    /// __HAL_RCC_GPIOC_CLK_ENABLE();
     // (2) Replace with our own function
    My_HAL_RCC_GPIOC_CLK_ENABLE();

void My_HAL_RCC_GPIOC_CLK_ENABLE(void)
{
    // Enable clock for GPIOC using bitwise operation
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // Optional: assert to verify it's really set
    assert((RCC->AHBENR & RCC_AHBENR_GPIOCEN) == RCC_AHBENR_GPIOCEN);
}

    assert((RCC->AHBENR & RCC_AHBENR_GPIOCEN) == RCC_AHBENR_GPIOCEN);

    GPIO_InitTypeDef initStr = {
        .Pin   = GPIO_PIN_8 | GPIO_PIN_9,
        .Mode  = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Pull  = GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOC, &initStr);

    uint32_t moder_val      = GPIOC->MODER;
    uint32_t moder_mask     = (3 << (2*8)) | (3 << (2*9));
    uint32_t moder_expected = (1 << (2*8)) | (1 << (2*9));
    assert((moder_val & moder_mask) == moder_expected);

    uint32_t otyper_val = GPIOC->OTYPER;
    uint32_t otyper_mask = (1 << 8) | (1 << 9);
    assert((otyper_val & otyper_mask) == 0);

    uint32_t ospeedr_val      = GPIOC->OSPEEDR;
    uint32_t ospeedr_mask     = (3 << (2*8)) | (3 << (2*9));
    uint32_t ospeedr_expected = 0;
    assert((ospeedr_val & ospeedr_mask) == ospeedr_expected);

    uint32_t pupdr_val      = GPIOC->PUPDR;
    uint32_t pupdr_mask     = (3 << (2*8)) | (3 << (2*9));
    uint32_t pupdr_expected = 0;
    assert((pupdr_val & pupdr_mask) == pupdr_expected);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    assert((GPIOC->ODR & (1U << 8)) != 0);

    while (1)
    {
        HAL_Delay(200);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
    }
}
