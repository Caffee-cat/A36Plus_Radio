/**
 * @file bk4819.c
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-05-24
 *
 * @copyright MIT License

Copyright (c) 2024 (Jamiexu or Jamie793)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 *
 */

#include "bk4819.h"

typedef enum
{
	CODE_TYPE_OFF = 0x00,
	CODE_TYPE_CONTINUOUS_TONE,
	CODE_TYPE_DIGITAL,
	CODE_TYPE_REVERSE_DIGITAL
}DCS_CodeType_t;

const uint16_t DCS_Options[104] = {
	0x0013, 0x0015, 0x0016, 0x0019, 0x001A, 0x001E, 0x0023, 0x0027,
	0x0029, 0x002B, 0x002C, 0x0035, 0x0039, 0x003A, 0x003B, 0x003C,
	0x004C, 0x004D, 0x004E, 0x0052, 0x0055, 0x0059, 0x005A, 0x005C,
	0x0063, 0x0065, 0x006A, 0x006D, 0x006E, 0x0072, 0x0075, 0x007A,
	0x007C, 0x0085, 0x008A, 0x0093, 0x0095, 0x0096, 0x00A3, 0x00A4,
	0x00A5, 0x00A6, 0x00A9, 0x00AA, 0x00AD, 0x00B1, 0x00B3, 0x00B5,
	0x00B6, 0x00B9, 0x00BC, 0x00C6, 0x00C9, 0x00CD, 0x00D5, 0x00D9,
	0x00DA, 0x00E3, 0x00E6, 0x00E9, 0x00EE, 0x00F4, 0x00F5, 0x00F9,
	0x0109, 0x010A, 0x010B, 0x0113, 0x0119, 0x011A, 0x0125, 0x0126,
	0x012A, 0x012C, 0x012D, 0x0132, 0x0134, 0x0135, 0x0136, 0x0143,
	0x0146, 0x014E, 0x0153, 0x0156, 0x015A, 0x0166, 0x0175, 0x0186,
	0x018A, 0x0194, 0x0197, 0x0199, 0x019A, 0x01AC, 0x01B2, 0x01B4,
	0x01C3, 0x01CA, 0x01D3, 0x01D9, 0x01DA, 0x01DC, 0x01E3, 0x01EC,
};

static uint32_t DCS_CalculateGolay(uint32_t CodeWord)
{
	unsigned int i;
	uint32_t Word = CodeWord;
	for (i = 0; i < 12; i++)
	{
		Word <<= 1;
		if (Word & 0x1000)
			Word ^= 0x08EA;
	}
	return CodeWord | ((Word & 0x0FFE) << 11);
}

uint32_t DCS_GetGolayCodeWord(DCS_CodeType_t CodeType, uint8_t Option)
{
	uint32_t Code = DCS_CalculateGolay(DCS_Options[Option] + 0x800U);
	if (CodeType == CODE_TYPE_REVERSE_DIGITAL)
		Code ^= 0x7FFFFF;
	return Code;
}

static void spi_write_byte(uint8_t data)
{
    BK4819_SCK_LOW;
    BK4819_SDA_DIR_OUT;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            BK4819_SDA_HIGH;
        else
            BK4819_SDA_LOW;
        // bk4819_delay(1);
        BK4819_SCK_HIGH;
        bk4819_delay(1);
        BK4819_SCK_LOW;
        bk4819_delay(1);
        data <<= 1;
    }
}

static void spi_write_half_word(uint16_t data)
{
    spi_write_byte((data >> 8) & 0xFF);
    spi_write_byte(data & 0xFF);
}

static uint16_t spi_read_half_word(void)
{
    uint16_t data = 0;
    BK4819_SDA_DIR_IN;
    BK4819_SCK_LOW;
    for (uint8_t i = 0; i < 16; i++)
    {
        data <<= 1;
        BK4819_SCK_LOW;
        bk4819_delay(1);
        BK4819_SCK_HIGH;
        bk4819_delay(1);
        data |= BK4819_SDA_READ;
    }
    return data;
}

uint16_t bk4819_read_reg(bk4819_reg_t reg)
{
    uint16_t data;
    BK4819_SCN_LOW;
    bk4819_delay(1);

    spi_write_byte(reg | BK4819_REG_READ);
    data = spi_read_half_word();

    bk4819_delay(1);
    BK4819_SCN_HIGH;
    return data;
}

void bk4819_write_reg(bk4819_reg_t reg, uint16_t data)
{
    BK4819_SCN_LOW;
    bk4819_delay(1);

    spi_write_byte(reg | BK4819_REG_WRITE);
    spi_write_half_word(data);

    bk4819_delay(1);
    BK4819_SCN_HIGH;
}

void bk4819_init(void)
{
    uint8_t data;
    bk4819_write_reg(BK4819_REG_00, 0x8000); // reset
    bk4819_delay(1000);
    bk4819_write_reg(BK4819_REG_00, 0x00); // maybe for clear reset bit

    bk4819_write_reg(BK4819_REG_37, 0x1d0f); // DSP voltage setting
    // BK4819_write_reg(BK4819_REG_36, 0x0022);     //less an option for PA setting,may will cause an error
    // BK4819_write_reg(BK4819_REG_7E, 0x302e);     //use default setting here for using auto AGC Fix mode
    bk4819_write_reg(BK4819_REG_13, 0x3be);  // Set RX AGC Gain Table[3] as default setting.<9:8>LNA Gain Short 0db.<7:5>LNA Gain -2dB.<4:3>MIXER Gain -3dB.<2:0>PGA Gain -3dB.
    bk4819_write_reg(BK4819_REG_12, 0x37b);  // Set RX AGC Gain Table[2] as default setting.
    bk4819_write_reg(BK4819_REG_53, 59000);  //****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_09, 0x603a); // DTMF/SelCall Symbol Coefficient for Detection.<15:12>Symbol number=0b110  <7:0>Coefficient = 0b 0011 1010
    bk4819_write_reg(BK4819_REG_11, 0x27b);  // Set RX AGC Gain Table[1]
    bk4819_write_reg(BK4819_REG_10, 0x7a);   // Set RX AGC Gain Table[0]
    bk4819_write_reg(BK4819_REG_14, 0x19);   // Set RX AGC Gain Table[-1] for no use of amModulation with setting BK4819_REG_49.
    bk4819_write_reg(BK4819_REG_49, 0x2a38); // Auto lo selection.Set RF AGC High/Low Threshold.
    bk4819_write_reg(BK4819_REG_7B, 0x8420); // Set RSSI table.****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_48, 0xb3ff); // Set AF RX Gain1/2 and AF DAC Gain
    bk4819_write_reg(BK4819_REG_1E, 0x4c58); //****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_1F, 0xa656); // Set rf PLL
    bk4819_write_reg(BK4819_REG_3E, 0xa037); // Band Selection Threshold.~=VCO Max Frequency(Hz)/96/640
    bk4819_write_reg(BK4819_REG_3F, 0x7fe);  // Set FSK,DTMF/5TONE,CTCSS/CDCSSTail,CDCSS,CTCSS,VoX,Squelch.
    bk4819_write_reg(BK4819_REG_2A, 0x7fff); //****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_28, 0x6b00); // Set Expander (AF Rx).
    bk4819_write_reg(BK4819_REG_7D, 0xe952); // Set MIC Sensitivity Tuning.****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_2C, 0x5705); //****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_4B, 0x7102); // AF Level Controller(ALC) Disable.****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_77, 0x88ef); // Squelch mode select.parameters are choices about RSSI,noise,Glitch combination
    bk4819_write_reg(BK4819_REG_26, 0x13a0); //****Haven't show detail in Rsgisters note.****
    bk4819_write_reg(BK4819_REG_4E, 0x6f15); // Set Squelch and Giltch.
    bk4819_write_reg(BK4819_REG_4F, 0x3f3e); // Set Ex-noise threshold for Squelch
    bk4819_write_reg(BK4819_REG_09, 0x6f);
    bk4819_write_reg(BK4819_REG_09, 0x106b);
    bk4819_write_reg(BK4819_REG_09, 0x2067);
    bk4819_write_reg(BK4819_REG_09, 0x3062);
    bk4819_write_reg(BK4819_REG_09, 0x4050);
    bk4819_write_reg(BK4819_REG_09, 0x5047);
    bk4819_write_reg(BK4819_REG_09, 0x702c);
    bk4819_write_reg(BK4819_REG_09, 0x8041);
    bk4819_write_reg(BK4819_REG_09, 0x9037);
    bk4819_write_reg(BK4819_REG_28, 0x6b38); // Set Expander (AF Rx) back to default setting.
    bk4819_write_reg(BK4819_REG_09, 0xa025);
    bk4819_write_reg(BK4819_REG_09, 0xb017);
    bk4819_write_reg(BK4819_REG_09, 0xc0e4);
    bk4819_write_reg(BK4819_REG_09, 0xd0cb);
    bk4819_write_reg(BK4819_REG_09, 0xe0b5);
    bk4819_write_reg(BK4819_REG_09, 0xf09f);
    bk4819_write_reg(BK4819_REG_74, 0xfa02);                                          // 3000Hz AF Response coefficient for Tx when it's 0xf50b
    bk4819_write_reg(BK4819_REG_44, 0x8f88);                                          // 300Hz AF Response coefficient for Tx when it's 0x9009
    bk4819_write_reg(BK4819_REG_45, 0x3201);                                          // 300Hz AF Response coefficient for Tx. when it's 0x31a9
    bk4819_write_reg(BK4819_REG_29, 0xb4cb);                                          // Set Compress (AF Tx).
    bk4819_write_reg(BK4819_REG_40, bk4819_read_reg(BK4819_REG_40) & 0xf000 | 0x4d2); // RF Tx Deviation Tuning (Apply for both in-band signal andsub-audio signal).
    bk4819_write_reg(BK4819_REG_31, bk4819_read_reg(BK4819_REG_31) & 0xfffffff7);     // disEnable Compander Function.Remain VOX detection,Scramble Function as defalult(dlsable).

    // bk4819_CTDCSS_enable(0);
    // bk4819_CDCSS_set(1,0x19);
    // bk4819_set_freq(43025000);

    // bk4819_CTDCSS_set(0, 719); // Set CTCSS to 71.9HZ
    bk4819_CTDCSS_set(0, 885); // Set CTCSS to 88.5HZ
    bk4819_CTDCSS_disable();
    // bk4819_tx_on();
    // bk4819_tx_off();

    bk4819_rx_off();
    // bk4819_rx_on();
}

static void bk4819_delay(uint32_t count)
{
    delay_1us(count);
}

/**
 * @brief Get interrupt
 *
 * @param interrupt Interrupt type
 * @return uint8_t 0:SET 1:RESET
 */
uint8_t bk4819_int_get(bk4819_int_t interrupt)
{
    return bk4819_read_reg(BK4819_REG_02 & interrupt);
}

/**
 * @brief Set frequency
 *
 * @param freq
 */
void bk4819_set_freq(uint32_t freq)
{
    bk4819_write_reg(BK4819_REG_39, (freq >> 16) & 0xFFFF);
    bk4819_write_reg(BK4819_REG_38, freq & 0xFFFF);
}

/**
 * @brief Rx ON
 *
 */
void bk4819_rx_on(void)
{
    gpio_bit_set(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
    bk4819_write_reg(BK4819_REG_37, 0x1d0f); // DSP voltage setting
    bk4819_write_reg(BK4819_REG_30, 0x00);   // reset

    bk4819_write_reg(BK4819_REG_30,
                     BK4819_REG30_REVERSE2_ENABLE |
                         BK4819_REG30_REVERSE1_ENABLE |
                         BK4819_REG30_VCO_CALIBRATION |
                         BK4819_REG30_RX_LINK_ENABLE |
                         // BK4819_REG30_AF_DAC_ENABLE |
                         BK4819_REG30_PLL_VCO_ENABLE |
                         // BK4819_REG30_MIC_ADC_ENABLE |
                         BK4819_REG30_PA_GAIN_ENABLE |
                         BK4819_REG30_RX_DSP_ENABLE);
}

void bk4819_rx_off(void)
{
    // gpio_bit_set(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
    bk4819_write_reg(BK4819_REG_30, 0);
    // bk4819_write_reg(BK4819_REG_37, 0x1D00);
}

/**
 * @brief Tx ON
 *
 */
void bk4819_tx_on(void)
{
    gpio_bit_reset(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN); // open microphone
    bk4819_write_reg(BK4819_REG_37, 0x1d0f);           // DSP voltage setting

    bk4819_write_reg(BK4819_REG_30, 0x00); // reset

    bk4819_write_reg(BK4819_REG_30,
                     BK4819_REG30_REVERSE1_ENABLE |
                         BK4819_REG30_REVERSE2_ENABLE |
                         BK4819_REG30_VCO_CALIBRATION |
                         BK4819_REG30_MIC_ADC_ENABLE |
                         BK4819_REG30_TX_DSP_ENABLE |
                         BK4819_REG30_PLL_VCO_ENABLE |
                         BK4819_REG30_PA_GAIN_ENABLE);
}

/**
 * @brief Tx OFF
 *
 */
void bk4819_tx_off(void)
{
    gpio_bit_set(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);

    bk4819_write_reg(BK4819_REG_30, 0);
    // bk4819_write_reg(BK4819_REG_37, 0x1D00);
}

/**
 * @brief Set CTCSS/CDCSS
 *
 * @param sel 0:CTC1 1:CTC2 2:CDCSS
 * @param frequency frquency control word, frequency * 10 = TxCTCSS
 */
void bk4819_CTDCSS_set(uint8_t sel, uint16_t frequency)
{
    // bk4819_write_reg(BK4819_REG_07, (sel << 13) | frequency);
    bk4819_write_reg(BK4819_REG_07, sel | (((frequency * 206488u) + 50000u) / 100000u));
}

/**
 * @brief Set squelch threshold
 *
 * @param RTSO RSSI threshold for Squelch=1, 0.5dB/step
 * @param RTSC RSSI threshold for Squelch =0, 0.5dB/step
 * @param ETSO Ex-noise threshold for Squelch =1
 * @param ETSC Ex-noise threshold for Squelch =0
 * @param GTSO Glitch threshold for Squelch =1
 * @param GTSC Glitch threshold for Squelch =0
 */
void bk4819_set_Squelch(uint8_t RTSO, uint8_t RTSC, uint8_t ETSO, uint8_t ETSC, uint8_t GTSO, uint8_t GTSC)
{
    bk4819_write_reg(BK4819_REG_78, (RTSO << 8) | RTSC);
    bk4819_write_reg(BK4819_REG_4F, (ETSC << 8) | ETSO);
    bk4819_write_reg(BK4819_REG_4D, GTSC);
    bk4819_write_reg(BK4819_REG_4E, GTSO);
}

/**
 * @brief Enable CTCSS/CDCSS
 *
 * @param sel 0:CDCSS   1:CTCSS
 */
void bk4819_CTDCSS_enable(uint8_t sel)
{
    uint16_t reg = bk4819_read_reg(BK4819_REG_51);
    reg |= BK4819_REG51_TX_CTCDSS_ENABLE;
    if (sel)
        reg |= BK4819_REG51_CTCSCSS_MODE_SEL;
    else
    {
        reg &= ~BK4819_REG51_CTCSCSS_MODE_SEL;
        reg &= ~BK4819_REG51_TRANSMIT_NEG_CDCSS_CODE;
        reg &= ~BK4819_REG51_CDCSS_BIT_SEL;
    }
    bk4819_write_reg(BK4819_REG_51, reg);
}

void bk4819_CTDCSS_disable(void)
{
    uint16_t reg = bk4819_read_reg(BK4819_REG_51);
    reg &= ~BK4819_REG51_TX_CTCDSS_ENABLE;
    bk4819_write_reg(BK4819_REG_51, reg);
}

/**
 * @brief Set CDCSS code
 *
 * @param sel  0:CDCSS high 12 bits     1:CDCSS low 12 bits
 * @param code CDCSS code
 */
void bk4819_CDCSS_set(uint8_t sel, uint16_t code)
{
    bk4819_write_reg(BK4819_REG_08, (BK4819_REG_08, sel << 15) | code);
}

void bk4819_CDCSS_set_v2(uint32_t code)
{
    bk4819_write_reg(BK4819_REG_08, (0u << 15) | ((code >> 0) & 0x0FFF));  // LS 12-bits
    bk4819_write_reg(BK4819_REG_08, (1u << 15) | ((code >> 12) & 0x0FFF)); // MS 12-bits
}

/**
 * @brief Set Conefficient for detection
 *
 * @param number Symbol number
 * @param coeff Coefficient
 */
void bk4819_DTMF_SELCall_set(uint8_t number, uint8_t coeff)
{
    bk4819_write_reg(BK4819_REG_09, (number << 12) | (coeff));
}
