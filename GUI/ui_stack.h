#ifndef __UI_STACK_JAMIEXU_H__
#define __UI_STACK_JAMIEXU_H__

#include "main.h"
#include "stdlib.h"

#define UI_STACK_MAX_SIZE 10

typedef struct
{
    void (*ui_init)(void);
    void (*ui_destory)(void);
    void (*ui_refresh)(void);
    void (*ui_event_cb)(void);

    bool initial_finished;
} ui_page_t, *ui_page_ptr;

typedef struct
{
    uint8_t pos;
    uint8_t size;
    ui_page_ptr *stack;
} ui_stack_t, *ui_stack_ptr;

void uiStackInit(ui_stack_ptr p, uint8_t size);

void uiStackPush(ui_stack_ptr p, ui_page_ptr page);

ui_page_ptr uiStackPop(ui_stack_ptr p);

ui_page_ptr uiStackGetTop(ui_stack_ptr p);

void uiStatckDestory(ui_stack_ptr p);

#endif