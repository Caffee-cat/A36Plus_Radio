#include "ui_main.h"

ui_page_t ui_main;
extern ui_stack_t ui_stack;
extern ui_page_t ui_menu;
ui_main_channel_t jgfx_channel;
extern Display_Timer_t Display_Timer;
extern Brightness_setting_t Display_brightness;

uint8_t tmp[100];
uint8_t input_state = 0;

uint8_t input_num = 0;
bool confirm = 0;

void ui_main_initial(void)
{
    ui_main.ui_init = ui_main_init;
    ui_main.ui_refresh = ui_main_refresh;
    ui_main.ui_event_cb = ui_main_event_cb;
    ui_main.ui_destory = ui_main_destory;
}

static void draw_main(void)
{
    if (jgfx_channel.cur_channel == &jgfx_channel.channel_1)
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 12);
#ifdef USE_FONT_LOADED_IN_MCU
        jgfx_draw_img(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 10, main_icon);
#else
        jgfx_draw_img_byaddr(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 10, FLASH_ICON_MAIN_ADDR);
#endif
        jgfx_channel.channel = 1;
    }
    else if (jgfx_channel.cur_channel == &jgfx_channel.channel_2)
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 12);
#ifdef USE_FONT_LOADED_IN_MCU
        jgfx_draw_img(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 10, main_icon);
#else
        jgfx_draw_img_byaddr(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 10, FLASH_ICON_MAIN_ADDR);
#endif
        jgfx_channel.channel = 0;
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
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

    // Determine the number of digits to draw MHZ differently
    if (jgfx_channel.channel_1.frequency / (1000 * 1000) == 1)
    {
        sprintf(tmp, "%04d", jgfx_channel.channel_1.frequency / 1000);
        jgfx_draw_text_en(32 + 8 + 8, DISPLAY_H / 10 + 30, tmp);
    }
    else
    {
        sprintf(tmp, "%03d", jgfx_channel.channel_1.frequency / 1000);
        jgfx_draw_text_en(32 + 8 + 16, DISPLAY_H / 10 + 30, tmp);
    }

    // draw point
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_draw_text_en(32 + 26 + 3 * 8, DISPLAY_H / 10 + 30, ".");

    // draw khz
    jgfx_set_font(JGFX_FONT_EN_8X16);
    sprintf(tmp, "%03d", jgfx_channel.channel_1.frequency % 1000);
    jgfx_draw_text_en(32 + 23 + 4 * 8 , DISPLAY_H / 10 + 30, tmp);

    // draw channel B
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    if (jgfx_channel.channel_2.frequency / (1000 * 1000) == 1)
    {
        sprintf(tmp, "%04d", jgfx_channel.channel_2.frequency / 1000);
        jgfx_draw_text_en(32 + 8 + 8, DISPLAY_H / 2 + 35, tmp);
    }
    else
    {
        sprintf(tmp, "%03d", jgfx_channel.channel_2.frequency / 1000);
        jgfx_draw_text_en(32 + 8 + 16, DISPLAY_H / 2 + 35, tmp);
    }

    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_draw_text_en(32 + 26 + 3 * 8, DISPLAY_H / 2 + 35, ".");

    jgfx_set_font(JGFX_FONT_EN_8X16);
    sprintf(tmp, "%03d", jgfx_channel.channel_2.frequency % 1000);
    jgfx_draw_text_en(32 + 23 + 4 * 8, DISPLAY_H / 2 + 35, tmp);

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

    vTaskDelay(200);

#ifdef USE_FONT_LOADED_IN_MCU
    jgfx_draw_img(32 + 5, DISPLAY_H / 10 + 5, 10, 10, channel_A);
#else
    jgfx_draw_img_byaddr(32 + 5, DISPLAY_H / 10 + 50, 10, 10, FLASH_ICON_CHANNELA_ADDR);
#endif

    // // draw separator
    // jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    // jgfx_draw_line(32, DISPLAY_H / 2 + 10, DISPLAY_W + 32, DISPLAY_W / 2 + 10);

#ifdef USE_FONT_LOADED_IN_MCU
    jgfx_draw_img(32 + 5, DISPLAY_H / 2 + 52, 10, 10, channel_B);
#else
    jgfx_draw_img_byaddr(32 + 5, DISPLAY_H / 2 + 52, 10, 10, FLASH_ICON_CHANNELB_ADDR);
#endif

    draw_main();
    draw_channel();
    jgfx_channel.flash_count_num1 = jgfx_channel.flash_count_num2 = 0;
    jgfx_channel.block_height1 = DISPLAY_H / 10 + 20;
    jgfx_channel.block_height2 = DISPLAY_H / 2 + 22;
}

void ui_main_destory(void)
{
}

void ui_main_event_cb(void)
{
    Display_Timer_count(&Display_Timer);

    // detect whether in the input state and flicker the selected channel number
    if (input_state == 1)
    {
        jgfx_channel.flash_count_num1 -= 1;
        if (jgfx_channel.flash_count_num1 == 0)
        {
            jgfx_channel.flash_count_num1 = 3;
            jgfx_channel.flash_count_num2 = !jgfx_channel.flash_count_num2;
            if (jgfx_channel.flash_count_num2 == 1)
            {
                draw_channel();
            }
            else
            {
                jgfx_set_color_back_hex(0x0000);
                jgfx_fill_react(32 + 3, jgfx_channel.cur_channel == &jgfx_channel.channel_1 ? (jgfx_channel.block_height1 + 1) : (jgfx_channel.block_height2 + 5), 128, 26);
            }
        }
    }

    key_map_t key = key_get();
    if (key == KEY_MAP_NONE)
    {
        main_channel_speak_t cur_chan = channel_detect(&jgfx_channel);
        if (cur_chan)
        {
            wakeup_screen(&Display_brightness, &Display_Timer);
            channel_speaking_draw(&jgfx_channel, cur_chan);
            ui_main_refresh();
        }
    }
    if (key != KEY_MAP_NONE)
    {
        wakeup_screen(&Display_brightness, &Display_Timer);
        if (key == KEY_MAP_1)
        {
            if (confirm == 1)
            {
                confirm = 0;
                input_state = 0;
                draw_channel();
                bk4819_set_freq(jgfx_channel.cur_channel->frequency * 100);
            }
            else if (input_state == 1)
            {
                input_state = 0;
                if (jgfx_channel.cur_channel->frequency == 0)
                    jgfx_channel.cur_channel->frequency = jgfx_channel.ch_bak;
                else
                {
                    while (!(jgfx_channel.cur_channel->frequency / (100 * 1000)))
                        jgfx_channel.cur_channel->frequency *= 10;
                }
                draw_channel();
                bk4819_set_freq(jgfx_channel.cur_channel->frequency * 100);
            }
            else
            {
                uiStackPush(&ui_stack, &ui_menu);
                ui_page_ptr page = uiStackGetTop(&ui_stack);
                if (page != NULL)
                {
                    page->ui_init();
                }
            }
        }

        else if (key == KEY_MAP_2)
        {
            if (jgfx_channel.cur_channel->frequency < 1300000)
                jgfx_channel.cur_channel->frequency += jgfx_channel.step;
            draw_channel();
        }
        else if (key == KEY_MAP_3)
        {
            if (jgfx_channel.cur_channel->frequency > 100000)
                jgfx_channel.cur_channel->frequency -= jgfx_channel.step;
            draw_channel();
        }

        // seems some bugs here
        else if (key == KEY_MAP_8)
        {
            if (input_state == 1)
            {
                jgfx_channel.cur_channel->frequency = jgfx_channel.ch_bak;
                input_state = 0;
                confirm = 0;
                main_channel_init(&jgfx_channel);
                draw_channel();
            }
            if (jgfx_channel.cur_channel == &jgfx_channel.channel_1)
                jgfx_channel.cur_channel = &jgfx_channel.channel_2;
            else if (jgfx_channel.cur_channel == &jgfx_channel.channel_2)
                jgfx_channel.cur_channel = &jgfx_channel.channel_1;
            draw_main();
        }

        //  exit/rollback
        else if (key == 4)
        {
            if (input_state == 1)
            {
                jgfx_channel.cur_channel->frequency /= 10;
                if (jgfx_channel.cur_channel->frequency == 0)
                {
                    input_state = 0;
                    jgfx_channel.cur_channel->frequency = jgfx_channel.ch_bak;
                }
            }
            draw_channel();
        }

        //  PTT press
        else if (key == 18)
        {
            // uint32_t color_timecount = 4096;
            bk4819_set_freq((jgfx_channel.cur_channel->frequency + jgfx_channel.cur_channel->offset) * 100);
            bk4819_tx_on();
            main_channel_speaking(&jgfx_channel);
            while (key_get() != 0)
            {
                //     color_timecount -= 1;
                //     if (color_timecount == 0)
                //     {
                //         color_timecount = 255;
                //     }
            }
            bk4819_tx_off();
            // shit
            ui_main_refresh();
        }

        // channel input
        else if (key != KEY_MAP_16)
        {
            jgfx_channel.flash_count_num1 = 3, jgfx_channel.flash_count_num2 = 1;
            if (input_state == 0)
            {
                jgfx_set_color_back_hex(0x0000);
                jgfx_fill_react(32 + 3, jgfx_channel.cur_channel == &jgfx_channel.channel_1 ? (jgfx_channel.block_height1 + 1) : (jgfx_channel.block_height2 + 5), 128, 26);
                input_state = 1;
                jgfx_channel.ch_bak = jgfx_channel.cur_channel->frequency;
                jgfx_channel.cur_channel->frequency = KEY_GET_NUM(key);
            }
            else
            {
                jgfx_channel.cur_channel->frequency = jgfx_channel.cur_channel->frequency * 10 + KEY_GET_NUM(key);
                if (jgfx_channel.cur_channel->frequency >= (200 * 1000))
                {
                    input_state = 0;
                    confirm = 0;
                    bk4819_set_freq(jgfx_channel.cur_channel->frequency * 100);
                }
                else if (jgfx_channel.cur_channel->frequency > 100 * 1000)
                {
                    // input_state = 0;
                    confirm = 1;
                }
                if (jgfx_channel.cur_channel->frequency > (1300 * 1000))
                {
                    jgfx_channel.cur_channel->frequency = jgfx_channel.ch_bak;
                    input_state = 0;
                    bk4819_set_freq(jgfx_channel.cur_channel->frequency * 100);
                }
            }
            draw_channel();
        }

        // ui_refresh();
        vTaskDelay(200);
    }
}
