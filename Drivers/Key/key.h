#ifndef __KEY_JAMIEXU_H__
#define __KEY_JAMIEXU_H__
#include "main.h"
#include "tim.h"


#define KEY_GET_NUM(x) key_return_num(x)

// #define KEY_GET_NUM(x) (x > 16 ? 0 : ((x > 12) ? (x - 6) : ((x > 8) ? ((x == 12) ? 0 : (x - 5)) : ((x > 4) ? (x - 4) : 0))))
// Written by Jamiexu

typedef enum
{
    KEY_MAP_NONE = 0x00,
    KEY_MAP_1,
    KEY_MAP_2,
    KEY_MAP_3,
    KEY_MAP_4,
    KEY_MAP_5,
    KEY_MAP_6,
    KEY_MAP_7,
    KEY_MAP_8,
    KEY_MAP_9,
    KEY_MAP_10,
    KEY_MAP_11,
    KEY_MAP_12,
    KEY_MAP_13,
    KEY_MAP_14,
    KEY_MAP_15,
    KEY_MAP_16,
    KEY_MAP_TOP,
    KEY_MAP_PTT,
    KEY_MAP_L1,
    KEY_MAP_L2
} key_map_t;

key_map_t key_get(void);

uint8_t key_return_num(uint8_t key);

#endif