#include "main.h"
#include "portmacro.h"

extern void jgfx_test(void);
// void vTaskTestings(void *pvParameters)
// {

//     for (;;)
//     {
//         LCD_LIGHT_LOW;
//         vTaskDelay(1000);
//         LCD_LIGHT_HIGH;
//         vTaskDelay(1000);
//     }
// }

void vTaskUIRefresh(void)
{
    for (;;)
    {
        ui_refresh();
    }
}
void vTaskUIEvent(void)
{
    for (;;)
    {
        ui_event_cb();
    }
}

void vTaskUIInit(void)
{
    for (;;)
    {
        /* code */
        ui_init();
        vTaskDelete(NULL);
    }
}

void vtasks_init(void)
{
    xTaskCreate(vTaskUIInit, "UI Init", 64, NULL, 0, NULL);
    xTaskCreate(vTaskUIEvent, "Event handler", 64, NULL, 0, NULL);
    // xTaskCreate(vTaskUIRefresh, "LCD refresh", 128, NULL, 0, NULL);
    timer_enable(TIMER1);
}