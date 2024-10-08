#ifndef __GFX_JAMIEXU_H__
#define __GFX_JAMIEXU_H__
#include "main.h"
#include "stdlib.h"
#include "st7735s.h"


#ifdef USE_FONT_LOADED_IN_MCU
#include "Font.h"
#endif

#define RGB565_TO_RGB666(rgb565) \
    ((((rgb565 >> 11) & 0x1F) << 18) | (((rgb565 >> 5) & 0x3F) << 12) | ((rgb565 & 0x1F) << 6))

#define RGB888_TO_RGB565(rgb888) \
    ((((rgb888 >> 19) & 0x1F) << 11) | (((rgb888 >> 10) & 0x3F) << 5) | ((rgb888 >> 3) & 0x1F))

#define JGFX_GET_CN_FONT_ADDR(f) ((((f & 0xFF) - 0xA1) * 94 + ((f >> 8) & 0xFF)) - 0xA1) * 16 * 16 / 8

#define BUFFER_SIZE DISPLAY_W

#define JGFX_FONT_START_INDEX_EN 0x20

#define JGFX_FONT_START_INDEX_CN 0xA1A1

#define JGFX_FONT_HIGH_MASK 0x80
#define JGFX_FONT_DATA_BUFFER_MAX_SIZE 128

#define JGFXF_COLOR_BLACK 0x0000
#define JGFXF_COLOR_WHITE 0xFFFF
#define JGFXF_COLOR_RED 0xF800
#define JGFXF_COLOR_GRENN 0x07E0
#define JGFXF_COLOR_BLUE 0x001F
#define JGFXF_COLOR_YELLOW 0xFFE0
#define JGFXF_COLOR_CYAN 0x07FF
#define JGFXF_COLOR_GRAY 0x8410
#define JGFXF_COLOR_PINK 0xFDB8
#define JGFXF_COLOR_PURPLE 0x8010

extern SemaphoreHandle_t jgfx_mutex;

// define piexel rgb depth format
typedef union
{
    struct
    {
        uint8_t r : 4;
        uint8_t g : 4;
        uint8_t b : 4;
    } ch;
    uint16_t full;
} jgfx_color_rgb444;

typedef union
{
    struct
    {
        uint16_t r : 5;
        uint16_t g : 6;
        uint16_t b : 5;
    } ch;
    uint16_t full;
} jgfx_color_rgb565;

typedef union
{
    struct
    {
        uint8_t r : 6;
        uint8_t reserved1 : 2;
        uint8_t g : 6;
        uint8_t reserved2 : 2;
        uint8_t b : 6;
        uint8_t reserved3 : 2;
    } ch;
    uint32_t full;
} jgfx_color_rgb666;

typedef struct
{
    jgfx_color_rgb565 *buf1;
    jgfx_color_rgb565 *buf2;
    jgfx_color_rgb565 *buf_act;
    uint32_t buf1_size;
    uint32_t buf2_size;
    uint32_t buf_point;
} jgfx_draw_buffer_t;

typedef enum
{
    COLOR_FORMAT_RGB444 = 0x03,
    COLOR_FORMAT_RGB565 = 0x05,
    COLOR_FORMAT_RGB666 = 0x06,
    COLOR_FORMAT_NO_USED = 0x07
} jgfx_color_format;

typedef enum
{
    JGFX_FONT_CN_16X16 = 0X00,
    // JGFX_FONT_CN_16X16_BLOD,
    JGFX_FONT_EN_8X16,
    JGFX_FONT_EN_8X16_BLOD,
    JGFX_FONT_EN_16X32,
} jgfx_font_sel_t;

typedef struct
{
    uint16_t xs;
    uint16_t xe;
    uint16_t ys;
    uint16_t ye;
} jgfx_area_t;

typedef struct
{
    uint8_t width;
    uint8_t height;
    uint8_t size;
    uint32_t addr;
    uint32_t index;
} jgfx_font_t;

typedef struct gfx
{
    jgfx_area_t area;
    uint8_t flushing;
    jgfx_font_t font;
    jgfx_color_rgb565 color_front;
    jgfx_color_rgb565 color_back;
    jgfx_color_format color_fmt;
    jgfx_draw_buffer_t draw_buf;
    uint8_t font_data[JGFX_FONT_DATA_BUFFER_MAX_SIZE];

    // void (*send_command)(uint8_t);
    // void (*send_data)(uint8_t);
} jgfx_t, *jgfx_ptr;

static jgfx_ptr jgfx;


void jgfx_test(void);
void jgfx_init(uint16_t buf1_size, uint16_t buf2_size);             //  jgfx init
void jgfx_set_color(uint8_t red, uint8_t green, uint8_t blue);      //  set color
void jgfx_set_color_hex(uint32_t color);                            //  set color by hex
void jgfx_set_color_back(uint8_t red, uint8_t green, uint8_t blue); //  set color
void jgfx_set_color_back_hex(uint32_t color);                       //  set color by hex
void jgfx_set_pixel_format(jgfx_color_format x);                    //  set pixel format
void jgfx_draw_pixel(uint8_t x, uint8_t y);
void jgfx_fill_react(uint16_t x, uint16_t y, uint16_t width, uint16_t height); //  fill react
void jgfx_draw_circle(uint16_t x, uint16_t y, uint16_t radius);
void jgfx_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void jgfx_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void jgfx_draw_text(uint16_t x, uint16_t y, uint8_t *str);
void jgfx_draw_text_en(uint16_t x, uint16_t y, uint8_t *str);
void jgfx_draw_text_cn(uint16_t x, uint16_t y, uint8_t *str);
void jgfx_draw_img(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *img);
void jgfx_draw_img_byaddr(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t addr);
void jgfx_set_font(jgfx_font_sel_t font);
void jgfx_set_buff_size(uint32_t buf1_size, uint32_t buf2_size);
uint16_t jgfx_measure_text_width(uint8_t *str);
void jgfx_clear_screen(void);
void jgfx_flush(void);

static void jgfx_reset_area(void);                    //  reset window
static void jgfx_update_area(uint16_t x, uint16_t y); //  update window
void jgfx_send_color(void);

#endif