#include "radio.h"

uint16_t count = 0;

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

void main_channel_listening_draw(ui_main_channel_ptr channel_ptr)
{
    bool render_finish = FALSE;
    uint16_t count = 1000;

    if (xSemaphoreTake(xMainListeningRender, portMAX_DELAY) == pdTRUE)
    {


        while (1)
        {
            temp_page = uiStackGetTop(&ui_stack);
            if (temp_page != &ui_main)
                render_finish = FALSE;

            if (channel_ptr->channel_listening == FALSE)
                break;
            if (render_finish == TRUE)
                continue;

            if (temp_page == &ui_main && count-- == 0) // Only render in main interface
            {
                // vTaskDelay(1000);

                while (ui_main.initial_finished == FALSE)
                    ;
                vTaskDelay(500);

                render_finish = TRUE;
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
                    jgfx_fill_react(32, 10, DISPLAY_W, DISPLAY_H / 2);
                    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
                    jgfx_draw_text_en(76, DISPLAY_H / 2 - 23, "B  RX");
                }
            }
        }
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
        ui_main_refresh();
    count = 1000;
}
