/**
 * @file w25q16jv.c
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-05-20
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
#include "w25q16jv.h"

// Written by Jamiexu
// Errors needs to fix


uint8_t spiFlash_SendRecv(uint8_t val)
{
    uint8_t data = 0;
    for(uint8_t i = 0; i < 8; i++)
    {
        gpio_bit_reset(FLASH_GPIO_PORT, FLASH_GPIO_SCK_PIN);
        delay_1us(1);

        if((val << i) & 0x80)
        {
            gpio_bit_set(FLASH_GPIO_PORT, FLASH_GPIO_DOUT_PIN);
        }
        else
        {
            gpio_bit_reset(FLASH_GPIO_PORT, FLASH_GPIO_DOUT_PIN);
        }

        gpio_bit_set(FLASH_GPIO_PORT, FLASH_GPIO_SCK_PIN);
        delay_1us(1);

        data <<= 1;
        data |= (gpio_input_bit_get(FLASH_GPIO_PORT, FLASH_GPIO_DIN_PIN)) ? 0x01 : 0x00;
    }

    return data;
}



static void SPI_send_data(uint8_t data)
{
#ifdef RTOS_ON
    // vTaskDelay(2);
#else
    delay_1us(50);
#endif
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET)
        ;
    spi_i2s_data_transmit(SPI0, data);
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS) != RESET)
        ;
}

static uint8_t SPI_read_data(void)
{
    SPI_send_data(0xFF); // send dummy data to generate sck to receive data
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET)
        ;
    return spi_i2s_data_receive(SPI0);
}

void w25q16jv_send_cmd(w25q16jv_cmd_t cmd)
{
    FLASH_CS_LOW;
    SPI_send_data(cmd);
    FLASH_CS_HIGHT;
}

uint8_t w25q16jv_read_reg1(w25q16jv_reg_t reg)
{
    uint8_t data;
    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_READ_REG1);
    data = SPI_read_data();
    FLASH_CS_HIGHT;
    return ((data & reg) != 0) ? W25Q16JV_SET : W25Q16JV_RESET;
}

uint8_t w25q16jv_read_reg2(w25q16jv_reg_t reg)
{
    uint8_t data;
    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_READ_REG2);
    data = SPI_read_data();
    FLASH_CS_HIGHT;
    return ((data & reg) != 0) ? W25Q16JV_SET : W25Q16JV_RESET;
}

uint8_t w25q16jv_read_reg3(w25q16jv_reg_t reg)
{
    uint8_t data;
    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_READ_REG3);
    data = SPI_read_data();
    FLASH_CS_HIGHT;
    return ((data & reg) != 0) ? W25Q16JV_SET : W25Q16JV_RESET;
}

void w25q16jv_write_reg1(w25q16jv_reg_t reg, uint8_t bit)
{
    uint8_t data;

    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_READ_REG1);
    data = SPI_read_data();
    FLASH_CS_HIGHT;

    if (bit != RESET)
        data |= reg;
    else
        data &= ~reg;

    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_WRITE_REG1);
    SPI_send_data(data);
    FLASH_CS_HIGHT;
}

void w25q16jv_write_reg2(w25q16jv_reg_t reg, uint8_t bit)
{
    uint8_t data;

    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_READ_REG2);
    data = SPI_read_data();
    FLASH_CS_HIGHT;

    if (bit != RESET)
        data |= reg;
    else
        data &= ~reg;

    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_WRITE_REG2);
    SPI_send_data(data);
    FLASH_CS_HIGHT;
}

void w25q16jv_write_reg3(w25q16jv_reg_t reg, uint8_t bit)
{
    uint8_t data;

    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_READ_REG3);
    data = SPI_read_data();
    FLASH_CS_HIGHT;

    if (bit != RESET)
        data |= reg;
    else
        data &= ~reg;

    FLASH_CS_LOW;
    SPI_send_data(W25Q16JV_CMD_WRITE_REG3);
    SPI_send_data(data);
    FLASH_CS_HIGHT;
}

uint8_t w25q16jv_read_busy(void)
{
    return w25q16jv_read_reg1(W25Q16JV_REG1_BUSY);
}

void w25q16jv_read_sector(uint32_t addr, uint8_t *readData)
{

    while (w25q16jv_read_busy() != W25Q16JV_RESET)
    {
    }

    FLASH_CS_LOW;

    SPI_send_data(W25Q16JV_CMD_READ);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    // printf("address sent.\r\n");

    for (uint16_t i = 0; i < W25Q16JV_SECTOR_SIZE; i++)
    {
        readData[i] = SPI_read_data();
        // printf("the number %d data: %d\r\n", i, readData[i]);
    }

    FLASH_CS_HIGHT;
}

void w25q16jv_read_block(uint32_t addr, uint8_t *readData)
{
    for (uint8_t i = 0; i < 16; i++)
        w25q16jv_read_sector(addr + i * W25Q16JV_SECTOR_SIZE, (uint8_t *)readData[i * W25Q16JV_SECTOR_SIZE]);
}

void w25q16jv_read_sector_fast(uint32_t addr, uint8_t *readData)
{

    while (w25q16jv_read_busy() != W25Q16JV_RESET)
    {
    }

    FLASH_CS_LOW;

    SPI_send_data(W25Q16JV_CMD_FAST_READ);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    // printf("address sent.\r\n");

    for (uint8_t i = 0; i < 8; i++)
        SPI_send_data(0xFF); // send dummy data to generate sck to start fast read

    for (uint16_t i = 0; i < W25Q16JV_SECTOR_SIZE; i++)
    {
        readData[i] = SPI_read_data();
        // printf("the number %d data: %d\r\n", i, readData[i]);
    }

    FLASH_CS_HIGHT;
}

void w25q16jv_read_block_fast(uint32_t addr, uint8_t *readData)
{
    for (uint8_t i = 0; i < 16; i++)
        w25q16jv_read_sector_fast(addr + i * W25Q16JV_SECTOR_SIZE, (uint8_t *)readData[i * W25Q16JV_SECTOR_SIZE]);
}

void w25q16jv_read_num(uint32_t addr, uint8_t *readData, uint32_t num)
{
    while (w25q16jv_read_busy() != W25Q16JV_RESET)
    {
    }

    FLASH_CS_LOW;
    spi_i2s_data_receive(SPI0);

    #ifdef W25Q16JV_FIRST_BYTE_READ_ERROR
    addr -= 1;
    #endif

    SPI_send_data(W25Q16JV_CMD_READ);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    delay_1us(10);

    #ifdef W25Q16JV_FIRST_BYTE_READ_ERROR
    SPI_read_data();
    #endif
    
    for (uint16_t i = 0; i < num; i++)
        readData[i] = SPI_read_data();

    FLASH_CS_HIGHT;
}

int W25Qx_readData(uint32_t addr, void* buf, size_t len)
{
    FLASH_CS_LOW;

    spiFlash_SendRecv(0x03);             /* Command        */
    spiFlash_SendRecv((addr >> 16) & 0xFF);  /* Address high   */
    spiFlash_SendRecv((addr >> 8) & 0xFF);   /* Address middle */
    spiFlash_SendRecv(addr & 0xFF);          /* Address low    */

    for(size_t i = 0; i < len; i++)
    {
        ((uint8_t *) buf)[i] = spiFlash_SendRecv(0x00);
    }

    FLASH_CS_HIGHT;

    return 0;
}

void w25q16jv_sector_erase(uint32_t addr)
{
    if ((addr % W25Q16JV_SECTOR_SIZE) != 0)
        return;
    FLASH_CS_LOW;

    // delay_1ms(10);
    SPI_send_data(W25Q16JV_CMD_SECTOR_ERASE);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    FLASH_CS_HIGHT;
}

void w25q16jv_block32_erase(uint32_t addr)
{
    if ((addr % W25Q16JV_BLOCK32_SIZE) != 0)
        return;
    FLASH_CS_LOW;

    delay_1ms(10);
    SPI_send_data(W25Q16JV_CMD_BLOCK32_ERASE);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    FLASH_CS_HIGHT;
}

void w25q16jv_block64_erase(uint32_t addr)
{
    if ((addr % W25Q16JV_BLOCK64_SIZE) != 0)
        return;
    FLASH_CS_LOW;

    delay_1ms(10);
    SPI_send_data(W25Q16JV_CMD_BLOCK64_ERASE);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    FLASH_CS_HIGHT;
}

void w25q16jv_page_program(uint32_t addr, uint8_t *raw_data, uint16_t size)
{
    FLASH_CS_LOW;

    // delay_1ms(10);
    SPI_send_data(W25Q16JV_CMD_PAGE_PROGRAM);
    SPI_send_data((addr >> 16) & 0xFF);
    SPI_send_data((addr >> 8) & 0xFF);
    SPI_send_data(addr & 0xFF);

    for (uint16_t i = 0; i < size; i++)
    {

        SPI_send_data(*raw_data);
        raw_data++;
    }

    FLASH_CS_HIGHT;
}

void w25q16jv_chip_erase(void)
{
    w25q16jv_send_cmd(W25Q16JV_CMD_CHIP_ERASE);
}
