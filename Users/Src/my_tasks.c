#include "main.h"
#include "my_task.h"
// #include "portmacro.h"


bool FREERTOS_ON = FALSE;
SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput, xMainChannelDTMFSending;
SemaphoreHandle_t xMainListeningRender, xMainListeningUnrender, xMainChannelDraw;
SemaphoreHandle_t xChannelScan, jgfx_mutex;

void vTaskSystemStart(void)
{
    PWR_restart();
}

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
        // draw_battery();
        main_channel_listening_draw(&radio_channel, &input_state);
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
        nvm_readCalibData(&calData);
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
    xChannelScan = xSemaphoreCreateMutex();
    jgfx_mutex = xSemaphoreCreateMutex();
    xMainChannelDTMFSending = xSemaphoreCreateMutex();

    xSemaphoreTake(xMainListeningUnrender, portMAX_DELAY);
}

void vtasks_init(void)
{
    xTaskCreate(vTaskUIInit, "UI Init", 128, NULL, 4, NULL);
    // xTaskCreate(vTaskSystemStart, "Event System", 64, NULL, 3, NULL);
    xTaskCreate(vTaskUIEvent, "Event handler", 150, NULL, 3, NULL);
    xTaskCreate(vTaskChannelListening, "Channel Listening", 128, NULL, 3, NULL);
    xTaskCreate(vTaskSpeakingDraw, "UI draw", 128, NULL, 3, NULL);
    xSemaphore_init();
    timer_enable(TIMER1);
    FREERTOS_ON = TRUE;
}
