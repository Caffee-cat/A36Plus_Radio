#ifndef __FMC_H__
#define __FMC_H__
#include "main.h"

#define CHANNEL_PAGE_ADDR 0x0800c800
#define CHANNEL_FMC_INIT_FALG_ADDR 0x0800c800
#define CAHNNEL_FMC_INIT_FALG 0xAD1331DA

#define CHANNEL_A_KHZ_ADDR 0x0800c810 // word
#define CHANNEL_A_HZ_ADDR 0x0800c814  // half word

#define CHANNEL_B_KHZ_ADDR 0x0800c820 // word
#define CHANNEL_B_HZ_ADDR 0x0800c824  // half word


uint32_t channel_A_get(void);

uint32_t channel_B_get(void);

void FMC_channel_init(void);

void FMC_data_init(void);

void channel_A_store(uint32_t channel_val);

void channel_B_store(uint32_t channel_val);

#endif