#include "ui_menu.h"
ui_page_t ui_menu;

void ui_menu_initial(void)
{
    ui_menu.ui_init = ui_menu_init;
    ui_menu.ui_refresh = ui_menu_refresh;
    ui_menu.ui_event_cb = ui_menu_event_cb;
    ui_menu.ui_destory = ui_menu_destory;
}

void ui_menu_init(void)
{
    jgfx_set_color_hex(JGFXF_COLOR_BLACK);

    jgfx_fill_react(32, 0, DISPLAY_W, DISPLAY_H);

    jgfx_set_color_hex(JGFXF_COLOR_BLUE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLUE);

    jgfx_fill_react(32, 0, 128, 16);
    
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_draw_text_en(32 + DISPLAY_W / 2, 0, "Menu");
}

void ui_menu_refresh(void)
{
}

void ui_menu_destory(void)
{
}

void ui_menu_event_cb(void)
{
}