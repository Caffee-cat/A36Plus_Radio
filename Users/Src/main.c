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
    // get the unit brick

    // ob_unlock();
    // ob_erase();
    // ob_reset();
    // ob_lock();

    systick_config();
    gpio_config();
    nvic_config();
    dma_config();
    spi_config();
    tim_config();
    usart_config();
   
    st7735s_init();
    // gpio_bit_set(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
    
    // usart_flash_run();


    jgfx_init(DISPLAY_W, DISPLAY_W);


    // jgfx_test();

    vtasks_init();
    vTaskStartScheduler();

    // delay_1ms(30000);
    // printf("Running...\r\n");
    // w25q16jv_send_cmd(W25Q16JV_CMD_WRITE_ENABLE);
    // w25q16jv_chip_erase();
    // w25q16jv_sector_erase(0x00);
    // flash_test();
    // w25q16jv_chip_erase();
    // w25q16jv_sector_erase(0x00);

    while (1)
    {
        // flash_dump();
        // delay_1ms(1000);
        // if (++time1_current_ms >= 1000){
        //     printf("1s...\r\n");
        //     time1_current_ms = 0;
        // }

        // if (key_get() != KEY_NONE)
        //     printf("KEY: %d\r\n", key_get());
        // printf("Testing...\r\n");
        // bk1080_test();
        // bk4819_test();
        // // flash_dump();
        // for (uint32_t i = 0; i < 16; i++)
        // {
        //     /* code */
        //     w25q16jv_read_sector(i * 0xFF, flash_data);
        //     for (uint32_t i = 0; i < W25Q16JV_SECTOR_SIZE; i++)
        //     {

        //         printf("%x ", flash_data[i]);
        //         delay_1ms(100);
        //     }
        //     delay_1ms(500);
        // }

        //  st7735s_test();
        //  delay_1ms(1000);
        //  LCD_LIGHT_LOW;
        //  delay_1ms(1000);
        //  LCD_LIGHT_HIGH;
        //  delay_1ms(1000);
    }
}