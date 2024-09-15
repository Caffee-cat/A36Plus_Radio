/**
 * @file main.c
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-05-11
 *
 * @copyright MIT License

Copyright (c) 2024 (Jamiexu or Jamie793)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 *
 */
#include "main.h"
#include "string.h"
#include "stdlib.h"

// Written by Jamiexu

extern void vtasks_init(void);

void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char *pcTaskName)
{
    /* Check pcTaskName for the name of the offending task,
     * or pxCurrentTCB if pcTaskName has itself been corrupted. */
    (void)xTask;
    (void)pcTaskName;
}

int main(void)
{
    /** Enable IRQ, it is disabled in boot */
    __enable_irq();

    /** Set NVIC vector table start address and offset*/
    nvic_vector_table_set(0x08001000, 0x00);

    /** Init core peripherals*/
    systick_config();
    gpio_config();
    nvic_config();
    dma_config();
    spi_config();
    tim_config();
    adc1_init();
    bk4819_init();
    RxAmplifier_enable();
    PWR_keep_blocked();

#ifdef USE_USART_AND_FLASH
    usart_config();
    // key press KEY_MAP_L1 and KEY_MAP_L2 to enter flash mode
    // usart_flash_run();
#endif

#ifndef RTOS_ON
    // squelch_test();
    while(1)
    {
        delay_1ms(100);
        uint16_t val=getValue(ADC_CHANNEL_1);
        printf("%d\n",val);
    }

#endif

#ifdef RTOS_ON
    /* Init ST7735S driver*/
    st7735s_init();
    jgfx_init(0, 0);
    vtasks_init();
    vTaskStartScheduler();
#endif

    // flash_test_v2();

    // backlight_terminate();

    while (1)
    {
    }
}