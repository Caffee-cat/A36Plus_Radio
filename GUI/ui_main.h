#ifndef __UI_MAIN_JAMIEXU_H__
#define __UI_MAIN_JAMIEXU_H___
#include "ui.h"

extern uint8_t channel_A[];
extern uint8_t channel_B[];
extern uint8_t main_icon[];

void ui_main_initial(void);

void ui_main_init(void);

void ui_main_refresh(void);

void ui_main_destory(void);

void ui_main_event_cb(void);


#endif