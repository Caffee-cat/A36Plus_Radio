#ifndef __USART_FLASH_JAMIEXU_H__
#define __USART_FLASH_JAMIEXU_H__
#include "main.h"

// Header and End
#define USART_FLASH_HEADER 0xAD1991DA
#define USART_FLASH_END 0xDA9119AD

#define USART_FLASH_TIMEOUT 0xFF
// Cmd or Data
typedef enum
{
    USART_FLASH_OPT_CMD = 0x00,
    USART_FLASH_OPT_FLAG = 0x01,
    USART_FLASH_OPT_DATA = 0xFF
} usart_flash_option_t;

// Handshake
typedef enum
{
    USART_FLASH_SHAKE_1 = 0xDF,
    USART_FLASH_SHAKE_2 = 0xFD,
    USART_FLASH_SHAKE_3 = 0x19,
} usart_flash_hanshake_t;

typedef enum
{
    USART_FLASH_FLAG_ACK = 0x00,
    USART_FLASH_FLAG_NACK = 0xFF
} usart_flash_flag_t;

// CMD
typedef enum
{
    USART_FLASH_CMD_FLASH_DATA = 0x01, // Flash data request
    USART_FLASH_CMD_PACK_DATA = 0x02,  // data package
    USART_FLASH_CMD_READ_DATA = 0x03   // read data from flash
} usart_flash_cmd_t;

// State
typedef enum
{
    USART_FLASH_STATE_NONE = 0x00,
    USART_FLASH_STATE_SHAKE_1,
    USART_FLASH_STATE_SHAKE_2,
    USART_FLASH_STATE_SHAKE_OK,

    USART_FLASH_STATE_RCV_DATA,
    USART_FLASH_STATE_TRS_DATA,
} usart_flash_state_t;

typedef void (*usart_flash_send_byte_cb)(uint8_t);

typedef struct
{
    uint32_t header;
    uint8_t option;
    uint32_t size;
    uint8_t *data;
    uint32_t end;
} usart_flash_protocol_t, *usart_flash_protocol_ptr;

typedef struct
{
    uint8_t data[512];     // buffer to be receive
    uint16_t data_pos;     // pos to point to data address
    uint8_t rcv_idle_bit;  // data frame finish flag
    uint8_t timeout_count; // timeout

    uint8_t temp;

    uint32_t rcv_data_addr; // address to be written
    uint32_t trs_data_addr; // address to be read
    uint8_t trs_data_buffer[4096];

    usart_flash_state_t state;        // current FSM state
    usart_flash_protocol_t rcv_proto; // protocol to be receive
    usart_flash_protocol_t trs_proto; // protocol to be send

    usart_flash_send_byte_cb send_cb; // send data callback

} usart_flash_context_t, *usart_flash_context_ptr;

static void protocol_send(usart_flash_protocol_ptr ptr);

static void protocol_unpack(usart_flash_protocol_ptr ptr, uint8_t *data);

static void protocol_pack_data(usart_flash_protocol_ptr ptr, usart_flash_option_t opt, uint32_t size, uint8_t *data);

static void protocol_pack_flag(usart_flash_protocol_ptr ptr, usart_flash_flag_t flag);

static void protocol_pack_handshake(usart_flash_protocol_ptr ptr, usart_flash_hanshake_t shake);

static void process_command(usart_flash_protocol_ptr ptr);

static void process_data(usart_flash_protocol_ptr ptr);

static void process_flag(usart_flash_protocol_ptr ptr);

void usart_flash_cb(uint8_t n);

void usart_flash_run(void);

#endif