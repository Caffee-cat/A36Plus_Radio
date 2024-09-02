#include "pwr_det.h"


//Reset the MCU when close the knob
void PWR_restart(void)
{
    if (gpio_input_bit_get(POWER_GPIO_PORT, POWER_GPIO_PIN) == SET)
    {
        __set_FAULTMASK(1);
        NVIC_SystemReset();
    }
}

//Remain in loop until turn on the knob 
void PWR_keep_blocked(void)
{
    while (gpio_input_bit_get(POWER_GPIO_PORT, POWER_GPIO_PIN) == SET)
        ;
}