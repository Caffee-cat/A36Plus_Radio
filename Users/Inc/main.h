#ifndef __MAIN_JAMIEXU_H__
#define __MAIN_JAMIEXU_H__

// #define USE_FONT_LOADED_IN_MCU
#define USE_USART_AND_FLASH


#include "gd32f3x0.h"
// #include "../printf/printf.h"
#include "stdarg.h"
#include "gpio.h"
#include "systick.h"
#include "nvic.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "st7735s.h"
#include "backlight.h"
#include "w25q16jv.h"
#include "bk1080.h"
#include "bk4819.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "jgfx.h"
#include "ui.h"
#include "ui_menu.h"
#include "comp_menu.h"
#ifdef USE_USART_AND_FLASH
#include "usart_flash.h"
#endif
// Written by Jamiexu



// Font address
#define FLASH_SPLASH_ADDR 0x7D000        // file: font/splash.bin, offset: 0x7d000
#define FLASH_ICON_CHANNELA_ADDR 0x85140 // file: font/splash.bin, offset: 0x7d000
#define FLASH_ICON_CHANNELB_ADDR 0x85350 // file: font/splash.bin, offset: 0x7d000
#define FLASH_ICON_MAIN_ADDR 0x85560     // file: font/splash.bin, offset: 0x7d000

#define FLASH_FONT_EN_8X16_ADDR 0x85910      // file: font/font_en_8x16.fon, offset: 0x85050
#define FLASH_FONT_EN_8X16_BLOD_ADDR 0x86060 // file: font/font_en_8x16_blod.fon, offset: 0x85690
#define FLASH_FONT_EN_16x32_ADDR 0x867b0     // file: font/font_en_8x16_blod.fon, offset: 0x85690
#define FLASH_FONT_CN_16X16_ADDR 0x88100     // file: font/font_cn_16x16.fon, offset: 0x85cd0

extern uint32_t time1_current_ms;

int main(void);

#endif