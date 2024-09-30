#include "backlight.h"

const uint16_t Display_Timer_param[] = {999, 10, 30, 60, 120};

#ifdef BRIGHTNESS_MENU_SHOW_WITH_PARAM
const uint32_t brightness_param[] = {5, 10, 15, 20,//20
                                    30, 40, 50, 60,//40
                                    80, 100,115,130,//60
                                    145,160,185,200,//80
                                    275,350,425,500
                                    };  
#else 
    const uint32_t brightness_param[] = {5, 20, 100, 200, 500};
#endif
Display_Timer_t Display_Timer;


void TIMER16_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER16, TIMER_INT_UP) != RESET)
    {
        Display_Timer.Timer_count += 1;
        timer_flag_clear(TIMER16, TIMER_INT_UP);
    }
}

void backlight_init()
{
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct time_ocpar;
    rcu_periph_clock_enable(RCU_TIMER16);
    rcu_periph_clock_enable(LCD_GPIO_RCU);

    gpio_af_set(LCD_GPIO_PORT, GPIO_AF_2, LCD_GPIO_LIGHT_PIN);
    gpio_mode_set(LCD_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LCD_GPIO_LIGHT_PIN);
    gpio_output_options_set(LCD_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_GPIO_LIGHT_PIN);

    timer_deinit(TIMER16);
    /* TIMER16 configuration */
    timer_initpara.prescaler = (5000 - 1); // 50M / 5000 = 10k
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = (250 - 1); // 10k / 250 = 400
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 1;
    timer_init(TIMER16, &timer_initpara);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER16);

    /* CH0 configuration in PWM mode */
    time_ocpar.outputstate = TIMER_CCX_ENABLE;
    time_ocpar.outputnstate = TIMER_CCXN_ENABLE;
    time_ocpar.ocpolarity = TIMER_OC_POLARITY_LOW;
    time_ocpar.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    time_ocpar.ocidlestate = TIMER_OC_IDLE_STATE_HIGH;
    time_ocpar.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER16, TIMER_CH_0, &time_ocpar);
    timer_primary_output_config(TIMER16, ENABLE);

    // Set the brightness primarily
    timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, 200);
    timer_channel_output_mode_config(TIMER16, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER16, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    // timer_auto_reload_shadow_enable(TIMER16);

    timer_enable(TIMER16);
    timer_interrupt_enable(TIMER16, TIMER_INT_UP);
}

void backlight_terminate()
{
    rcu_periph_clock_disable(RCU_TIMER16);
}