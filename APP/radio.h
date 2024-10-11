#ifndef __RADIO_H__
#define __RADIO_H__
#include "main.h"
#include "comp_menu.h"
#include "DataCalib.h"


extern ui_page_t ui_menu, ui_main;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
extern ui_main_channel_t radio_channel;
extern SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput, xMainChannelDTMFSending;
extern SemaphoreHandle_t xMainListeningRender,xMainListeningUnrender;
extern SemaphoreHandle_t xMainChannelDraw, xChannelScan;
extern QueueHandle_t xQueue; 


void main_channel_init(ui_main_channel_ptr channel_ptr);

main_channel_speak_t channel_detect(ui_main_channel_ptr channel_ptr);

void dual_band_standby(ui_main_channel_ptr channel_ptr, Brightness_setting_ptr Brightness_ptr, Display_Timer_ptr Timer_ptr, uint8_t *state);

void loudspeaker_TurnOn(ui_main_channel_ptr channel_ptr, main_channel_speak_t status);

bool loudspeaker_TurnOff(void);

void main_channel_listening_draw(ui_main_channel_ptr channel_ptr,uint8_t *state);

void MainChannel_input_interrupt(void);

void FM_main_init(void);

void channel_ANI_change(uint8_t param);

static void FM_main_callback(void);

static void sub_channel_init(sub_channel_ptr sub_ch);

static main_channel_speak_t main_channel_CTDCSS_judge(sub_channel_ptr sub_channel);


#endif  