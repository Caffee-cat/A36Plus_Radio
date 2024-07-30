#ifndef __COMP_MENU_JAMIEXU_H__
#define __COMP_MENU_JAMIEXU_H__
#include "main.h"
#define JGFX_MENU_STR_MAX_LEN 10

typedef struct jgfx_menu_item_t *jgfx_menu_item_ptr;
typedef struct jgfx_menu_t *jgfx_menu_ptr;
typedef void (*jgfx_menu_item_event_cb)(jgfx_menu_ptr);

typedef enum
{
    JGFX_MENU_TYPE_TXT = 0x00,
    JGFX_MENU_TYPE_COMBOBOX,
    JGFX_MENU_TYPE_SWITCH
} jgfx_menu_item_type_t;

typedef enum
{
    JGFX_MENU_ITEM_STATUS_SELECTED = 0x00,
    JGFX_MENU_ITEM_STATUS_UNSELECTED,
    JGFX_MENU_ITEM_STATUS_CLICKED,
}jgfx_menu_item_status_t;

// typedef enum{
//     JGFX_MENU_STATUS_
// }

typedef struct jgfx_menu_item_t
{
    jgfx_menu_item_type_t item_type;
    jgfx_menu_ptr parent;

    uint8_t item_name[JGFX_MENU_STR_MAX_LEN];
    uint8_t *item_img_data;

    uint8_t **combox_menu;
    uint8_t combox_index;

    uint8_t **switch_item;
    uint8_t switch_status;

    jgfx_menu_item_ptr item_pre;
    jgfx_menu_item_ptr item_next;

    jgfx_menu_item_event_cb item_event_cb;

} jgfx_menu_item_t;

typedef struct jgfx_menu_t
{
    uint16_t menu_x;
    uint16_t menu_y;
    uint16_t menu_width;
    uint16_t menu_height;

    uint8_t menu_item_width;
    uint8_t menu_item_height;

    uint8_t menu_divide_height;

    uint8_t menu_use_tiltle;
    uint8_t menu_title[JGFX_MENU_STR_MAX_LEN];
    uint8_t menu_title_height;

    uint8_t index;  
    uint8_t item_size;
    uint8_t show_item_num;

    jgfx_menu_item_ptr head_item;
    jgfx_menu_item_ptr cur_item;
} jgfx_menu_t;

void jgfx_menu_init(jgfx_menu_t * menu_ptr);

void jgfx_menu_set_size(jgfx_menu_ptr menu_ptr, uint16_t width, uint16_t height);

void jgfx_menu_set_item_size(jgfx_menu_ptr menu_ptr, uint16_t width, uint16_t height);

void jgfx_menu_set_position(jgfx_menu_ptr menu_ptr, uint16_t x, uint16_t y);

void jgfx_menu_set_divide(jgfx_menu_ptr menu_ptr, uint8_t height);

void jgfx_menu_append_text(jgfx_menu_ptr menu_ptr, uint8_t *str, jgfx_menu_item_event_cb cb);

void jgfx_menu_show(jgfx_menu_ptr menu_ptr);

void jgfx_menu_set_title(jgfx_menu_ptr menu_ptr, uint8_t *str);

void jgfx_menu_set_title_height(jgfx_menu_ptr menu_ptr, uint8_t height);

void jgfx_menu_item_next(jgfx_menu_ptr menu_ptr);

void jgfx_menu_item_previous(jgfx_menu_ptr menu_ptr);

void jgfx_menu_update(jgfx_menu_ptr menu_ptr);

void jgfx_menu_click(jgfx_menu_ptr menu_ptr);

void jgfx_menu_destory(jgfx_menu_ptr menu_ptr);

static void _l_destory_menu_item(jgfx_menu_item_ptr item);

static void _r_menu_draw_items(jgfx_menu_ptr menu_ptr);

static void _l_menu_append_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr);

static void _r_menu_draw_selector(jgfx_menu_ptr menu_ptr);

static void _r_menu_draw_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr, jgfx_menu_item_status_t status, uint8_t index);

static void _r_clear_item_area(jgfx_menu_ptr menu_ptr);

#endif