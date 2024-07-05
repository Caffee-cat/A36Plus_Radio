#ifndef __UI_STACK_JAMIEXU_H__
#define __UI_STACK_JAMIEXU_H__

#include "main.h"
#include "stdlib.h"

#define UI_STACK_MAX_SIZE 10

typedef struct
{
    int8_t pos;
    uint8_t size;
    uint32_t *stack;
} ui_stack_t, *ui_stack_ptr;

typedef struct
{
    void (*ui_init)(void);
    void (*ui_destory)(void);
    void (*ui_refresh)(void);
    void (*ui_event_cb)(void);
} ui_page_t, *ui_page_ptr;

void uiStackInit(ui_stack_ptr p, uint8_t size);

void uiStackPush(ui_stack_ptr p, ui_page_ptr page);

ui_page_ptr uiStackPop(ui_stack_ptr p);

ui_page_ptr uiStackGetTop(ui_stack_ptr p);

#endif