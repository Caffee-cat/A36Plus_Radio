#include "radio.h"

uint16_t count = 0;
bool render_finish = FALSE; 

/**
 * @brief Dual Band in main interface
 *
 * @param channel_ptr channel pointer
 * @return uint8_t 1: Detect successd,refresh main interface; 0 :Detect failed,doing nothing
 */
main_channel_speak_t channel_detect(ui_main_channel_ptr channel_ptr)
{

    uint16_t count = 10, reg_0c_flag;
    // channel turn
    if (channel_ptr->dual_channel)
    {

        channel_ptr->dual_channel = FALSE;

        bk4819_set_freq(channel_ptr->channel_1.frequency);

        bk4819_rx_on();

        vTaskDelay(800);

        // printf("RSSI equal to :0x%04x\n",bk4819_read_reg(BK4819_REG_67));

        bk4819_subchannel_set_Squelch(channel_ptr->cur_channel->sql);

        // printf("detecting!\n");
        reg_0c_flag = bk4819_read_reg(BK4819_REG_0C);
        // recive info
        if (reg_0c_flag & 0x02)
        {

            // check RxCTC/CDC
            // vTaskDelay(10);
            if (main_channel_CTDCSS_judge(&channel_ptr->channel_1) == CHANNEL_SPEAKING)
                return CHANNAL_A_SPEAKING;
            return CTDCSS_INCORRENT;
        }
    }

    else if (!channel_ptr->dual_channel)
    {

        channel_ptr->dual_channel = TRUE;

        bk4819_set_freq(channel_ptr->channel_2.frequency);

        bk4819_rx_on();

        vTaskDelay(800);

        bk4819_subchannel_set_Squelch(channel_ptr->cur_channel->sql);

        // printf("detecting!\n");
        reg_0c_flag = bk4819_read_reg(BK4819_REG_0C);

        if (bk4819_read_reg(BK4819_REG_0C) & 0x02)
        {

            if (main_channel_CTDCSS_judge(&channel_ptr->channel_2) == CHANNEL_SPEAKING)
                return CHANNEL_B_SPEAKING;
            return CTDCSS_INCORRENT;
        }
    }

    bk4819_set_freq(channel_ptr->cur_channel->frequency);
    return 0;
}


/**
 * @brief Dual-Band standby and draw the main interface when a signal is detected
 *
 * @param channel_ptr channel pointer
 * @param Brightness_ptr brightness pointer
 * @param Timer_ptr timer pointer
 * @param state input state refer from ui_main
 */
void dual_band_standby(ui_main_channel_ptr channel_ptr, Brightness_setting_ptr Brightness_ptr, Display_Timer_ptr Timer_ptr, uint8_t *state)
{
    if (xSemaphoreTake(xMainChannelTalking, portMAX_DELAY) == pdTRUE)
    {   
        xSemaphoreTake(xMainChannelDTMFSending,portMAX_DELAY);
        xSemaphoreGive(xMainChannelDTMFSending);
        xSemaphoreTake(xChannelScan, portMAX_DELAY);
        xSemaphoreGive(xChannelScan);
        xSemaphoreGive(xMainChannelTalking);
        if (xSemaphoreTake(xMainChannelListening, portMAX_DELAY) == pdTRUE)
        {
            printf("Dual watching!\n");
            if (loudspeaker_TurnOff() == TRUE)
            {
                main_channel_speak_t cur_chan = channel_detect(channel_ptr);
                if (cur_chan != NONE_CHANNEL_SPEAKING && cur_chan != CTDCSS_INCORRENT)
                {
                    printf("Dual watch Detected!\n");
#ifndef LOAD_IN_A36PLUS
                    bk4819_Tx_Power(TXP_MID);
#else 
                    bk4819_setTxPower(TXP_MID, channel_ptr->cur_channel->frequency, calData);
#endif
                    wakeup_screen(Brightness_ptr, Timer_ptr);

                    loudspeaker_TurnOn(channel_ptr, cur_chan);
                    xSemaphoreGive(xMainListeningRender);
                }
            }
            xSemaphoreGive(xMainChannelListening);
        }
    }
}

/**
 * @brief Turn on loudspeaker after RSSI meet requirment
 *
 * @param channel_ptr channel pointer
 * @param status the speaking channel
 */
void loudspeaker_TurnOn(ui_main_channel_ptr channel_ptr, main_channel_speak_t status)
{
    uint16_t reg;
#ifdef LOAD_IN_A36PLUS
    G_LED_ON;
#endif

    if (status == NONE_CHANNEL_SPEAKING)
    {
        bk4819_set_freq(channel_ptr->cur_channel->frequency);
        return;
    }
    // turn on sound
    gpio_bit_set(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
    reg = bk4819_read_reg(BK4819_REG_30);
    bk4819_write_reg(BK4819_REG_30, reg | BK4819_REG30_AF_DAC_ENABLE | BK4819_REG30_MIC_ADC_ENABLE);
    // RxAmplifier_enable();

    channel_ptr->channel_listening = TRUE;
}


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
#ifdef LOAD_IN_A36PLUS
        G_LED_OFF;
#endif
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


/**
 * @brief Check if CT/DCSS setting is correct or not
 *
 * @param sub_channel sub_channel pointer
 * @return main_channel_speak_t
 */
static main_channel_speak_t main_channel_CTDCSS_judge(sub_channel_ptr sub_channel)
{
    uint8_t count = 20;
    if (sub_channel->Rx_CTCSS || sub_channel->Rx_CDCSS)
    {
        if (sub_channel->Rx_CTCSS != 0)
        {
            bk4819_CTDCSS_set(0, sub_channel->Rx_CTCSS);
            while (count--)
            {
                if (bk4819_read_reg(BK4819_REG_0C) & 0x400)
                    return CHANNEL_SPEAKING;
            }
            return CTDCSS_INCORRENT;
        }
        if (sub_channel->Rx_CDCSS != 0)
        {
            bk4819_CDCSS_set_v2(sub_channel->Rx_CDCSS);
            while (count--)
            {
                if (bk4819_read_reg(BK4819_REG_0C) & 0x4000)
                    return CHANNEL_SPEAKING;
            }
            return CTDCSS_INCORRENT;
        }
    }
    return CHANNEL_SPEAKING;
}


