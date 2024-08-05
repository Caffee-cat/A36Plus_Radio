#include "ui_main.h"

ui_page_t ui_main;
extern ui_stack_t ui_stack;
extern ui_page_t ui_menu;

uint8_t tmp[100];
uint8_t input_state = 0;

uint32_t ch1 = 437067;
uint32_t ch2 = 439460;
uint32_t ch_bak = 0;
uint32_t *cur_ch = &ch1;
uint8_t step = 1;

void ui_main_initial(void)
{
    ui_main.ui_init = ui_main_init;
    ui_main.ui_refresh = ui_main_refresh;
    ui_main.ui_event_cb = ui_main_event_cb;
    ui_main.ui_destory = ui_main_destory;
}

static void draw_main(void)
{
    if (cur_ch == &ch1)
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 12);
        jgfx_draw_img_byaddr(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 10, FLASH_ICON_MAIN_ADDR);
    }
    else if (cur_ch == &ch2)
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 12);
        jgfx_draw_img_byaddr(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 10, FLASH_ICON_MAIN_ADDR);
    }
}

static void draw_satus(void)
{
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    jgfx_draw_text_en(32 + 20, DISPLAY_H / 2 + 30 - 18, "DCSL");
}

static void draw_channel(void)
{
    // draw channel A
    jgfx_set_font(JGFX_FONT_EN_16X32);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    sprintf(tmp, "%03d.%03d", ch1 / 1000, ch1 % 1000);
    jgfx_draw_text_en(32 + 5, DISPLAY_H / 10 + 20, tmp);

    // draw channel B
    jgfx_set_font(JGFX_FONT_EN_16X32);
    sprintf(tmp, "%03d.%03d", ch2 / 1000, ch2 % 1000);
    jgfx_draw_text_en(32 + 5, DISPLAY_H / 2 + 22, tmp);

    draw_satus();
}

void ui_main_init(void)
{
}

void ui_main_refresh(void)
{
    // clear screen
    jgfx_set_color_hex(JGFXF_COLOR_BLACK);
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(32, 0, DISPLAY_W, DISPLAY_H + 1);

    // draw title
    jgfx_set_color_hex(JGFXF_COLOR_GRAY);
    jgfx_fill_react(32, 0, 128, 16);

    vTaskDelay(100);

    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_GRAY);
    jgfx_draw_text_en(32 + DISPLAY_W / 2 + 5, 0, "Jamiexu");

    vTaskDelay(100);
    jgfx_draw_img_byaddr(32 + 5, DISPLAY_H / 10 + 50, 10, 10, FLASH_ICON_CHANNELA_ADDR);

    // draw separator
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_draw_line(32, DISPLAY_H / 2 + 10, DISPLAY_W + 32, DISPLAY_W / 2 + 10);

    jgfx_draw_img_byaddr(32 + 5, DISPLAY_H / 2 + 52, 10, 10, FLASH_ICON_CHANNELB_ADDR);

    draw_main();
    draw_channel();
}

void ui_main_destory(void)
{
}

void ui_main_event_cb(void)
{
    key_map_t key = key_get();
    if (key != KEY_MAP_NONE)
    {
        if (key == KEY_MAP_1)
        {
            uiStackPush(&ui_stack, &ui_menu);
            ui_page_ptr page = uiStackGetTop(&ui_stack);
            if (page != NULL)
            {
                page->ui_init();
            }
        }
        else if (key == KEY_MAP_2)
        {
            if (*cur_ch < 999999)
                *cur_ch += step;
            draw_channel();
        }
        else if (key == KEY_MAP_3)
        {
            if (*cur_ch > 100000)
                *cur_ch -= step;
            draw_channel();
        }
        else if (key == KEY_MAP_8)
        {
            if (cur_ch == &ch1)
                cur_ch = &ch2;
            else if (cur_ch == &ch2)
                cur_ch = &ch1;
            draw_main();
        }
        else if (key == 4)
        {
            if (input_state == 1)
            {
                *cur_ch /= 10;
                if (*cur_ch == 0)
                {
                    input_state = 0;
                    *cur_ch = ch_bak;
                }
            }
            draw_channel();
        }
        else if (key != KEY_MAP_16)
        {
            if (input_state == 0)
            {
                input_state = 1;
                ch_bak = *cur_ch;
                *cur_ch = KEY_GET_NUM(key);
            }
            else
            {
                *cur_ch = *cur_ch * 10 + KEY_GET_NUM(key);
                if (*cur_ch > 100000)
                    input_state = 0;
            }
            draw_channel();
        }

        // ui_refresh();
        vTaskDelay(200);
    }
}
