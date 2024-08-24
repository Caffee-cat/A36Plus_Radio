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

#ifdef USE_FONT_LOADED_IN_MCU
extern uint8_t font8x16[][16];
extern uint8_t font8x16_song[];
extern uint8_t font_16x32_song[];

static void font_8x16_for_temp(uint16_t offset, uint8_t *readData, uint32_t num)
{
    for (uint16_t i = 0; i < num; i++)
        readData[i] = font8x16[offset][i];
}

static void font_8x16_song(uint16_t offset, uint8_t *readData, uint32_t num)
{
    for (uint16_t i = 0; i < num; i++)
        readData[i] = font8x16_song[offset * 16 + i];
}

static void font_16x32_blodface(uint16_t offset, uint8_t *readData, uint32_t num)
{
    for (uint16_t i = 0; i < num; i++)
        readData[i] = font_16x32_song[offset * 64 + i];
}

#endif

static jgfx_ptr jgfx;

void jgfx_init(uint16_t buf1_size, uint16_t buf2_size)
{
    jgfx = (jgfx_t *)malloc(sizeof(jgfx_t));
    memset(jgfx, 0, sizeof(jgfx_t));

    if (jgfx == NULL)
        return;

    if (buf1_size != 0)
    {
        jgfx->draw_buf.buf1 = (jgfx_color_rgb565 *)malloc(BUFFER_SIZE * sizeof(jgfx_color_rgb565));
    }
    if (buf1_size != 0 && buf2_size != 0)
    {
        jgfx->draw_buf.buf2 = (jgfx_color_rgb565 *)malloc(BUFFER_SIZE * sizeof(jgfx_color_rgb565));
    }

    jgfx->draw_buf.buf1_size = BUFFER_SIZE;
    jgfx->draw_buf.buf2_size = BUFFER_SIZE;
    jgfx->draw_buf.buf_point = 0;
    jgfx->draw_buf.buf_act = jgfx->draw_buf.buf1;
    jgfx->flushing = 0;

    jgfx->color_front.full = 0x00;
    jgfx->color_back.full = 0x00;
    jgfx->color_fmt = COLOR_FORMAT_RGB565;

    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_pixel_format(jgfx->color_fmt);
}

void jgfx_set_pixel_format(jgfx_color_format x)
{
    jgfx->color_fmt = x;
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
    if (jgfx->color_fmt == COLOR_FORMAT_RGB666)
    {
        // st7735s_send_data(color_rgb666.ch.r << 2);
        // st7735s_send_data(color_rgb666.ch.g << 2);
        // st7735s_send_data(color_rgb666.ch.b << 2);
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB565)
    {
        st7735s_send_data(((jgfx->color_front.full >> 8) & 0xFF));
        st7735s_send_data(jgfx->color_front.full & 0xFF);
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB444)
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
    st7735s_set_window(x, x + width - 1, y, y + height - 1);
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
            if (jgfx->draw_buf.buf_act == NULL)
            {
                st7735s_send_data(((jgfx->color_back.full >> 8) & 0xFF));
                st7735s_send_data(jgfx->color_back.full & 0xFF);
                continue;
            }
            else
            {
                ((jgfx->draw_buf.buf_act) + (jgfx->draw_buf.buf_point))->full = ((jgfx->color_back.full >> 8) | (jgfx->color_back.full << 8));
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
    uint8_t read_len = 0;
    uint32_t byte_num = w * h * 2;
    st7735s_set_window(x, x + w - 1, y, y + h - 1);
    while (i < byte_num)
    {
        read_len = (byte_num - i < sizeof(jgfx->font_data)) ? (byte_num - i) : sizeof(jgfx->font_data);
        w25q16jv_read_num(addr + i, jgfx->font_data, read_len);
        i += read_len;

        for (uint32_t j = 0; j < read_len; j += 2)
        {
            // temp =
            st7735s_send_data(*(uint8_t *)(jgfx->font_data + j));
            st7735s_send_data(*(uint8_t *)(jgfx->font_data + j + 1));
            // (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full = (*(uint8_t *)(jgfx->font_data + j) << 8);
            // (jgfx->draw_buf.buf_act + jgfx->draw_buf.buf_point)->full |= (*(uint8_t *)(jgfx->font_data + j + 1));
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
    while (*str != '\0')
    {
        if (*(uint16_t *)str >= JGFX_FONT_START_INDEX_CN)
        {
            jgfx_set_font(JGFX_FONT_CN_16X16);
            jgfx_draw_text_cn(x, y, str);
            str += 2;
        }
        else
        {
            jgfx_set_font(JGFX_FONT_EN_8X16);
            jgfx_draw_text_cn(x, y, str);
            str++;
        }

        x += jgfx->font.width;
        if (x > DISPLAY_W - jgfx->font.width + 32)
        {
            y += jgfx->font.height;
            x = 32;
        }
    }
}

void jgfx_draw_text_en(uint16_t x, uint16_t y, uint8_t *str)
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

        offset = *str - jgfx->font.index;

#ifdef USE_FONT_LOADED_IN_MCU

        // replace font_data with font loaded in MCU temporarily
        if (jgfx->font.addr == FLASH_FONT_EN_8X16_ADDR)
        {
            font_8x16_song(offset, jgfx->font_data, jgfx->font.size);
        }
        else if (jgfx->font.addr == FLASH_FONT_EN_8X16_BLOD_ADDR)
        {
            font_8x16_for_temp(offset + jgfx->font.index, jgfx->font_data, jgfx->font.size);
        }
        else if (jgfx->font.addr == FLASH_FONT_EN_16x32_ADDR)
        {
            font_16x32_blodface(offset, jgfx->font_data, jgfx->font.size);
        }
#else
        w25q16jv_read_num(jgfx->font.addr + offset * jgfx->font.size, jgfx->font_data, jgfx->font.size);
#endif

        for (uint16_t i = 0; i < font_height * font_width; i++)
        {
            if (i % 8 == 0)
            {
                temp = jgfx->font_data[j++];

                k = 0;
            }

            if (((temp << k++) & JGFX_FONT_HIGH_MASK) == JGFX_FONT_HIGH_MASK)
            {
                st7735s_send_data(((jgfx->color_front.full >> 8) & 0xFF));
                st7735s_send_data(jgfx->color_front.full & 0xFF);
            }
            else
            {
                st7735s_send_data(((jgfx->color_back.full >> 8) & 0xFF));
                st7735s_send_data(jgfx->color_back.full & 0xFF);
            }
        }

        j = 0;
        x += font_width;
        if (x > DISPLAY_W - font_width + 32)
        {
            y += font_height;
            x = 32;
        }

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

        w25q16jv_read_num(jgfx->font.addr + JGFX_GET_CN_FONT_ADDR(*(uint16_t *)str), jgfx->font_data, jgfx->font.size);

        for (uint16_t i = 0; i < font_height * font_width; i++)
        {
            if (i % 8 == 0)
            {
                temp = jgfx->font_data[j++];
                k = 0;
            }

            if (((temp << k++) & JGFX_FONT_HIGH_MASK) == JGFX_FONT_HIGH_MASK)
            {
                st7735s_send_data(((jgfx->color_front.full >> 8) & 0xFF));
                st7735s_send_data(jgfx->color_front.full & 0xFF);
            }
            else
            {
                st7735s_send_data(((jgfx->color_back.full >> 8) & 0xFF));
                st7735s_send_data(jgfx->color_back.full & 0xFF);
            }
        }

        j = 0;
        x += font_width;
        if (x > DISPLAY_W - font_width + 32)
        {
            x = 32;
            y += font_height;
        }

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

void jgfx_clear_screen(void)
{
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(32, 0, DISPLAY_W, DISPLAY_H);
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

    if (jgfx->draw_buf.buf2 != NULL)
    {
        if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf1)
            jgfx->draw_buf.buf_act = jgfx->draw_buf.buf2;
        else if (jgfx->draw_buf.buf_act == jgfx->draw_buf.buf2)
            jgfx->draw_buf.buf_act = jgfx->draw_buf.buf1;
    }

    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    dma_channel_enable(DMA_CH4);
}

void jgfx_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    if (jgfx->color_fmt == COLOR_FORMAT_RGB666)
    {
        // color_rgb666.ch.r = red;
        // color_rgb666.ch.g = green;
        // color_rgb666.ch.b = blue;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB565)
    {
        jgfx->color_front.ch.r = red;
        jgfx->color_front.ch.g = green;
        jgfx->color_front.ch.b = blue;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB444)
    {
        // color_rgb444.ch.r = red;
        // color_rgb444.ch.g = green;
        // color_rgb444.ch.b = blue;
    }
}

uint16_t jgfx_measure_text_width(uint8_t *str)
{
    uint16_t len = 0;
    while (*str != '\0')
    {
        len += jgfx->font.width;
        str++;
    }
    return len;
}

void jgfx_set_color_hex(uint32_t color)
{
    if (jgfx->color_fmt == COLOR_FORMAT_RGB666)
    {
        // color_rgb666.ch.r = (color >> 16) & 0xFF;
        // color_rgb666.ch.g = (color >> 8) & 0xFF;
        // color_rgb666.ch.b = color & 0xFF;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB565)
    {
        jgfx->color_front.full = color;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB444)
    {
    }
}

void jgfx_set_color_back(uint8_t red, uint8_t green, uint8_t blue)
{
    if (jgfx->color_fmt == COLOR_FORMAT_RGB666)
    {
        // color_rgb666.ch.r = red;
        // color_rgb666.ch.g = green;
        // color_rgb666.ch.b = blue;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB565)
    {
        jgfx->color_back.ch.r = red;
        jgfx->color_back.ch.g = green;
        jgfx->color_back.ch.b = blue;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB444)
    {
        // color_rgb444.ch.r = red;
        // color_rgb444.ch.g = green;
        // color_rgb444.ch.b = blue;
    }
}
void jgfx_set_color_back_hex(uint32_t color)
{
    if (jgfx->color_fmt == COLOR_FORMAT_RGB666)
    {
        // color_rgb666.ch.r = (color >> 16) & 0xFF;
        // color_rgb666.ch.g = (color >> 8) & 0xFF;
        // color_rgb666.ch.b = color & 0xFF;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB565)
    {
        jgfx->color_back.full = color;
    }
    else if (jgfx->color_fmt == COLOR_FORMAT_RGB444)
    {
    }
}
