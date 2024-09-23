#include "adc.h"

void adc_config(void)
{

    rcu_periph_clock_enable(RCU_ADC);

    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
    adc_enable();
    adc_special_function_config(ADC_SCAN_MODE, ENABLE);
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
    adc_regular_channel_config(0, ADC_CHANNEL_1, ADC_SAMPLETIME_239POINT5);
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    adc_resolution_config(ADC_RESOLUTION_12B);
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);

    adc_enable();
    adc_calibration_enable();
}

uint16_t getValue(uint8_t ADC_CHANNEL_x)
{
    adc_regular_channel_config(0, ADC_CHANNEL_x, ADC_SAMPLETIME_239POINT5);
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
    while (adc_flag_get(ADC_FLAG_EOC) == RESET)
        ;
    uint16_t res = adc_regular_data_read();
    return res;
}
