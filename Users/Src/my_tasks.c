#include "main.h"
// #include "portmacro.h"

extern ui_main_channel_t radio_channel;
extern Brightness_setting_t Display_brightness;
extern Display_Timer_t Display_Timer;
extern uint8_t input_state;

bool FREERTOS_ON = FALSE;
SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput;
SemaphoreHandle_t xMainListeningRender, xMainListeningUnrender, xMainChannelDraw;

void vTaskChannelListening(void)
{
    vTaskDelay(1500);
    xSemaphoreTake(xMainListeningRender, portMAX_DELAY);
    for (;;)
    {
        PWR_restart();
        dual_band_standby(&radio_channel, &Display_brightness, &Display_Timer, &input_state);
    }
}
void vTaskUIEvent(void)
{
    for (;;)
    {
        ui_event_cb();
    }
}

void vTaskSpeakingDraw(void)
{
    vTaskDelay(1800);
    for (;;)
    {
        main_channel_listening_draw(&radio_channel,&input_state);
    }
}

void vTaskUIInit(void)
{
    for (;;)
    {
        LCD_LIGHT_HIGH;
        Startup_display();
        FMC_channel_init();
        main_channel_init(&radio_channel);
        Brightness_init(&Display_brightness);
        Display_Timer_Init(&Display_Timer);
        ui_init();
        vTaskDelete(NULL);
    }
}

void xSemaphore_init(void)
{
    xMainChannelTalking = xSemaphoreCreateMutex();
    xMainChannelListening = xSemaphoreCreateMutex();
    xMainChannelInput = xSemaphoreCreateMutex();
    xMainListeningRender = xSemaphoreCreateMutex();
    xMainListeningUnrender = xSemaphoreCreateMutex();
    xMainChannelDraw = xSemaphoreCreateMutex();

    xSemaphoreTake(xMainListeningUnrender, portMAX_DELAY);
}

void vtasks_init(void)
{
    xTaskCreate(vTaskUIInit, "UI Init", 128, NULL, 4, NULL);
    xTaskCreate(vTaskUIEvent, "Event handler", 128, NULL, 3, NULL);
    xTaskCreate(vTaskChannelListening, "Channel Listening", 128, NULL, 3, NULL);
    xTaskCreate(vTaskSpeakingDraw, "UI draw", 128, NULL, 3, NULL);
    xSemaphore_init();
    timer_enable(TIMER1);
    FREERTOS_ON = TRUE;
}
