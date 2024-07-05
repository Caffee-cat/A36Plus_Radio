#include "ui_stack.h"

void uiStackInit(ui_stack_ptr p, uint8_t size)
{
    p->stack = (uint32_t *)malloc(size);
    p->pos = 0;
    p->size = size;
}

void uiStackPush(ui_stack_ptr p, ui_page_ptr page)
{
    if (p == NULL || page == NULL)
        return;

    if (p->pos >= p->size - 1)
        return;

    *(p->stack + p->pos) = (uint32_t)page;
    p->pos++;
}

ui_page_ptr uiStackPop(ui_stack_ptr p)
{
    if (p == NULL || p->pos < 0)
        return NULL;

    ui_page_ptr temp = (ui_page_ptr)(p->stack + p->pos);
    p->pos--;
    return temp;
}

ui_page_ptr uiStackGetTop(ui_stack_ptr p)
{
    if (p == NULL || p->pos < 0)
        return NULL;
    return (ui_page_ptr)(p->stack + p->pos);
}