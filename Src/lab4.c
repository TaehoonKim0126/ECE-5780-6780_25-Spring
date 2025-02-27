#include "main.h"
#include <stdint.h>
#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include "hal_gpio.h"


void SystemClock_Config(void);


// Globals for received data and its flag
volatile char receivedData;
volatile uint8_t DataFlag = 0;



int lab4_main(void)
{
    HAL_Init();

}



