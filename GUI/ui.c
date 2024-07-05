#include "ui.h"

ui_stack_t ui_stack;
extern ui_page_t ui_menu;
uint32_t counter = 0;

void ui_init(void)
{
    ui_menu_initial();
    uiStackInit(&ui_stack, UI_STACK_MAX_SIZE);
    uiStackPush(&ui_stack, &ui_menu);

    uiStackGetTop(&ui_stack)->ui_init();
}

void ui_refresh(void)
{
    uiStackGetTop(&ui_stack)->ui_refresh();
}

void ui_event_cb(void)
{
    uiStackGetTop(&ui_stack)->ui_event_cb();
}