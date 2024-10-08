#ifndef __LED_H__
#define __LED_H__
#include "main.h"

#define R_LED_ON gpio_bit_set(LED_GPIO_PORT, R_LED_GPIO_PIN)
#define R_LED_OFF gpio_bit_reset(LED_GPIO_PORT, R_LED_GPIO_PIN)

#define G_LED_ON gpio_bit_set(LED_GPIO_PORT, G_LED_GPIO_PIN)
#define G_LED_OFF gpio_bit_reset(LED_GPIO_PORT, G_LED_GPIO_PIN)


#endif