#include "main.h"
// #include "portmacro.h"

extern ui_main_channel_t radio_channel;
extern Brightness_setting_t Display_brightness;
extern Display_Timer_t Display_Timer;
extern uint8_t input_state;

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
        // dual_band_standby(&radio_channel, &Display_brightness, &Display_Timer, &input_state);
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
        // RxAmplifier_enable();
        LCD_LIGHT_HIGH;
        Startup_display();
        main_channel_init(&radio_channel);
        Brightness_init(&Display_brightness);
        Display_Timer_Init(&Display_Timer);
        ui_init();
        vTaskDelete(NULL);
    }
}

void vtasks_init(void)
{
    xTaskCreate(vTaskUIInit, "UI Init", 128, NULL, 2, NULL);
    xTaskCreate(vTaskUIEvent, "Event handler", 128, NULL, 0, NULL);
    xTaskCreate(vTaskUIRefresh, "LCD refresh", 128, NULL, 0, NULL);
    timer_enable(TIMER1);
}