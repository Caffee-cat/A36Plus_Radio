#include "ui_menu.h"
#define MENU_MAX 3

ui_page_t ui_menu;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;

uint8_t *menu_list[MENU_MAX] = {
    "Common",
    "Tone",
    "Truncking",
};

void ui_menu_initial(void)
{
    ui_menu.ui_init = ui_menu_init;
    ui_menu.ui_refresh = ui_menu_refresh;
    ui_menu.ui_event_cb = ui_menu_event_cb;
    ui_menu.ui_destory = ui_menu_destory;
}

static void draw_menu(void)
{
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_back_hex(JGFXF_COLOR_GRAY);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);

    for (size_t i = 0; i < MENU_MAX; i++)
    {
        // jgfx_fill_react(32, 24 + (i * 20), DISPLAY_W, 20);
        // vTaskDelay(100);
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_draw_text_en(32 + 5, 25 + (i * 20), menu_list[i]);

        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_draw_line(32, 24 + (i * 20) + 20, DISPLAY_W, 24 + (i * 20) + 20);
    }
}

static void draw_slider(void){
    
}

void ui_menu_init(void)
{
    jgfx_set_color_hex(JGFXF_COLOR_BLACK);
    jgfx_fill_react(32, 0, DISPLAY_W, DISPLAY_H);

    jgfx_set_color_hex(JGFXF_COLOR_BLUE);
    jgfx_fill_react(32, 0, 128, 24);

    vTaskDelay(100);
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLUE);
    jgfx_draw_text_en(32 + DISPLAY_W / 2 - jgfx_measure_text_width("Menu") / 2, 5, "Menu");

    jgfx_set_color_hex(JGFXF_COLOR_GRAY);
    jgfx_fill_react(32, 24, DISPLAY_W, DISPLAY_H - 24);

    draw_menu();
}

void ui_menu_refresh(void)
{
}

void ui_menu_destory(void)
{
}

void ui_menu_event_cb(void)
{
    key_map_t key = key_get();
    if (key == KEY_MAP_4)
    {
        uiStackPop(&ui_stack);
        temp_page = uiStackGetTop(&ui_stack);
        if (temp_page != NULL)
            temp_page->ui_refresh();
    }
}