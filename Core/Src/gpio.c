#include "gpio.h"

// Written by Jamiexu

static void LCD_gpio_init(void)
{
    gpio_af_set(LCD_GPIO_PORT, GPIO_AF_0, LCD_GPIO_SCK_PIN | LCD_GPIO_SDA_PIN);
    gpio_mode_set(LCD_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LCD_GPIO_SCK_PIN | LCD_GPIO_SDA_PIN);
    gpio_output_options_set(LCD_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_GPIO_SCK_PIN | LCD_GPIO_SDA_PIN);

    gpio_mode_set(LCD_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LCD_GPIO_RST_PIN | LCD_GPIO_CS_PIN | LCD_GPIO_WR_PIN | LCD_GPIO_LIGHT_PIN);
    gpio_output_options_set(LCD_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_GPIO_RST_PIN | LCD_GPIO_CS_PIN | LCD_GPIO_WR_PIN | LCD_GPIO_LIGHT_PIN);
}

static void usart_gpio_init(void)
{
    gpio_af_set(USART_GPIO_PORT, GPIO_AF_1, USART_GPIO_TX_PIN | USART_GPIO_RX_PIN);
    gpio_mode_set(USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_GPIO_TX_PIN | USART_GPIO_RX_PIN);
    gpio_output_options_set(USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_GPIO_TX_PIN | USART_GPIO_RX_PIN);
}

static void flash_gpio_init(void)
{
    gpio_af_set(FLASH_GPIO_PORT, GPIO_AF_0, FLASH_GPIO_SCK_PIN | FLASH_GPIO_DIN_PIN | FLASH_GPIO_DOUT_PIN);
    gpio_mode_set(FLASH_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_GPIO_SCK_PIN | FLASH_GPIO_DIN_PIN | FLASH_GPIO_DOUT_PIN);
    gpio_output_options_set(FLASH_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_GPIO_SCK_PIN | FLASH_GPIO_DIN_PIN | FLASH_GPIO_DOUT_PIN);

    gpio_mode_set(FLASH_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, FLASH_GPIO_CS_PIN);
    gpio_output_options_set(FLASH_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_GPIO_CS_PIN);
}

static void bk4819_gpio_init(void)
{
    gpio_mode_set(BK4819_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, BK4819_GPIO_SCK_PIN | BK4819_GPIO_SDA_PIN);
    gpio_output_options_set(BK4819_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BK4819_GPIO_SCK_PIN | BK4819_GPIO_SDA_PIN);

    gpio_mode_set(BK4819_GPIO_SCN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BK4819_GPIO_SCN_PIN);
    gpio_output_options_set(BK4819_GPIO_SCN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BK4819_GPIO_SCN_PIN);
}

static void bk1080_gpio_init(void)
{
    gpio_mode_set(BK1080_GPIO_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BK1080_GPIO_EN_PIN);
    gpio_output_options_set(BK1080_GPIO_EN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BK1080_GPIO_EN_PIN);

    gpio_mode_set(BK1080_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BK1080_GPIO_SCK_PIN);
    gpio_output_options_set(BK1080_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BK1080_GPIO_SCK_PIN);

    gpio_mode_set(BK1080_GPIO_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BK1080_GPIO_SDA_PIN);
    gpio_output_options_set(BK1080_GPIO_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BK1080_GPIO_SDA_PIN);
}

static void key_gpio_init(void)
{
    gpio_mode_set(KEY_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, KEY_GPIO_ROW0_PIN | KEY_GPIO_ROW1_PIN | KEY_GPIO_ROW2_PIN | KEY_GPIO_ROW3_PIN);
    gpio_output_options_set(KEY_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEY_GPIO_ROW0_PIN | KEY_GPIO_ROW1_PIN | KEY_GPIO_ROW2_PIN | KEY_GPIO_ROW3_PIN);

    gpio_mode_set(KEY_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_GPIO_COL0_PIN | KEY_GPIO_COL1_PIN | KEY_GPIO_COL2_PIN | KEY_GPIO_COL3_PIN);

    gpio_mode_set(KEY_GPIO_PTT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_GPIO_PTT_PIN);
}

static void misc_gpio_init(void)
{
    gpio_mode_set(MIC_EN_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, MIC_EN_GPIO_PIN);
    gpio_output_options_set(MIC_EN_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MIC_EN_GPIO_PIN);
}

static void TxAmplifier_gpio_init(void)
{
    gpio_mode_set(TxAmplifier_VHF_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, TxAmplifier_VHF_PIN);
    gpio_output_options_set(TxAmplifier_VHF_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TxAmplifier_VHF_PIN);
}

static void RxAmplifier_gpio_init(void)
{
    gpio_mode_set(RxAmplifier_VHF_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, RxAmplifier_VHF_PIN);
    gpio_output_options_set(RxAmplifier_VHF_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RxAmplifier_VHF_PIN);

    gpio_mode_set(RxAmplifier_UHF_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, RxAmplifier_UHF_PIN);
    gpio_output_options_set(RxAmplifier_UHF_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RxAmplifier_UHF_PIN);
}

void battery_gpio_init(void)
{
    gpio_mode_set(BATTERY_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BATTERY_GPIO_PIN);
    gpio_output_options_set(BATTERY_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, BATTERY_GPIO_PIN);
}

void led_gpio_init(void)
{
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, R_LED_GPIO_PIN);
    gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, R_LED_GPIO_PIN);

    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, G_LED_GPIO_PIN);
    gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, G_LED_GPIO_PIN);
}


void gpio_config(void)
{
    rcu_periph_clock_enable(LCD_GPIO_RCU);
    rcu_periph_clock_enable(USART_GPIO_RCU);
    rcu_periph_clock_enable(FLASH_GPIO_RCU);
    rcu_periph_clock_enable(BK4819_GPIO_RCU);
    rcu_periph_clock_enable(BK4819_GPIO_SCN_RCU);
    rcu_periph_clock_enable(BK1080_GPIO_RCU);
    rcu_periph_clock_enable(BK1080_GPIO_SDA_EN_RCU);
    rcu_periph_clock_enable(KEY_GPIO_RCU);
    rcu_periph_clock_enable(KEY_GPIO_PTT_RCU);
    rcu_periph_clock_enable(MIC_EN_GPIO_RCU);

#ifdef LOAD_IN_A36PLUS
    rcu_periph_clock_enable(LED_GPIO_RCU);
#endif


    LCD_gpio_init();
    key_gpio_init();
    flash_gpio_init();
    usart_gpio_init();
    bk4819_gpio_init();
    bk1080_gpio_init();
    misc_gpio_init();
    TxAmplifier_gpio_init();
    RxAmplifier_gpio_init();
#ifdef LOAD_IN_A36PLUS
    led_gpio_init();
#endif
}
