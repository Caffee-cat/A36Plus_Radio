#ifndef __RADIO_H__
#define __RADIO_H__
#include "main.h"
#include "comp_menu.h"

// extern struct ui_main_channel_t radio_channel;
// extern struct ui_page_t ui_menu, ui_main;
// extern struct ui_stack_t ui_stack;
// extern struct ui_page_ptr temp_page;
// extern struct SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput;

extern ui_page_t ui_menu, ui_main;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
extern ui_main_channel_t radio_channel;
extern SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput;
extern SemaphoreHandle_t xMainListeningRender,xMainListeningUnrender;


bool loudspeaker_TurnOff(void);

void main_channel_listening_draw(ui_main_channel_ptr channel_ptr);

void MainChannel_input_interrupt(void);

#endif  