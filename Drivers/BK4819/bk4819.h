// Written by Jamiexu
#ifndef __BK4819_JAMIEXU_H__
#define __BK4819_JAMIEXU_H__
#include "main.h"
#include <comp_menu.h>


#define MEM_CAHNNEL_LISTS 64
#define MEM_CAHNNEL_SIZES 8
#define MEM_CHANNEL_LIST_IN_BLOCK 8



typedef struct bk4819_squelch_t *bk4819_squelch_ptr;





extern const uint16_t CTCSS_param[];
extern const float main_channel_step[];
extern const uint16_t DCS_Options[];







// extern uint8_t flash_channel[MEM_CAHNNEL_LISTS][15];




#define BK4819_SCK_LOW gpio_bit_reset(BK4819_GPIO_PORT, BK4819_GPIO_SCK_PIN)
#define BK4819_SCK_HIGH gpio_bit_set(BK4819_GPIO_PORT, BK4819_GPIO_SCK_PIN)

#define BK4819_SDA_LOW gpio_bit_reset(BK4819_GPIO_PORT, BK4819_GPIO_SDA_PIN)
#define BK4819_SDA_HIGH gpio_bit_set(BK4819_GPIO_PORT, BK4819_GPIO_SDA_PIN)

#define BK4819_SCN_LOW gpio_bit_reset(BK4819_GPIO_SCN_PORT, BK4819_GPIO_SCN_PIN)
#define BK4819_SCN_HIGH gpio_bit_set(BK4819_GPIO_SCN_PORT, BK4819_GPIO_SCN_PIN)

#define BK4819_SDA_READ gpio_input_bit_get(BK4819_GPIO_PORT, BK4819_GPIO_SDA_PIN)

#define BK4819_SDA_DIR_OUT gpio_mode_set(BK4819_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BK4819_GPIO_SDA_PIN)
#define BK4819_SDA_DIR_IN gpio_mode_set(BK4819_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BK4819_GPIO_SDA_PIN)

#define BK4819_REG_READ 0x80
#define BK4819_REG_WRITE 0x00

#define BIT(x) (1u << (x))
#define BITV(x, y) ((x) << (y))

#define BK4819_REG30_VCO_CALIBRATION BIT(15)
#define BK4819_REG30_REVERSE1_ENABLE BIT(14)
#define BK4819_REG30_RX_LINK_ENABLE BITV(0x0F, 10)
#define BK4819_REG30_REVERSE2_ENABLE BIT(8)
#define BK4819_REG30_AF_DAC_ENABLE BIT(9)
#define BK4819_REG30_PLL_VCO_ENABLE BITV(0x0F, 4)
#define BK4819_REG30_PA_GAIN_ENABLE BIT(3)
#define BK4819_REG30_MIC_ADC_ENABLE BIT(2)
#define BK4819_REG30_TX_DSP_ENABLE BIT(1)
#define BK4819_REG30_RX_DSP_ENABLE BIT(0)

#define BK4819_REG51_TX_CTCDSS_ENABLE BIT(15)
#define BK4819_REG51_GPIO6_CDCSS BIT(14)
#define BK4819_REG51_TRANSMIT_NEG_CDCSS_CODE BIT(13)
#define BK4819_REG51_CTCSCSS_MODE_SEL BIT(12)
#define BK4819_REG51_CDCSS_BIT_SEL BIT(11)
#define BK4819_REG51_1050HZ_DET_MOD BIT(10)
#define BK4819_REG51_AUTO_CDCSS_BW_MOD BIT(9)
#define BK4819_REG51_AUTO_CTCSS_BW_MOD BIT(8)
#define BK4819_REG51_CTCDCSS_TX_GAIN1T(x) BITV((x), 0)

typedef enum
{
    BK4819_REG_00 = 0x00,
    BK4819_REG_02 = 0x02,
    BK4819_REG_07 = 0x07,
    BK4819_REG_08 = 0x08,
    BK4819_REG_09 = 0x09,
    BK4819_REG_0A = 0x0A,
    BK4819_REG_0B = 0x0B,
    BK4819_REG_0C = 0x0C,

    BK4819_REG_0D = 0x0D,
    BK4819_REG_0E = 0x0E,
    BK4819_REG_10 = 0x10,
    BK4819_REG_11 = 0x11,
    BK4819_REG_12 = 0x12,
    BK4819_REG_13 = 0x13,
    BK4819_REG_14 = 0x14,
    BK4819_REG_18 = 0x18,

    BK4819_REG_19 = 0x19,
    BK4819_REG_1A = 0x1A,
    BK4819_REG_1F = 0x1F,
    BK4819_REG_1E = 0x1E,
    BK4819_REG_21 = 0x21,
    BK4819_REG_24 = 0x24,
    BK4819_REG_26 = 0x26,
    BK4819_REG_28 = 0x28,
    BK4819_REG_29 = 0x29,
    BK4819_REG_2A = 0X2A,
    BK4819_REG_2B = 0x2B,
    BK4819_REG_2C = 0x2c,
    BK4819_REG_2E = 0x2E,
    BK4819_REG_30 = 0x30,
    BK4819_REG_31 = 0x31,
    BK4819_REG_32 = 0x32,
    BK4819_REG_33 = 0x33,
    BK4819_REG_36 = 0x36,
    BK4819_REG_37 = 0x37,
    BK4819_REG_38 = 0x38,

    BK4819_REG_39 = 0x39,
    BK4819_REG_3B = 0x3B,
    BK4819_REG_3C = 0x3C,
    BK4819_REG_3D = 0x3D,
    BK4819_REG_3E = 0x3E,
    BK4819_REG_3F = 0x3F,
    BK4819_REG_40 = 0x40,
    BK4819_REG_43 = 0x43,

    BK4819_REG_44 = 0x44,
    BK4819_REG_45 = 0x45,
    BK4819_REG_46 = 0x46,
    BK4819_REG_47 = 0x47,
    BK4819_REG_48 = 0x48,
    BK4819_REG_49 = 0x49,

    BK4819_REG_4B = 0x4B,
    BK4819_REG_4D = 0x4D,
    BK4819_REG_4E = 0x4E,
    BK4819_REG_4F = 0x4F,
    BK4819_REG_50 = 0x50,
    BK4819_REG_51 = 0x51,
    BK4819_REG_52 = 0x52,
    BK4819_REG_54 = 0x54,
    BK4819_REG_53 = 0x53,
    BK4819_REG_55 = 0x55,
    BK4819_REG_58 = 0x58,
    BK4819_REG_59 = 0x59,
    BK4819_REG_5A = 0x5A,
    BK4819_REG_5B = 0x5B,
    BK4819_REG_5C = 0x5C,
    BK4819_REG_5D = 0x5D,
    BK4819_REG_5E = 0x5E,

    BK4819_REG_5F = 0x5F,
    BK4819_REG_63 = 0x63,
    BK4819_REG_64 = 0x64,
    BK4819_REG_65 = 0x65,
    BK4819_REG_67 = 0x67,
    BK4819_REG_68 = 0x68,
    BK4819_REG_69 = 0x69,
    BK4819_REG_6A = 0x6A,

    BK4819_REG_6F = 0x6F,
    BK4819_REG_70 = 0x70,
    BK4819_REG_71 = 0x71,
    BK4819_REG_72 = 0x72,
    BK4819_REG_73 = 0x73,
    BK4819_REG_74 = 0x74,
    BK4819_REG_75 = 0x75,
    BK4819_REG_77 = 0x77,

    BK4819_REG_78 = 0x78,
    BK4819_REG_79 = 0x79,
    BK4819_REG_7A = 0x7A,
    BK4819_REG_7B = 0x7B,
    BK4819_REG_7C = 0x7C,
    BK4819_REG_7D = 0x7D,
    BK4819_REG_7E = 0x7E,
} bk4819_reg_t;

typedef enum
{
    BK4819_INT_FSKTF = BIT(15),   // FSK TX Finished Interrupt
    BK4819_INT_FSKFFAE = BIT(14), // FSK FIFO Almost Empty interrupt
    BK4819_INT_FSKRXF = BIT(13),  // FSK RX Finished interrupt
    BK4819_INT_FSKFFAF = BIT(12), // FSK FIFO Almost Full interrupt
    BK4819_INT_DTMFTF = BIT(11),  // DTMF/5 TONE Found interrupt
    BK4819_INT_CTDSTF = BIT(10),  // CTCSS/CDCSS Tail Found interrupt
    BK4819_INT_CDCSF = BIT(9),    // CDCSS Found interrupt
    BK4819_INT_CDCSL = BIT(8),    // CDCSS Lost interrupt
    BK4819_INT_CTSSF = BIT(7),    // CTCSS Found interrupt
    BK4819_INT_CTCSL = BIT(6),    // CTCSS Lost interrupt
    BK4819_INT_VOXF = BIT(5),     // VOX Found interrupt
    BK4819_INT_VOXL = BIT(4),     // VOX Lost interrupt
    BK4819_INT_SECF = BIT(3),     // Squelch Found interrupt
    BK4819_INT_SECL = BIT(2),     // Squelch Lost interrupt
    BK4819_INT_FSKRS = BIT(1)     // FSK RX Sync interrupt
} bk4819_int_t;

typedef enum
{
    BK4819_FLAG_DTMF_REV = BITV(0x8, 11), // DTMF/5 Tone code received
    BK4819_FLAG_FSK_RX_SNF = BIT(7),      // FSK RX Sync Negative has been found
    BK4819_FLAG_FSK_RX_SPF = BIT(6),      // FSK RX Sync Positive has been found
    BK4819_FLAG_FSK_RX_CRC = BIT(4),      // FSK RX CRC indicator
    BK4819_FLAG_CDCSS_PCR = BIT(14),      // CDCSS positive code received
    BK4819_FLAG_CDCSS_PNR = BIT(15),      //  CDCSS negative code received
} bk4819_flag_t;


typedef enum
{
    CODE_TYPE_OFF = 0,
    CODE_TYPE_CONTINUOUS_TONE,
    CODE_TYPE_DIGITAL,
    CODE_TYPE_REVERSE_DIGITAL
} DCS_CodeType_t;

typedef enum
{
    BK4819_BW_WIDE = 0x49a8,            // 0 100 100 110 10 1 0 00       
                                        // 3.75  KHZ RF filter bandwidth, no auto fix
                                        // 4.0  kHz AFTxLPF2 filter Band Width
                                        // 25k/20k BW Mode Selection
                                        // 0 dB Gain after FM Demodulation

    BK4819_BW_NARROW = 0x4808,          // 0 100 100 000 00 1 0 00
                                        // 3.75  KHZ RF filter bandwidth, no auto fix
                                        // 3.0  kHz AFTxLPF2 filter Band Width
                                        // 12.5k BW Mode Selection
                                        // 0 dB Gain after FM Demodulation

    BK4819_BW_NARROWER = 0x3658,        // 0 011 011 001 01 1 0 00
                                        // 3.0  KHZ RF filter bandwidth, no auto fix
                                        // 2.5  kHz AFTxLPF2 filter Band Width
                                        // 6.25k BW Mode Selection
                                        // 0 dB Gain after FM Demodulation

}bk4819_bandwidth_t;


typedef enum
{
    BK4819_AF_MUTE = 0u,      //
    BK4819_AF_NORMAL = 1u,    // FM
    BK4819_AF_ALAM = 2u,      //  Tone Out for Rx (Should enable Tone1 first)
    BK4819_AF_BEEP = 3u,      //  Beep Out for Tx (Should enable Tone1 first and set REG_03[9] = 1 to enable AF
    BK4819_AF_BASEBAND1 = 4u, // RAW
    BK4819_AF_BASEBAND2 = 5u, // USB
    BK4819_AF_CTCO = 6u,      // strange LF audio .. maybe the CTCSS LF line ?
    BK4819_AF_AM = 7u,        // AM
    BK4819_AF_FSKO = 8u,      // nothing
    BK4819_AF_UNKNOWN3 = 9u,  // BYP
    BK4819_AF_UNKNOWN4 = 10u, // nothing at all
    BK4819_AF_UNKNOWN5 = 11u, // distorted
    BK4819_AF_UNKNOWN6 = 12u, // distorted
    BK4819_AF_UNKNOWN7 = 13u, // interesting
    BK4819_AF_UNKNOWN8 = 14u, // interesting
    BK4819_AF_UNKNOWN9 = 15u  // not a lot
} BK4819_AF_Type_t;


enum PF_function_t
{
    PF_OFF = 0x00,
    PF_DTMF,
    PF_NOAA,
    PF_FM
};


typedef struct bk4819_squelch_t
{
    uint8_t RTSO; 
    uint8_t RTSC;
    uint8_t ETSO;
    uint8_t ETSC; 
    uint8_t GTSO; 
    uint8_t GTSC;
}bk4819_squelch_t;


typedef struct PowerCalibration {
    uint8_t power_below_130mhz;
    uint8_t power_455_470mhz;
    uint8_t power_420_455mhz;
    uint8_t power_300_420mhz;
    uint8_t power_200_300mhz;
    uint8_t power_166_200mhz;
    uint8_t power_145_166mhz;
    uint8_t power_130_145mhz;
    // uint8_t unused[56]; // Remaining unused bytes in the 64-byte block
} PowerCalibration_t;

// typedef enum
// {

// }

// typedef enum
// {
//     bk4819_CTCSS_PHASE_120 = 0X01,
//     bk4819_CTCSS_PHASE_180 = 0X02,
//     bk4819_CTCSS_PHASE_240 = 0X03,
// } bk4819_CTCSS_PHASE;

// typedef enum
// {
//     bk4819_RX_ACG_GAIN_PAG =
// } bk4819_RX_ACG_GAIN;

static uint32_t DCS_CalculateGolay(uint32_t CodeWord);
static void spi_write_byte(uint8_t data);
static void spi_write_half_word(uint16_t data);
static uint16_t spi_read_half_word(void);
static void bk4819_delay(uint32_t count);
static void channel_ShowParam_add(uint8_t param);
static void channel_ShowParam_delete(uint8_t param);


uint16_t bk4819_read_reg(bk4819_reg_t reg);
void bk4819_write_reg(bk4819_reg_t reg, uint16_t data);

uint8_t bk4819_int_get(bk4819_int_t interrupt);

void bk4819_init(void);

void bk4819_set_freq(uint32_t frq);

void bk4819_rx_on(void);

void bk4819_tx_on(void);

void bk4819_tx_off(void);

void bk4819_rx_off(void);

void bk4819_Squelch_val_change(uint8_t sqlLevel);

void bk4819_CTDCSS_enable(uint8_t sel);

void bk4819_CTDCSS_disable(void);

void bk4819_CTDCSS_set(uint8_t sel, uint16_t frequency);

void bk4819_set_Squelch(uint8_t RTSO, uint8_t RTSC, uint8_t ETSO, uint8_t ETSC, uint8_t GTSO, uint8_t GTSC);

void bk4819_CDCSS_set(uint8_t sel, uint16_t code);

void bk4819_CDCSS_set_v2(uint32_t code);

uint16_t bk4819_RSSI_return(void);

// void bk4819_Tx_Power(Tx_Power_t power);


void TxAmplifier_disable(void);

void RxAmplifier_enable(void);

void RxAmplifier_disable(void);

void BK4819_EnableFrequencyScan(void);

uint32_t FrequencyScanResult(uint8_t *mode, uint32_t *CTDCSS_result);

void BK4819_DisableFrequencyScan(void);

void bk4819_gpio_pin_set(uint8_t pin,bool state);

uint8_t DCS_GetCdcssCode(uint32_t Code);

void bk4819_set_BandWidth(uint8_t param);

void flash_channel_save(uint16_t param, uint32_t channel_frequency);

bool flash_channel_read(uint8_t param, uint32_t *frequency);

void flash_channel_delete(uint16_t param);

void flash_channel_init(void);

void BK4819_AFTxMute(void);

void BK4819AFTxNomal(void);

void BK4819_EnableDTMF(void);

void BK4819_DisableDTMF(void);

void BK4819_PlayDTMF(char Code);

void Send_DTMF_String(uint8_t  *pString);

uint8_t getPaBiasCalValue(uint32_t freq, PowerCalibration_t calTable);

void BK4819_ResetFSK(void);

void FSKReceive_pre(void);

void BK4819_SetupFSK(void);

void BK4819_SendFSKData(uint16_t *pData);

void FSK_Info_TX(void);

void FSK_Info_RX(void);

static uint8_t *FSK_s_info_decode(uint16_t *string);

#endif
