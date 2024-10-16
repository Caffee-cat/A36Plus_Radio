
/**
 * @file usart_flash.c
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-28
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
#include "usart_flash.h"

#ifdef USE_USART_AND_FLASH

usart_flash_context_t context = {
    .data = {0},
    .data_pos = 0,
    .rcv_idle_bit = 0,
    .timeout_count = 0,
    .trs_data_addr = 0,
    .rcv_data_addr = 0,
    .temp = 0,
    .state = USART_FLASH_STATE_NONE,
    .rcv_proto = {
        .header = USART_FLASH_HEADER,
        .end = USART_FLASH_END},
    .trs_proto = {.header = USART_FLASH_HEADER, .end = USART_FLASH_END},
    .send_cb = USART_SendByte};

usart_flash_context_ptr context_ptr = &context;

#else 
    usart_flash_context_ptr context_ptr;

#endif
    


static void protocol_unpack(usart_flash_protocol_ptr ptr, uint8_t *data)
{
    uint16_t pos = 0;
    ptr->header = *(uint32_t *)data;
    pos += 4;

    ptr->option = *(uint8_t *)(data + pos);
    pos += 1;

    ptr->size = *(uint32_t *)(data + pos);
    pos += 4;

    ptr->data = data + pos;
    pos += ptr->size;

    ptr->end = ptr->end;
}

static void protocol_send(usart_flash_protocol_ptr ptr)
{
    for (uint8_t i = 0; i < 4; i++)
        context_ptr->send_cb((ptr->header >> (i * 8)) & 0xFF); // header

    context_ptr->send_cb(ptr->option); // options

    for (uint8_t i = 0; i < 4; i++)
        context_ptr->send_cb((ptr->size >> (i * 8)) & 0xFF); // length

    for (uint16_t i = 0; i < ptr->size; i++) // data
        context_ptr->send_cb(*(ptr->data + i));

    for (uint8_t i = 0; i < 4; i++)
        context_ptr->send_cb((ptr->end >> (i * 8)) & 0xFF); // end
}

static void protocol_pack_data(usart_flash_protocol_ptr ptr, usart_flash_option_t opt, uint32_t size, uint8_t *data)
{
    ptr->header = USART_FLASH_HEADER;
    ptr->option = opt;
    ptr->size = size;
    ptr->data = data;
    ptr->end = USART_FLASH_END;
}

static void protocol_pack_flag(usart_flash_protocol_ptr ptr, usart_flash_flag_t flag)
{
    context_ptr->temp = flag;
    protocol_pack_data(ptr, USART_FLASH_OPT_FLAG, 0x01, &context_ptr->temp);
}

static void protocol_pack_handshake(usart_flash_protocol_ptr ptr, usart_flash_hanshake_t shake)
{
    context_ptr->temp = shake;
    protocol_pack_data(ptr, USART_FLASH_OPT_CMD, 0x01, &context_ptr->temp);
}

static void process_command(usart_flash_protocol_ptr ptr)
{
    if (context_ptr->state != USART_FLASH_STATE_SHAKE_OK)
    {
        switch (context_ptr->state)
        {
        case USART_FLASH_STATE_NONE:
            if (*ptr->data == USART_FLASH_SHAKE_1)
            {
                context_ptr->state = USART_FLASH_STATE_SHAKE_1;
                protocol_pack_handshake(&context_ptr->trs_proto, USART_FLASH_SHAKE_1);
            }
            else
            {
                context_ptr->state = USART_FLASH_STATE_NONE;
                protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
            }
            break;
        case USART_FLASH_STATE_SHAKE_1:

            if (*ptr->data == USART_FLASH_SHAKE_2)
            {
                context_ptr->state = USART_FLASH_STATE_SHAKE_2;
                protocol_pack_handshake(&context_ptr->trs_proto, USART_FLASH_SHAKE_2);
            }
            else
            {
                context_ptr->state = USART_FLASH_STATE_NONE;
                protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
            }
            break;
        case USART_FLASH_STATE_SHAKE_2:
            if (*ptr->data == USART_FLASH_SHAKE_3)
            {
                context_ptr->state = USART_FLASH_STATE_SHAKE_OK;
                protocol_pack_handshake(&context_ptr->trs_proto, USART_FLASH_SHAKE_3);
            }
            else
            {
                context_ptr->state = USART_FLASH_STATE_NONE;
                protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
            }
            break;
        default:
            if (*ptr->data == USART_FLASH_SHAKE_1)
            {
                context_ptr->state = USART_FLASH_STATE_SHAKE_1;
                protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_SHAKE_1);
            }
            break;
        }
        protocol_send(&context_ptr->trs_proto);
        return;
    }

    if (*ptr->data == USART_FLASH_SHAKE_1)
    {
        context_ptr->state = USART_FLASH_STATE_SHAKE_1;
        protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_ACK);
    }
    else if (*ptr->data == USART_FLASH_CMD_FLASH_DATA)
    {
        if (ptr->size < 5)
            protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);

        w25q16jv_send_cmd(W25Q16JV_CMD_WRITE_ENABLE);
        w25q16jv_chip_erase();
        context_ptr->timeout_count = 0;
        while (w25q16jv_read_busy() != W25Q16JV_RESET)
        {
            context_ptr->timeout_count++;
            if (context_ptr->timeout_count > USART_FLASH_TIMEOUT)
                protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
        }

        context_ptr->rcv_data_addr = *(uint32_t *)(ptr->data + 1);
        context_ptr->state = USART_FLASH_STATE_RCV_DATA;
        protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_ACK);
    }
    else if (*ptr->data == USART_FLASH_CMD_READ_DATA)
    {

        if (ptr->size < 9)
            protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);

        context_ptr->trs_data_addr = *(uint32_t *)(ptr->data + 1);
        context_ptr->state = USART_FLASH_STATE_TRS_DATA;
        protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_ACK);
    }
    else
        protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
    protocol_send(&context_ptr->trs_proto);
}

static void process_data(usart_flash_protocol_ptr ptr)
{
    if (context_ptr->state == USART_FLASH_STATE_RCV_DATA)
    {
        w25q16jv_send_cmd(W25Q16JV_CMD_WRITE_ENABLE);
        w25q16jv_page_program(context_ptr->rcv_data_addr, ptr->data, ptr->size);
        context_ptr->timeout_count = 0;
        while (w25q16jv_read_busy() != W25Q16JV_RESET)
        {
            context_ptr->timeout_count++;
            if (context_ptr->timeout_count > USART_FLASH_TIMEOUT)
                protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
        }
        context_ptr->rcv_data_addr += W25Q16JV_PAGE_SIZE;
        protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_ACK);
    }
    else if (context_ptr->state == USART_FLASH_STATE_TRS_DATA)
    {
        w25q16jv_read_sector(context_ptr->trs_data_addr, context_ptr->trs_data_buffer);
        protocol_pack_data(&context_ptr->trs_proto, USART_FLASH_OPT_DATA, W25Q16JV_SECTOR_SIZE, context_ptr->trs_data_buffer);
    }
    else
        protocol_pack_flag(&context_ptr->trs_proto, USART_FLASH_FLAG_NACK);
    protocol_send(&context_ptr->trs_proto);
}

static void process_flag(usart_flash_protocol_ptr ptr)
{
    switch (*(uint8_t *)ptr->data)
    {
    case USART_FLASH_FLAG_ACK:

        break;

    case USART_FLASH_FLAG_NACK:
        if (context_ptr->state == USART_FLASH_STATE_RCV_DATA)
            context_ptr->state = USART_FLASH_STATE_NONE;
        break;
    }
}

void usart_flash_cb(uint8_t n)
{
    if (n == 0)
        context_ptr->data[context_ptr->data_pos++] = usart_data_receive(USART0);
    else if (n == 1)
        context_ptr->rcv_idle_bit = 1;
}

void usart_flash_run(void)
{
    // gpio_bit_set(KEY_GPIO_PORT, KEY_GPIO_ROW0_PIN | KEY_GPIO_ROW1_PIN | KEY_GPIO_ROW2_PIN | KEY_GPIO_ROW3_PIN);
    // if (!(gpio_input_bit_get(KEY_GPIO_PORT, KEY_GPIO_COL1_PIN) == RESET && gpio_input_bit_get(KEY_GPIO_PORT, KEY_GPIO_COL2_PIN) == RESET))
    //     return;
    while (1)
    {
        if (context_ptr->rcv_idle_bit == 1)
        {
            if (*(uint32_t *)context_ptr->data == USART_FLASH_HEADER && *(uint32_t *)(context_ptr->data + (context_ptr->data_pos - 5)) == USART_FLASH_END)
            {
                protocol_unpack(&context_ptr->rcv_proto, context_ptr->data);
                if (context_ptr->rcv_proto.option == USART_FLASH_OPT_CMD)
                    process_command(&context_ptr->rcv_proto);
                else if (context_ptr->rcv_proto.option == USART_FLASH_OPT_DATA)
                    process_data(&context_ptr->rcv_proto);
                else if (context_ptr->rcv_proto.option == USART_FLASH_OPT_FLAG)
                    process_flag(&context_ptr->rcv_proto);
            }
            context_ptr->rcv_idle_bit = 0;
            context_ptr->data_pos = 0;
            memset(context_ptr->data, 0, 512);
        }
    }
}
