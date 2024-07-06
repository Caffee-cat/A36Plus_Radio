#include "ui_main.h"

ui_page_t ui_main;
extern ui_stack_t ui_stack;
extern ui_page_t ui_menu;

void ui_main_initial(void)
{
    ui_main.ui_init = ui_main_init;
    ui_main.ui_refresh = ui_main_refresh;
    ui_main.ui_event_cb = ui_main_event_cb;
    ui_main.ui_destory = ui_main_destory;
}

void ui_main_init(void)
{
    jgfx_set_color_hex(JGFXF_COLOR_BLACK);
    jgfx_set_color_back_hex(JGFXF_COLOR_GRAY);

    jgfx_fill_react(32, 0, DISPLAY_W, DISPLAY_H);

    jgfx_set_color_hex(JGFXF_COLOR_GRAY);
    jgfx_fill_react(32, 0, 128, 16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);

    jgfx_draw_text_en(32 + DISPLAY_W / 2 + 5, 0, "Jamiexu");

    jgfx_set_font(JGFX_FONT_EN_16X32);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    jgfx_draw_text_en(32 + 10, DISPLAY_H / 2 + 20, "439.460");
}

void ui_main_refresh(void)
{
}

void ui_main_destory(void)
{
}

void ui_main_event_cb(void)
{
    key_map_t key = key_get();
    if (key != KEY_MAP_NONE)
    {
        uiStackPush(&ui_stack, &ui_menu);
        ui_page_ptr page = uiStackGetTop(&ui_stack);
        if (page != NULL)
        {
            page->ui_init();
        }
    }
}
