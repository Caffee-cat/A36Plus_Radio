#include "ui_stack.h"

void uiStackInit(ui_stack_ptr p, uint8_t size)
{
    p->stack = (ui_page_ptr *)malloc(size * sizeof(ui_page_ptr));
    if (p->stack == NULL)
        return;

    p->pos = 0;
    p->size = size;
}

void uiStackPush(ui_stack_ptr p, ui_page_ptr page)
{
    if (p == NULL || page == NULL)
        return;

    if (p->pos >= p->size)
        return;

    *(p->stack + p->pos) = page;
    p->pos++;
}

ui_page_ptr uiStackPop(ui_stack_ptr p)
{
    if (p == NULL || p->pos <= 0)
        return NULL;

    p->pos--;
    return *(p->stack + p->pos);
}

ui_page_ptr uiStackGetTop(ui_stack_ptr p)
{
    if (p == NULL || p->pos <= 0)
        return NULL;
    return *(p->stack + (p->pos - 1));
}

void uiStatckDestory(ui_stack_ptr p)
{
    if (p == NULL)
        return;
    free(p->stack);
    p->stack = NULL;
    free(p);
}