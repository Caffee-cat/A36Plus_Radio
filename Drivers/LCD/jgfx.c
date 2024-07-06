/**
 * @file jgfx.c
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-25
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

#include "jgfx.h"

static jgfx_color_format color_format = COLOR_FORMAT_RGB565;
// static _color_rgb444 color_rgb444;
static jgfx_color_rgb565 color_rgb565_front;
static jgfx_color_rgb565 color_rgb565_back = {
    .full = 0x00};

// static _color_rgb666 color_rgb666;

static jgfx_t *jgfx;
static uint8_t font_data[128];

void jgfx_init(void)
{
    jgfx = (jgfx_t *)malloc(sizeof(jgfx_t));
    jgfx->draw_buf.buf1 = (jgfx_color_rgb565 *)malloc(BUFFER_SIZE);
    jgfx->draw_buf.buf2 = (jgfx_color_rgb565 *)malloc(BUFFER_SIZE);
    jgfx->draw_buf.buf1_size = BUFFER_SIZE;
    jgfx->draw_buf.buf2_size = BUFFER_SIZE;
    jgfx->draw_buf.buf_point = 0;
    jgfx->draw_buf.buf_act = jgfx->draw_buf.buf1;
    jgfx->flushing = 0;

    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_pixel_format(COLOR_FORMAT_RGB565);
}

// void jgfx_set_buff_size(uint32_t buf1_size, uint32_t buf2_size)
// {
//     if (buf1_size == 0 && buf2_size == 0)
//         return;
//     if (buf1_size != 0)
//     {
//         &jgfx.draw_buf.buf1 =
//     }
// }

void jgfx_set_pixel_format(jgfx_color_format x)
{
    color_format = x;
    st7735s_send_command(ST7735S_CMD_COLMOD);
    st7735s_send_data(x);
}

static void jgfx_update_area(uint16_t x, uint16_t y)
{
    if (x >= DISPLAY_W || x < 0 || y >= DISPLAY_H || y < 0)
        return;

    if (x > jgfx->area.xe)
        jgfx->area.xe = x;
    if (x < jgfx->area.xs)
        jgfx->area.xs = x;

    if (y > jgfx->area.ye)
        jgfx->area.ye = y;
    if (y < jgfx->area.ys)
        jgfx->area.ys = y;
}

void jgfx_send_color(void)
{
    if (color_format == COLOR_FORMAT_RGB666)
    {
        // st7735s_send_data(color_rgb666.ch.r << 2);
        // st7735s_send_data(color_rgb666.ch.g << 2);
        // st7735s_send_data(color_rgb666.ch.b << 2);
    }
    else if (color_format == COLOR_FORMAT_RGB565)
    {
        st7735s_send_data((color_rgb565_front.ch.r << 5) | (color_rgb565_front.ch.g & 0x38));
        st7735s_send_data(((color_rgb565_front.ch.g & 0x07) << 5) | color_rgb565_front.ch.b << 2);
    }
    else if (color_format == COLOR_FORMAT_RGB444)
    {
        // st7735s_send_data((color_rgb444.ch.r << 4) | (color_rgb444.ch.g));
        // st7735s_send_data(color_rgb444.ch.b << 4);
    }
}

void jgfx_draw_pixel(uint8_t x, uint8_t y)
{
    st7735s_set_window(x, x + 1, y, y + 1);
    jgfx_send_color();
}

void jgfx_fill_react(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    // jgfx_reset_area();
    // jgfx_update_area(x, y);
    // jgfx_update_area(x + width - 1, y + height - 1);
    // st7735s_set_window(jgfx->area.xs, jgfx->area.xe, jgfx->area.ys, jgfx->area.ye);
    st7735s_set_window(x, x + width - 1, y, y + height - 1);
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
            (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full = color_rgb565_front.full;
            jgfx->draw_buf.buf_point++;

            if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf1)
            {
                if (jgfx->draw_buf.buf_point >= jgfx->draw_buf.buf1_size)
                    jgfx_flush();
            }
            else if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf2)
            {
                if (jgfx->draw_buf.buf_point >= jgfx->draw_buf.buf2_size)
                    jgfx_flush();
            }
        }
    }
}

void jgfx_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    // Ensure x1 is less than x2 and y1 is less than y2
    if (x1 >= x2 || y1 >= y2)
    {
        return; // Invalid rectangle
    }

    // Draw top and bottom edges
    for (uint16_t x = x1; x <= x2; x++)
    {
        jgfx_draw_pixel(x, y1);
        jgfx_draw_pixel(x, y2);
    }

    // Draw left and right edges (excluding corners to avoid redundancy)
    for (uint16_t y = y1 + 1; y < y2; y++)
    {
        jgfx_draw_pixel(x1, y);
        jgfx_draw_pixel(x2, y);
    }
}

void jgfx_draw_img(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *img)
{
    st7735s_set_window(x, x + w - 1, y, y + h - 1);
    uint16_t cur = 0;
    while (cur < w * h * 2)
    {
        st7735s_send_data(*img);
        img++;
        st7735s_send_data(*img);
        img++;

        cur += 2;
        // break;

        // (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full = ((*img) << 8);
        // (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full |= (*img);
        // jgfx->draw_buf.buf_point++;

        // if (jgfx->draw_buf.buf_act == jgfx->draw_	buf.buf1)
        // {
        //     if (jgfx->draw_buf.buf_point >= jgfx->draw_buf.buf1_size)
        //         jgfx_flush();
        // }
        // else if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf2)
        // {
        //     if (jgfx->draw_buf.buf_point >= jgfx->draw_buf.buf2_size)
        //         jgfx_flush();
        // }
    }
}

void jgfx_draw_img_byaddr(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t addr)
{
    uint16_t temp;
    uint32_t i = 0;
    uint32_t byte_num = w * h * 2;
    st7735s_set_window(x, x + w - 1, y, y + h - 1);
    while (i < byte_num)
    {
        w25q16jv_read_num(addr + i, font_data, 128);
        if (i + 128 < byte_num)
            i += 128;
        else
            i += (byte_num - i);

        for (uint32_t j = 0; j < 128; j += 2)
        {
            // temp =
            st7735s_send_data(*(uint8_t *)(font_data + j));
            st7735s_send_data(*(uint8_t *)(font_data + j + 1));
            // (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full = (*(uint8_t *)(font_data + j) << 8);
            // (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full |= (*(uint8_t *)(font_data + j + 1));
            // jgfx->draw_buf.buf_point++;

            // if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf1)
            // {
            //     if (jgfx->draw_buf.buf_point >= jgfx->draw_buf.buf1_size)
            //         jgfx_flush();
            // }
            // else if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf2)
            // {
            //     if (jgfx->draw_buf.buf_point >= jgfx->draw_buf.buf2_size)
            //         jgfx_flush();
            // }
        }
    }
}

void jgfx_set_font(jgfx_font_sel_t font)
{
    if (font == JGFX_FONT_EN_8X16)
    {
        jgfx->font.addr = FLASH_FONT_EN_8X16_ADDR;
        jgfx->font.width = 8;
        jgfx->font.height = 16;
        jgfx->font.size = 16;
        jgfx->font.index = JGFX_FONT_START_INDEX_EN;
    }
    else if (font == JGFX_FONT_EN_8X16_BLOD)
    {
        jgfx->font.addr = FLASH_FONT_EN_8X16_BLOD_ADDR;
        jgfx->font.width = 8;
        jgfx->font.height = 16;
        jgfx->font.size = 16;
        jgfx->font.index = JGFX_FONT_START_INDEX_EN;
    }
    else if (font == JGFX_FONT_EN_16X32)
    {
        jgfx->font.addr = FLASH_FONT_EN_16x32_ADDR;
        jgfx->font.width = 16;
        jgfx->font.height = 32;
        jgfx->font.size = 64;
        jgfx->font.index = JGFX_FONT_START_INDEX_EN;
    }
    else if (font == JGFX_FONT_CN_16X16)
    {
        jgfx->font.addr = FLASH_FONT_CN_16X16_ADDR;
        jgfx->font.width = 16;
        jgfx->font.height = 16;
        jgfx->font.size = 32;
        jgfx->font.index = JGFX_FONT_START_INDEX_CN;
    }
}

void jgfx_draw_text(uint16_t x, uint16_t y, uint8_t *str)
{
    // NEG CODE, MSb(bit), ROW
    uint8_t font_width = jgfx->font.width;
    uint8_t font_height = jgfx->font.height;
    int16_t offset;
    uint8_t tmp;
    uint8_t j = 0;
    uint8_t k = 0;
    while (*str != '\0')
    {

        st7735s_set_window(x, x + font_width - 1, y, y + font_height - 1);

        offset = *str - jgfx->font.index;
        w25q16jv_read_num(jgfx->font.addr + offset * jgfx->font.size, font_data, jgfx->font.size);

        for (uint16_t i = 0; i < font_height * font_width; i++)
        {
            if (i % 8 == 0)
            {
                tmp = font_data[j++];
                k = 0;
            }

            if (((tmp << k++) & JGFX_FONT_HIGH_MASK) == JGFX_FONT_HIGH_MASK)
            {
                st7735s_send_data((color_rgb565_front.ch.r << 5) | (color_rgb565_front.ch.g & 0x38));
                st7735s_send_data(((color_rgb565_front.ch.g & 0x07) << 5) | color_rgb565_front.ch.b << 2);
            }
            else
            {
                st7735s_send_data((color_rgb565_back.ch.r << 5) | (color_rgb565_back.ch.g & 0x38));
                st7735s_send_data(((color_rgb565_back.ch.g & 0x07) << 5) | color_rgb565_back.ch.b << 2);
            }
        }

        j = 0;
        x += font_width;
        if (x > DISPLAY_W - font_width)
            y += font_height;

        str++;
    }
}

void jgfx_draw_text_cn(uint16_t x, uint16_t y, uint8_t *str)
{
    // NEG CODE, MSb(bit), ROW
    uint8_t font_width = jgfx->font.width;
    uint8_t font_height = jgfx->font.height;
    int16_t offset;
    uint8_t temp;
    uint8_t j = 0;
    uint8_t k = 0;
    while (*str != '\0')
    {
        st7735s_set_window(x, x + font_width - 1, y, y + font_height - 1);

        w25q16jv_read_num(jgfx->font.addr + JGFX_GET_CN_FONT_ADDR(*(uint16_t *)str), font_data, jgfx->font.size);

        for (uint16_t i = 0; i < font_height * font_width; i++)
        {
            if (i % 8 == 0)
            {
                temp = font_data[j++];
                k = 0;
            }

            if (((temp << k++) & JGFX_FONT_HIGH_MASK) == JGFX_FONT_HIGH_MASK)
            {
                st7735s_send_data((color_rgb565_front.ch.r << 5) | (color_rgb565_front.ch.g & 0x38));
                st7735s_send_data(((color_rgb565_front.ch.g & 0x07) << 5) | color_rgb565_front.ch.b << 2);
            }
            else
            {
                st7735s_send_data((color_rgb565_back.ch.r << 5) | (color_rgb565_back.ch.g & 0x38));
                st7735s_send_data(((color_rgb565_back.ch.g & 0x07) << 5) | color_rgb565_back.ch.b << 2);
            }
        }

        j = 0;
        x += font_width;
        if (x > DISPLAY_W - font_width)
            y += font_height;

        str += 2;
    }
}

void jgfx_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        jgfx_draw_pixel(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = err * 2;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void jgfx_draw_circle(uint16_t x, uint16_t y, uint16_t radius)
{
    int x2 = 0;
    int y2 = radius;
    int d = 3 - 2 * radius;

    while (x2 <= y2)
    {
        jgfx_draw_pixel(x + x2, y + y2);
        jgfx_draw_pixel(x - x2, y + y2);
        jgfx_draw_pixel(x + x2, y - y2);
        jgfx_draw_pixel(x - x2, y - y2);
        jgfx_draw_pixel(x + y2, y + x2);
        jgfx_draw_pixel(x - y2, y + x2);
        jgfx_draw_pixel(x + y2, y - x2);
        jgfx_draw_pixel(x - y2, y - x2);

        if (d < 0)
        {
            d = d + 4 * x2 + 6;
        }
        else
        {
            d = d + 4 * (x2 - y2) + 10;
            y2--;
        }
        x2++;
    }
}

static void jgfx_reset_area(void)
{
    jgfx->area.xs = DISPLAY_W - 1;
    jgfx->area.xe = 0;
    jgfx->area.ys = DISPLAY_H - 1;
    jgfx->area.ye = 0;
}

void lcd_flush_finish_cb(void)
{
    LCD_CS_HIGH;
    spi_dma_disable(SPI1, SPI_DMA_TRANSMIT);
    dma_channel_disable(DMA_CH4);
    jgfx->flushing = 0;
}

void jgfx_flush()
{
    LCD_CS_LOW;
    LCD_DC_HIGH;

    while (jgfx->flushing)
        ;

    jgfx->flushing = 1;

    DMA_CHMADDR(DMA_CH4) = (uint32_t)jgfx->draw_buf.buf_act;
    DMA_CHCNT(DMA_CH4) = jgfx->draw_buf.buf_point * 2;
    jgfx->draw_buf.buf_point = 0;

    if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf1)
        jgfx->draw_buf.buf_act = jgfx->draw_buf.buf2;
    else if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf2)
        jgfx->draw_buf.buf_act = jgfx->draw_buf.buf1;

    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    dma_channel_enable(DMA_CH4);
}

void jgfx_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    if (color_format == COLOR_FORMAT_RGB666)
    {
        // color_rgb666.ch.r = red;
        // color_rgb666.ch.g = green;
        // color_rgb666.ch.b = blue;
    }
    else if (color_format == COLOR_FORMAT_RGB565)
    {
        color_rgb565_front.ch.r = red;
        color_rgb565_front.ch.g = green;
        color_rgb565_front.ch.b = blue;
    }
    else if (color_format == COLOR_FORMAT_RGB444)
    {
        // color_rgb444.ch.r = red;
        // color_rgb444.ch.g = green;
        // color_rgb444.ch.b = blue;
    }
}

void jgfx_set_color_hex(uint32_t color)
{
    if (color_format == COLOR_FORMAT_RGB666)
    {
        // color_rgb666.ch.r = (color >> 16) & 0xFF;
        // color_rgb666.ch.g = (color >> 8) & 0xFF;
        // color_rgb666.ch.b = color & 0xFF;
    }
    else if (color_format == COLOR_FORMAT_RGB565)
    {
        color_rgb565_front.full = (color);
    }
    else if (color_format == COLOR_FORMAT_RGB444)
    {
    }
}
