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

        uint16_t reg = bk4819_read_reg(BK4819_REG_30);
        bk4819_write_reg(BK4819_REG_30, reg | ~(BK4819_REG30_AF_DAC_ENABLE | BK4819_REG30_MIC_ADC_ENABLE));
        bk4819_set_freq(radio_channel.cur_channel->frequency * 100);

        radio_channel.channel_listening = FALSE;
        bk4819_Tx_Power(TXP_LOW);
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
