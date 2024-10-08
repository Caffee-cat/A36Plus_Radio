#ifndef __UI_MAIN_JAMIEXU_H__
#define __UI_MAIN_JAMIEXU_H___
#include "ui.h"
#include "DataCalib.h"

#ifdef USE_FONT_LOADED_IN_MCU
extern uint8_t channel_A[];
extern uint8_t channel_B[];
extern uint8_t main_icon[];
#endif


extern uint8_t battery_full[], battery_level3[], battery_level2[], battery_level1[], battery_empty[];

extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
extern ui_page_t ui_menu;
extern Display_Timer_t Display_Timer;
extern Brightness_setting_t Display_brightness;
extern ui_page_t ui_main;
extern SemaphoreHandle_t xMainChannelTalking;
extern SemaphoreHandle_t xMainChannelListening;
extern SemaphoreHandle_t xMainChannelInput;
extern SemaphoreHandle_t xMainChannelDraw;
extern SemaphoreHandle_t jgfx_mutex;
extern BatteryCalibrationTables_t BatCalData;
extern bool render_finish;

void draw_channel(void);

void ui_main_initial(void);

void ui_main_init(void);

void ui_main_refresh(void);

void ui_main_destory(void);

void ui_main_event_cb(void);

#endif