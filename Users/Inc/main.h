#ifndef __MAIN_JAMIEXU_H__
#define __MAIN_JAMIEXU_H__
#include "gd32f3x0.h"
#include "../printf/printf.h"
#include "gpio.h"
#include "systick.h"
#include "nvic.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "st7735s.h"
#include "w25q16jv.h"
#include "bk1080.h"
#include "bk4819.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "jgfx.h"
#include "ui.h"
#include "usart_flash.h"
// Written by Jamiexu

// Font address
#define FLASH_SPLASH_ADDR 0x7D000                  // file: font/splash.bin, offset: 0x7d000

#define FLASH_FONT_EN_8X16_ADDR 0x83050           // file: font/font_en_8x16.fon, offset: 0x85050
#define FLASH_FONT_EN_8X16_BLOD_ADDR 0x836B0      // file: font/font_en_8x16_blod.fon, offset: 0x85690
#define FLASH_FONT_EN_16x32_ADDR 0x83D10      // file: font/font_en_8x16_blod.fon, offset: 0x85690
#define FLASH_FONT_CN_16X16_ADDR 0x85570 // file: font/font_cn_16x16.fon, offset: 0x85cd0




extern uint32_t time1_current_ms;

int main(void);

#endif