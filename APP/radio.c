#include "radio.h"

uint16_t count = 0;
bool render_finish = FALSE; //


/**
 * @brief Check if radio is opening loudspeaker and check Squelch resultoutput.
 *
 * @return true Turn off loudspeaker if resultoutput is FALSE
 * @return false Listening
 */
bool loudspeaker_TurnOff(void)
{
    if (radio_channel.channel_listening == FALSE)
        return TRUE;
    if (Squelch_resultoutput() == TRUE)
        return FALSE;
    if (Squelch_resultoutput() == FALSE)
    {

        gpio_bit_reset(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
        uint16_t reg = bk4819_read_reg(BK4819_REG_30);
        bk4819_write_reg(BK4819_REG_30, reg | ~(BK4819_REG30_AF_DAC_ENABLE | BK4819_REG30_MIC_ADC_ENABLE));
        bk4819_set_freq(radio_channel.cur_channel->frequency * 100);

        radio_channel.channel_listening = FALSE;
#ifndef LOAD_IN_A36PLUS
        bk4819_Tx_Power(TXP_LOW);
#else
        bk4819_setTxPower(TXP_STANDBY, radio_channel.cur_channel->frequency, calData);
#endif
        xSemaphoreTake(xMainListeningRender, portMAX_DELAY);
        
        return TRUE;

    }
}

/**
 * @brief While listening, judge if page is pointing at main interface to render RX.Also make sure when turn into main interface from menu,it can still render
 *
 * @param channel_ptr channel pointer
 * @param state input state
 */
void main_channel_listening_draw(ui_main_channel_ptr channel_ptr, uint8_t *state)
{

    uint16_t count = 1000;
    xSemaphoreTake(jgfx_mutex, portMAX_DELAY);
    xSemaphoreGive(jgfx_mutex);

    if (xSemaphoreTake(xMainListeningRender, portMAX_DELAY) == pdTRUE)
    {
        if (*state == 1)
        {
            *state = 0;
            draw_channel();
            vTaskDelay(20);
        }

        while (1)
        {
            // Render disable temporarily when page not point at main interface
            temp_page = uiStackGetTop(&ui_stack);
            if (temp_page != &ui_main)
                render_finish = FALSE;

            // Render disable when channel nothing detected
            if (channel_ptr->channel_listening == FALSE)
                break;

            // Render disable temporarily when render finished
            if (render_finish == TRUE)
                continue;

            if (temp_page == &ui_main && count-- == 0) // Only render in main interface
            {
                // vTaskDelay(1000);

                while (ui_main.initial_finished == FALSE)
                    ;
                vTaskDelay(500);

                jgfx_set_color_hex(JGFXF_COLOR_BLACK);
                jgfx_set_color_back_hex(JGFXF_COLOR_BLUE);
                if (channel_ptr->dual_channel == FALSE)
                {
                    jgfx_fill_react(32, DISPLAY_H / 2 + 11, DISPLAY_W, DISPLAY_H / 2);
                    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
                    jgfx_draw_text_en(76, DISPLAY_H / 2 + 33, "A  RX");
                }
                else if (channel_ptr->dual_channel == TRUE)
                {
                    jgfx_fill_react(32, 10 + 4, DISPLAY_W, DISPLAY_H / 2 - 4);
                    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
                    jgfx_draw_text_en(76, DISPLAY_H / 2 - 23, "B  RX");
                }
                render_finish = TRUE;
            }
        }
        render_finish = FALSE;
        xSemaphoreGive(xMainChannelDraw);
        xSemaphoreGive(xMainListeningRender);

        vTaskDelay(200);

        temp_page = uiStackGetTop(&ui_stack);
        if (temp_page == &ui_main)
            ui_main_refresh();
    }
}

void MainChannel_input_interrupt(void)
{
    if (radio_channel.channel_listening == TRUE)
    {
        while (render_finish == FALSE)
            ;
        ui_main_refresh();
    }
    count = 1000;
}

static void FM_draw_current_frequency(void)
{
    uint16_t frequency = bk1080_get_cur_frequency();
    uint8_t string[10];

    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_PINK);

    sprintf(string, "%d", frequency);
    if(frequency > 1000)
    {
        //1067'0'
        string[5] = '\0';
        string[4] = string[3];
        string[3] = '.';
    }
    else
    {
        //876'\0'
        string[5] = '\0';
        string[4] = ' ';
        string[3] = string[2];
        string[2] = '.';
    }
    jgfx_draw_text_en(70, DISPLAY_H / 2 + 33, string);
}

void FM_main_init(void)
{
    bool bk1080_power = FALSE;
    uint16_t reg;
    
    if (xSemaphoreTake(xMainChannelTalking, portMAX_DELAY) == pdTRUE)
    {
        bk4819_init();          //Have to do this or bk1080 have no voice and don't know why

        jgfx_set_font(JGFX_FONT_EN_8X16);
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_set_color_back_hex(JGFXF_COLOR_PINK);

        jgfx_fill_react(32, DISPLAY_H / 2 + 11, DISPLAY_W, DISPLAY_H / 2);
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_draw_text_en(50, DISPLAY_H / 2 + 33, "FM ");

        bk1080_power = bk1080_status();

        bk1080_init();

        if(bk1080_power == FALSE)
        {
            bk1080_set_band_spacing(0,1);
            bk1080_set_frequency(898);
        }

        bk1080_enable_loudspeaker();
        FM_draw_current_frequency();

        FM_main_callback();             

        xSemaphoreGive(xMainChannelTalking);
    }
}

static void FM_main_callback(void)
{
    uint8_t key;
    uint16_t frequency;
    seek_tune_complete_cb  cb;
    bool status;

    while(1)
    {
        key = key_get();
        if(key != KEY_MAP_NONE)
        {
            if(key == 4)
            {
                ui_main_refresh();
                break;
            }
            if(key == 1)
            {
                // input
            }
            if(key == 2)
            {
                frequency = bk1080_get_cur_frequency();
                status = bk1080_seek_channel_hw(1, 0, frequency, 2, cb);
                if(status == TRUE)
                {
                    FM_draw_current_frequency();
                }
                else
                {
                    printf("Out of range\n");
                }
                
            }
            if(key == 3)
            {
                frequency = bk1080_get_cur_frequency();
                status = bk1080_seek_channel_hw(1, 1, frequency, 2, cb);
                if(status == TRUE)
                {
                    FM_draw_current_frequency();
                }
            }
            if(key == 16)
            {
                BK1080_Mute();
                ui_main_refresh();
                break;
            }
        }
    }
}


