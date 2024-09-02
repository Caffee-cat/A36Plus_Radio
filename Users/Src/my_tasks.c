#include "main.h"
// #include "portmacro.h"

extern ui_main_channel_t radio_channel;
extern Brightness_setting_t Display_brightness;
extern Display_Timer_t Display_Timer;

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
        PWR_restart();
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
        main_channel_init(&radio_channel);
        Brightness_init(&Display_brightness);
        Display_Timer_Init(&Display_Timer);
        ui_init();
        vTaskDelete(NULL);
    }
}

void vtasks_init(void)
{
    xTaskCreate(vTaskUIInit, "UI Init", 128, NULL, 0, NULL);
    xTaskCreate(vTaskUIEvent, "Event handler", 128, NULL, 0, NULL);
    xTaskCreate(vTaskUIRefresh, "LCD refresh", 128, NULL, 0, NULL);
    timer_enable(TIMER1);
}