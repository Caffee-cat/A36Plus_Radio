#include "ui_main.h"

static uint8_t tmp[100];
static bool confirm = 0;
static uint8_t input_num = 0;

uint8_t input_state = 0;
ui_page_t ui_main;
ui_main_channel_t radio_channel;

void ui_main_initial(void)
{
    ui_main.ui_init = ui_main_init;
    ui_main.ui_refresh = ui_main_refresh;
    ui_main.ui_event_cb = ui_main_event_cb;
    ui_main.ui_destory = ui_main_destory;
}

static void draw_main(void)
{
    if (radio_channel.cur_channel == &radio_channel.channel_1)
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 12);
#ifdef USE_FONT_LOADED_IN_MCU
        jgfx_draw_img(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 10, main_icon);
#else
        jgfx_draw_img_byaddr(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 10, FLASH_ICON_MAIN_ADDR);
#endif
        radio_channel.channel = 1;
    }
    else if (radio_channel.cur_channel == &radio_channel.channel_2)
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(DISPLAY_W, DISPLAY_H / 10 + 5, 31, 12);
#ifdef USE_FONT_LOADED_IN_MCU
        jgfx_draw_img(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 10, main_icon);
#else
        jgfx_draw_img_byaddr(DISPLAY_W, DISPLAY_H / 2 + 12, 31, 10, FLASH_ICON_MAIN_ADDR);
#endif
        radio_channel.channel = 0;
    }
}

static void draw_status(void)
{
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

    jgfx_fill_react(32 + 49, DISPLAY_H / 10 + 3, 26, 18);
    jgfx_fill_react(32 + 49, DISPLAY_H / 2 + 11, 26, 18);

    // draw channelA

    if (radio_channel.channel_1.Rx_CTCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 10 + 5, "CT");
    }
    if (radio_channel.channel_1.Rx_CDCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 10 + 5, "DCS");
    }

    if (radio_channel.channel_1.offset)
    {
        if (radio_channel.channel_1.direction == ADDITION)
            jgfx_draw_text_en(32 + 112, DISPLAY_H / 10 + 23, "+");
        else if (radio_channel.channel_1.direction == SUBTRACTION)
            jgfx_draw_text_en(32 + 112, DISPLAY_H / 10 + 23, "-");
    }

    switch (radio_channel.channel_1.power)
    {
    case TXP_HIGH:
        jgfx_draw_text_en(32 + 38, DISPLAY_H / 10 + 5, "H");
        break;
    case TXP_MID:
        jgfx_draw_text_en(32 + 38, DISPLAY_H / 10 + 5, "M");
        break;
    case TXP_LOW:
        jgfx_draw_text_en(32 + 38, DISPLAY_H / 10 + 5, "L");
    default:
        break;
    }

    if (radio_channel.channel_1.channnel_bandwidth == 3)
    {
        jgfx_draw_text_en(32 + 24, DISPLAY_H / 10 + 5, "N");
    }

    // draw channel_B

    if (radio_channel.channel_2.Rx_CTCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 2 + 12, "CT");
    }
    if (radio_channel.channel_2.Rx_CDCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 2 + 12, "DCS");
    }

    if (radio_channel.channel_2.offset)
    {
        if (radio_channel.channel_2.direction == ADDITION)
            jgfx_draw_text_en(32 + 112, DISPLAY_H / 2 + 70, "+");
        else if (radio_channel.channel_2.direction == SUBTRACTION)
            jgfx_draw_text_en(32 + 112, DISPLAY_H / 2 + 70, "-");
    }

    switch (radio_channel.channel_2.power)
    {
    case TXP_HIGH:
        jgfx_draw_text_en(32 + 38, DISPLAY_H / 2 + 12, "H");
        break;
    case TXP_MID:
        jgfx_draw_text_en(32 + 38, DISPLAY_H / 2 + 12, "M");
        break;
    case TXP_LOW:
        jgfx_draw_text_en(32 + 38, DISPLAY_H / 2 + 12, "L");
    default:
        break;
    }

    if (radio_channel.channel_1.channnel_bandwidth == 3)
    {
        jgfx_draw_text_en(32 + 24, DISPLAY_H / 2 + 12, "N");
    }
}

static void draw_status_tx(void)
{
    if (radio_channel.cur_channel->Tx_CDCSS == 0 && radio_channel.cur_channel->Tx_CTCSS == 0)
        return;
    jgfx_fill_react(32 + 49, DISPLAY_H / 10 + 3, 26, 18);
    jgfx_fill_react(32 + 49, DISPLAY_H / 2 + 11, 26, 18);
    // draw channelA

    if (radio_channel.channel_1.Tx_CTCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 10 + 5, "CT");
    }
    if (radio_channel.channel_1.Tx_CDCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 10 + 5, "DCS");
    }

    if (radio_channel.channel_2.Tx_CTCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 2 + 12, "CT");
    }
    if (radio_channel.channel_2.Tx_CDCSS)
    {
        jgfx_draw_text_en(32 + 50, DISPLAY_H / 2 + 12, "DCS");
    }
}

void draw_channel(void)
{
    temp_page = uiStackGetTop(&ui_stack);
    if (temp_page != &ui_main)
        return;

    // store value to draw channel
    uint32_t temp_channel = radio_channel.cur_channel->frequency;
    if (input_state == 1)
        radio_channel.cur_channel->frequency = radio_channel.ch_bak;

    // draw channel A
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

    // Determine the number of digits to draw MHZ differently
    if (radio_channel.channel_1.frequency / (1000 * 1000 * 100) == 1)
    {
        sprintf(tmp, "%04d", radio_channel.channel_1.frequency / (1000 * 100));
        jgfx_draw_text_en(32 + 8 + 23, DISPLAY_H / 10 + 30, tmp);
    }
    else
    {
        sprintf(tmp, "%03d", radio_channel.channel_1.frequency / (1000 * 100));
        jgfx_draw_text_en(32 + 8 + 31, DISPLAY_H / 10 + 30, tmp);
    }

    // draw point
    jgfx_draw_text_en(32 + 41 + 3 * 8, DISPLAY_H / 10 + 30, ".");

    // draw khz
    sprintf(tmp, "%03d", radio_channel.channel_1.frequency % (1000 * 100) / 100);
    jgfx_draw_text_en(32 + 38 + 4 * 8, DISPLAY_H / 10 + 30, tmp);

    // draw hz
    if (input_state == 0)
    {
        sprintf(tmp, "%02d", radio_channel.channel_1.frequency % 100);
        jgfx_draw_text_en(32 + 38 + 7 * 8, DISPLAY_H / 10 + 30, tmp);
    }

    // draw channel B
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    if (radio_channel.channel_2.frequency / (1000 * 1000 * 100) == 1)
    {
        sprintf(tmp, "%04d", radio_channel.channel_2.frequency / (1000 * 100));
        jgfx_draw_text_en(32 + 8 + 23, DISPLAY_H / 2 + 35, tmp);
    }
    else
    {
        sprintf(tmp, "%03d", radio_channel.channel_2.frequency / (1000 * 100));
        jgfx_draw_text_en(32 + 8 + 31, DISPLAY_H / 2 + 35, tmp);
    }

    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_draw_text_en(32 + 41 + 3 * 8, DISPLAY_H / 2 + 35, ".");

    jgfx_set_font(JGFX_FONT_EN_8X16);
    sprintf(tmp, "%03d", radio_channel.channel_2.frequency % (1000 * 100) / 100);
    jgfx_draw_text_en(32 + 38 + 4 * 8, DISPLAY_H / 2 + 35, tmp);

    // draw hz
    if (input_state == 0)
    {
        sprintf(tmp, "%02d", radio_channel.channel_2.frequency % 100);
        jgfx_draw_text_en(32 + 38 + 7 * 8, DISPLAY_H / 2 + 35, tmp);
    }

    radio_channel.cur_channel->frequency = temp_channel;

    draw_status();
}

void draw_battery(void)
{
    uint8_t count = 10;
    uint16_t val = 0;
    while(count--)
    {
        val += getValue(ADC_CHANNEL_1);
        printf("%d\n",val);
    }
    val = val / 10;
    printf("total: %d\n",val);

    if(val>2553)
    {
        jgfx_draw_img(32, 0, 21, 14, battery_full);
    }
    else if(val > 2409)
    {
        jgfx_draw_img(32, 0, 21, 14, battery_level3);
    }
    else if(val > 2280)
    {
        jgfx_draw_img(32, 0, 21, 14, battery_level2);
    }
    else if (val > 2104)
    {
        jgfx_draw_img(32, 0, 21, 14, battery_level1);
    }
    else
    {
        jgfx_draw_img(32, 0, 21, 14, battery_empty);
    }

}

void ui_main_init(void)
{
    ui_main.initial_finished = FALSE;
}

void ui_main_refresh(void)
{

    // clear screen
    jgfx_set_color_hex(JGFXF_COLOR_BLACK);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    jgfx_fill_react(32, 0, DISPLAY_W, DISPLAY_H + 1);

    // draw title
    jgfx_set_color_back_hex(JGFXF_COLOR_GRAY);
    jgfx_fill_react(32, 0, 128, 15);

    // draw header
    draw_battery();

    vTaskDelay(200);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
#ifdef USE_FONT_LOADED_IN_MCU
    jgfx_draw_img(32 + 5, DISPLAY_H / 10 + 5, 10, 10, channel_A);
#else
    // jgfx_draw_img_byaddr(42 + 5, DISPLAY_H / 10 + 30, 10, 10, FLASH_ICON_CHANNELA_ADDR);
    jgfx_draw_text_en(44 + 5, DISPLAY_H / 10 + 30, "A");
#endif

#ifdef USE_FONT_LOADED_IN_MCU
    jgfx_draw_img(32 + 5, DISPLAY_H / 2 + 52, 10, 10, channel_B);
#else
    // jgfx_draw_img_byaddr(42 + 5, DISPLAY_H / 2 + 35, 10, 10, FLASH_ICON_CHANNELB_ADDR);
    jgfx_draw_text_en(44 + 5, DISPLAY_H / 2 + 35, "B");
#endif

    draw_main();
    draw_channel();
    radio_channel.flash_count_num1 = radio_channel.flash_count_num2 = 0;
    radio_channel.block_height1 = DISPLAY_H / 10 + 20;
    radio_channel.block_height2 = DISPLAY_H / 2 + 22;

    ui_main.initial_finished = TRUE;
}

void ui_main_destory(void)
{
}

void ui_main_event_cb(void)
{
    Display_Timer_count(&Display_Timer);
    channel_input_flicker(&radio_channel, input_state);
    draw_battery();

    key_map_t key = key_get();
    if (key == KEY_MAP_NONE)
    {
        channel_store(&radio_channel);
        // dual_band_standby(&radio_channel, &Display_brightness, &Display_Timer, &input_state);
        // printf("1\n");
    }
    if (key != KEY_MAP_NONE)
    {
        wakeup_screen(&Display_brightness, &Display_Timer);
        if (key == KEY_MAP_1)
        {
            // radio_channel.ch_bak > 100 * 1000 * 100,waiting for confirmation
            if (confirm == 1)
            {
                radio_channel.channel_changed = TRUE;
                confirm = 0;
                input_state = 0;
                xSemaphoreGive(xMainChannelInput);
                radio_channel.cur_channel->frequency = radio_channel.ch_bak;
                bk4819_set_freq(radio_channel.cur_channel->frequency);
                draw_channel();
            }
            // radio_channel.ch_bak < 100 * 1000 * 100
            else if (input_state == 1)
            {
                radio_channel.channel_changed = TRUE;
                input_state = 0;
                xSemaphoreGive(xMainChannelInput);
                if (radio_channel.ch_bak == 0)
                    ;
                // radio_channel.cur_channel->frequency = radio_channel.ch_bak;
                else
                {
                    while (!(radio_channel.ch_bak / (100 * 1000 * 100)))
                        radio_channel.ch_bak *= 10;
                    radio_channel.cur_channel->frequency = radio_channel.ch_bak;
                }
                draw_channel();
                bk4819_set_freq(radio_channel.cur_channel->frequency);
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

        else if (key == KEY_MAP_2 && (radio_channel.cur_channel->frequency < 1300 * 1000 * 100))
        {
            // MainChannel_input_interrupt();
            if (radio_channel.channel_listening == TRUE)
                MainChannel_input_interrupt();
            radio_channel.cur_channel->frequency += (int)(radio_channel.step * 100);
            radio_channel.channel_changed = TRUE;
            bk4819_set_freq(radio_channel.cur_channel->frequency);
            draw_channel();
        }
        else if (key == KEY_MAP_3 && (radio_channel.cur_channel->frequency > 100 * 1000 * 100))
        {
            // MainChannel_input_interrupt();
            if (radio_channel.channel_listening == TRUE)
                MainChannel_input_interrupt();
            radio_channel.cur_channel->frequency -= (int)(radio_channel.step * 100);
            radio_channel.channel_changed = TRUE;
            bk4819_set_freq(radio_channel.cur_channel->frequency);
            draw_channel();
        }

        // seems some bugs here
        else if (key == KEY_MAP_8)
        {
            if (input_state == 1)
            {
                input_state = 0;
                confirm = 0;
                main_channel_init(&radio_channel);
                draw_channel();
                xSemaphoreGive(xMainChannelInput);
            }
            if (radio_channel.cur_channel == &radio_channel.channel_1)
            {
                radio_channel.cur_channel = &radio_channel.channel_2;
            }
            else if (radio_channel.cur_channel == &radio_channel.channel_2)
            {
                radio_channel.cur_channel = &radio_channel.channel_1;
            }
            bk4819_set_BandWidth(radio_channel.cur_channel->channnel_bandwidth);
            draw_main();
        }

        //  exit/rollback
        else if (key == 4)
        {
            if (input_state == 1)
            {
                radio_channel.ch_bak /= 10;
                if (radio_channel.ch_bak / 100 == 0)
                {
                    xSemaphoreGive(xMainChannelInput);
                    input_state = 0;
                }
                radio_channel.ch_bak = radio_channel.ch_bak / 100 * 100;
            }
            draw_channel();
        }
        //  PTT press
        else if (key == 18)
        {
            //  while talking,it can't listen,while listening ,it can't talk .Just avoid BK4819's frequency has been changed between two threads
            if (xSemaphoreTake(xMainChannelTalking, portMAX_DELAY) == pdTRUE)
            {
                if (xSemaphoreTake(xMainChannelListening, portMAX_DELAY) == pdTRUE)
                {
                    if (radio_channel.channel_listening == TRUE)
                        ui_main_refresh();

                    xSemaphoreGive(xMainChannelListening);
                    if (input_state == 1)
                        input_state = 0;
                    channel_offset_preload(&radio_channel);
                    draw_channel();
                    uint16_t reg_temp = bk4819_read_reg(BK4819_REG_36);
                    bk4819_write_reg(BK4819_REG_36, 0xefbf);
                    TxAmplifier_enable(radio_channel.cur_channel->power);

                    bk4819_set_freq(radio_channel.cur_channel->frequency);
                    bk4819_tx_on();
                    bk4819_TxCTDCSS_set_auto(&radio_channel);
                    draw_status_tx();
                    main_channel_speaking(&radio_channel);
                    while (key_get() != 0)
                        ;
                    bk4819_tx_off();
                    TxAmplifier_disable();
                    bk4819_write_reg(BK4819_REG_36, reg_temp);
                    channel_offset_unload(&radio_channel);
                    // shit
                    ui_main_refresh();

                    xSemaphoreGive(xMainChannelTalking);
                }
            }
        }

        // channel input
        else if (key != KEY_MAP_16 && key != KEY_MAP_L1 && key != KEY_MAP_L2 && key != KEY_MAP_TOP)
        {
            radio_channel.flash_count_num1 = 30, radio_channel.flash_count_num2 = 1;
            if (input_state == 0)
            {
                MainChannel_input_interrupt();
                jgfx_set_color_back_hex(0x0000);
                jgfx_fill_react(32 + 3, radio_channel.cur_channel == &radio_channel.channel_1 ? (radio_channel.block_height1 + 1) : (radio_channel.block_height2 + 5), 128, 26);
                input_state = 1;
                radio_channel.ch_bak = KEY_GET_NUM(key) * 100;
            }
            else
            {
                radio_channel.ch_bak = radio_channel.ch_bak * 10 + KEY_GET_NUM(key) * 100;
                if (radio_channel.ch_bak >= (200 * 1000 * 100))
                {
                    radio_channel.cur_channel->frequency = radio_channel.ch_bak;
                    xSemaphoreGive(xMainChannelInput);
                    input_state = 0;
                    radio_channel.channel_changed = TRUE;
                    confirm = 0;
                    bk4819_set_freq(radio_channel.cur_channel->frequency);
                }
                else if (radio_channel.ch_bak > 100 * 1000 * 100)
                {
                    // input_state = 0;
                    confirm = 1;
                }
                if (radio_channel.ch_bak > (1300 * 1000 * 100))
                {
                    radio_channel.ch_bak = 0;
                    input_state = 0;
                    xSemaphoreGive(xMainChannelInput);
                    // bk4819_set_freq(radio_channel.cur_channel->frequency);
                }
            }
            draw_channel();
        }

        // ui_refresh();
        vTaskDelay(200);
    }
}
