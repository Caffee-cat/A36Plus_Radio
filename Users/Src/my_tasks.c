#include "main.h"
#include "my_task.h"
#define QUEUE_LENGTH 3         
#define ITEM_SIZE sizeof(uint8_t) * 20 
// #include "portmacro.h"


bool FREERTOS_ON = FALSE;
SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput, xMainChannelDTMFSending;
SemaphoreHandle_t xMainListeningRender, xMainListeningUnrender, xMainChannelDraw;
SemaphoreHandle_t xChannelScan, jgfx_mutex;
QueueHandle_t xQueue; 

void vTaskSystemStart(void)
{
    PWR_restart();
}

void vTaskChannelListening(void)
{
    vTaskDelay(2000);
    xSemaphoreTake(xMainListeningRender, portMAX_DELAY);
    for (;;)
    {
        dual_band_standby(&radio_channel, &Display_brightness, &Display_Timer, &input_state);
    }
}

void vTaskUIEvent(void)
{
    for (;;)
    {
        PWR_restart();
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
        nvm_readCalibData();
        Brightness_init(&Display_brightness);
        Display_Timer_Init(&Display_Timer);
        ui_init();
        vTaskDelete(NULL);
    }
}

static void xSemaphore_init(void)
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

    xQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);  
    if (xQueue == NULL)  
    {  
        printf("xQueue create error!\n");
        while(1); 
    } 

 


    xSemaphoreTake(xMainListeningUnrender, portMAX_DELAY);
}

void vtasks_init(void)
{
    xSemaphore_init();

    xTaskCreate(vTaskUIInit, "UI Init", 128, NULL, 4, NULL);
    // xTaskCreate(vTaskSystemStart, "Event System", 64, NULL, 3, NULL);
    xTaskCreate(vTaskUIEvent, "Event handler", 150, NULL, 3, NULL);
    xTaskCreate(vTaskSpeakingDraw, "UI draw", 128, NULL, 3, NULL);
    xTaskCreate(vTaskChannelListening, "Channel Listening", 128, NULL, 3, NULL);
    timer_enable(TIMER1);
    FREERTOS_ON = TRUE;
}
