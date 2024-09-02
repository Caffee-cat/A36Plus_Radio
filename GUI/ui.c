#include "ui.h"

extern ui_page_t ui_menu, ui_main;
ui_stack_t ui_stack;
ui_page_ptr temp_page;
uint32_t counter = 0;

void ui_init(void)
{

    // splash
    // jgfx_draw_img_byaddr(32, 0, 128, 128, FLASH_SPLASH_ADDR);

    LCD_LIGHT_HIGH;

    backlight_init();
    vTaskDelay(30);

    Startup_display();
    ui_main_initial();
    ui_menu_initial();
    uiStackInit(&ui_stack, UI_STACK_MAX_SIZE);
    uiStackPush(&ui_stack, &ui_main);
    uiStackPush(&ui_stack, &ui_menu);

    temp_page = uiStackGetTop(&ui_stack);
    if (temp_page == NULL)
        return;
    temp_page->ui_init();
    temp_page->ui_refresh();
}

void ui_refresh(void)
{
    uiStackGetTop(&ui_stack)->ui_refresh();
}

void ui_event_cb(void)
{
    temp_page = uiStackGetTop(&ui_stack);
    if (temp_page != NULL)
        temp_page->ui_event_cb();
}
