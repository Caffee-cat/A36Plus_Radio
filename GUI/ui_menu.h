#ifndef __UI_MENU_JAMIEXU_H__
#define __UI_MENU_JAMIEXU_H__
#include "ui.h"

#define MENU_MAX 3

// extern ui_stack_t ui_stack;
// extern ui_page_ptr temp_page;
// extern ui_main_channel_t radio_channel;
// extern Display_Timer_t Display_Timer;




void ui_menu_initial(void);

void ui_menu_init(void);

void ui_menu_refresh(void);

void ui_menu_destory(void);

void ui_menu_event_cb(void);

static void Step_callback(void);

static void Brightness_callback_v1(void);

static void Brightness_callback_v2(void);

// void create_channel_callback();

static void all_channel_callback(void);

static void Timer_callback();

#endif