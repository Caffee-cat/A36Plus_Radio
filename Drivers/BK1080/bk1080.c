/**
 * @file bk1080.c
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
#include "bk1080.h"

static bool BK1080_POWER = FALSE;
static uint16_t last_frequency = 875;

static void i2c_start(void)
{
    BK1080_SDA_HIGH;
    bk1080_delay(1);
    BK1080_SCK_HIGH;
    bk1080_delay(1);
    BK1080_SDA_LOW;
    bk1080_delay(1);
    BK1080_SCK_LOW;
    bk1080_delay(1);
}

static void i2c_stop(void)
{
    BK1080_SCK_LOW;
    bk1080_delay(1);
    BK1080_SDA_LOW;
    bk1080_delay(1);
    BK1080_SCK_HIGH;
    bk1080_delay(1);
    BK1080_SDA_HIGH;
}

static void i2c_write_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            BK1080_SDA_HIGH;
        else
            BK1080_SDA_LOW;
        bk1080_delay(1);
        BK1080_SCK_HIGH;
        bk1080_delay(1);
        BK1080_SCK_LOW;
        bk1080_delay(1);
        data <<= 1;
    }
}

static uint8_t i2c_read_byte(void)
{
    uint8_t data = 0;
    BK1080_SDA_DIR_IN;
    bk1080_delay(5);
    for (uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;
        BK1080_SCK_HIGH;
        bk1080_delay(1);
        data |= BK1080_SDA_READ;
        BK1080_SCK_LOW;
        bk1080_delay(1);
    }
    BK1080_SDA_DIR_OUT;
    bk1080_delay(5);
    return data;
}

static void i2c_send_ack(iic_ack_t ack)
{
    if (ack)
        BK1080_SDA_HIGH;
    else
        BK1080_SDA_LOW;
    bk1080_delay(1);
    BK1080_SCK_HIGH;
    bk1080_delay(1);
    BK1080_SCK_LOW;
    bk1080_delay(1);
}

static iic_ack_t i2c_get_ack(void)
{
    iic_ack_t ack;
    BK1080_SDA_DIR_IN;
    bk1080_delay(5);
    BK1080_SCK_HIGH;
    bk1080_delay(1);
    if (BK1080_SDA_READ)
        ack = I2C_NACK;
    else
        ack = I2C_ACK;
    BK1080_SCK_LOW;
    bk1080_delay(1);
    BK1080_SDA_DIR_OUT;
    bk1080_delay(1);
    return ack;
}


static void bk1080_delay(uint32_t count)
{
    delay_1us(count);
}


void bk1080_enable_loudspeaker(void)
{
    gpio_bit_set(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
}

void bk1080_disable_loudspeaker(void)
{
    gpio_bit_reset(MIC_EN_GPIO_PORT, MIC_EN_GPIO_PIN);
}


void bk1080_write_reg(bk1080_reg_t reg, uint16_t data)
{
    i2c_start();
    i2c_write_byte(BK1080_ADDRESS);
    if (i2c_get_ack() == I2C_ACK)
    {
        i2c_write_byte((reg << 1));
        if (i2c_get_ack() == I2C_ACK)
        {
            i2c_write_byte((data >> 8) & 0xFF);
            if (i2c_get_ack() == I2C_ACK)
            {
                i2c_write_byte(data & 0xFF);
                i2c_send_ack(I2C_NACK);
            }
        }
        // else
        //     printf("ACK2 Not in...\r\n");
    }
    // else
    // {
    //     printf("ACK1 Not in...\r\n");
    // }
    i2c_stop();
}

uint16_t bk1080_read_reg(bk1080_reg_t reg)
{
    uint16_t data = 0;
    i2c_start();
    i2c_write_byte(BK1080_ADDRESS);
    if (i2c_get_ack() == I2C_ACK)
    {
        i2c_write_byte((reg << 1) | 0x01);
        if (i2c_get_ack() == I2C_ACK)
        {
            data |= (i2c_read_byte() << 8);
            i2c_send_ack(I2C_ACK);
            data |= i2c_read_byte();
            i2c_send_ack(I2C_NACK);
        }
        // else
        //     printf("ACK2 Not in...\r\n");
    }
    // else
    // {
    //     printf("ACK1 Not in...\r\n");
    // }
    i2c_stop();
    return data;
}

void bk1080_init(void)
{
    if(BK1080_POWER == TRUE)
        return;
    BK1080_POWER = TRUE;
    
    for (uint8_t i = 0; i < sizeof(BK1080_RegisterTable) / sizeof(uint16_t);
         i++)
    {
        bk1080_write_reg(i, BK1080_RegisterTable[i]);
    }
}

void bk1080_set_frequency(uint16_t freq)
{
    uint16_t channel = bk1080_freq_to_chan(freq);
    bk1080_set_channel(channel);
}

uint16_t bk1080_chan_to_freq(uint16_t channel){
    uint16_t reg5 = bk1080_read_reg(BK1080_REG_05);
    uint16_t band = 0;
    uint16_t spacing = 0;

    if (((reg5 >> 4) & 0x03) == 0x00)
        spacing = 200;
    else if (((reg5 >> 4) & 0x03) == 0x01)
        spacing = 100;
    else if (((reg5 >> 4) & 0x03) == 0x02)
        spacing = 50;
    
     if (((reg5 >> 6) & 0x03) == 0x00)
        band = 875;
    else if (((reg5 >> 6) & 0x03) == 0x01)
        band = 760;
    else if (((reg5 >> 6) & 0x03) == 0x02)
        band = 760;
    else if (((reg5 >> 6) & 0x03) == 0x03)
        band = 640;
    
    return (channel * spacing) / 100  + band;
}

uint16_t bk1080_freq_to_chan(uint16_t freq){
    uint16_t reg5 = bk1080_read_reg(BK1080_REG_05);
    uint16_t band = 0;
    uint16_t spacing = 0;

    if (((reg5 >> 4) & 0x03) == 0x00)
        spacing = 200;
    else if (((reg5 >> 4) & 0x03) == 0x01)
        spacing = 100;
    else if (((reg5 >> 4) & 0x03) == 0x02)
        spacing = 50;
    
     if (((reg5 >> 6) & 0x03) == 0x00)
        band = 875;
    else if (((reg5 >> 6) & 0x03) == 0x01)
        band = 760;
    else if (((reg5 >> 6) & 0x03) == 0x02)
        band = 760;
    else if (((reg5 >> 6) & 0x03) == 0x03)
        band = 640;
    
    return (freq - band) * 100 / spacing;
}

void bk1080_set_channel(uint16_t channel){
    uint16_t reg;

    bk1080_write_reg(BK1080_REG_03, channel);
    delay_1ms(2);
    reg = bk1080_read_reg(BK1080_REG_03);
    reg |= 0x8000;
    bk1080_write_reg(BK1080_REG_03, reg);
}

bool bk1080_seek_channel_hw(uint8_t mode, uint8_t dir, uint16_t startChannel, uint8_t timeout, seek_tune_complete_cb cb){
    uint16_t data;


    bk1080_write_reg(BK1080_REG_03, 0x0000);
    bk1080_delay(1000);
    bk1080_write_reg(BK1080_REG_03, startChannel);

    data = bk1080_read_reg(BK1080_REG_02);
    data &= ~(0x11 << 9);
    data |= (((mode << 1) | dir) << 9) | (0x01 << 8);

    bk1080_write_reg(BK1080_REG_02, data);

    while (timeout-- && !bk1080_get_flag(BK1080_FLAG_STC))
    {
        bk1080_delay(40);
    }

    data = bk1080_read_reg(BK1080_REG_10);
    if(data & 0x2000 == 1)
        return FALSE;
    else
    {
        data = bk1080_read_reg(BK1080_REG_03);
        data |= 0x8000;
        bk1080_write_reg(BK1080_REG_03, data);
        data = bk1080_read_reg(BK1080_REG_03);
        return TRUE;
    }

//    cb(bk1080_get_flag(BK1080_FLAG_SFBL), bk1080_read_reg(BK1080_REG_10) & 0x7F, bk1080_read_reg(BK1080_REG_03) & 0x01FF);
    
}

void bk1080_set_band_spacing(uint8_t band, uint8_t spacing){
    uint16_t data = bk1080_read_reg(BK1080_REG_05);
    data &= ~(0x03 << 4);
    data &= ~(0x03 << 6);
    data |= (spacing << 4);
    data |= (band << 6);
    bk1080_write_reg(BK1080_REG_05, data);
}

uint8_t bk1080_get_flag(bk1080_flag_t flag){
    if (bk1080_read_reg(BK1080_REG_10) & flag)
        return 1;
    else
        return 0;
}

uint16_t bk1080_get_cur_frequency(void)
{
    uint16_t reg;

    delay_1us(2);
    reg = bk1080_read_reg(BK1080_REG_11);
    reg &= 0x03FF;

    return bk1080_chan_to_freq(reg);

}

void BK1080_Mute(void)
{
    uint16_t reg;
    reg = bk1080_read_reg(BK1080_REG_02);
    reg |= 0x0041;
	bk1080_write_reg(BK1080_REG_02, reg);
    bk1080_disable_loudspeaker();
    BK1080_POWER = FALSE;
    last_frequency = bk1080_get_cur_frequency();
}

bool bk1080_status(void)
{
    if(BK1080_POWER == TRUE)
        return TRUE;
    return FALSE;
}