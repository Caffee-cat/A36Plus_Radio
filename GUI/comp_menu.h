#ifndef __COMP_MENU_JAMIEXU_H__
#define __COMP_MENU_JAMIEXU_H__
#include "main.h"
#define JGFX_MENU_STR_MAX_LEN 10

typedef struct jgfx_menu_item_t *jgfx_menu_item_ptr;
typedef struct jgfx_menu_t *jgfx_menu_ptr;
typedef struct submenu_item_t *submenu_item_ptr;
typedef struct corner_index_num_t *corner_index_num_ptr;
typedef void (*jgfx_menu_item_event_cb)(jgfx_menu_ptr);

typedef enum
{
    JGFX_MENU_TYPE_TXT = 0x00,
    JGFX_MENU_TYPE_COMBOBOX,
    JGFX_MENU_TYPE_SWITCH
} jgfx_menu_item_type_t;

typedef enum
{
    JGFX_MENU_STATUS_SELECTED = 0x00,
    JGFX_MENU_STATUS_UNSELECTED,
    JGFX_MENU_STATUS_CLICKED,
    JGFX_MENU_STATUS_SHUMENU,
} jgfx_menu_status_t;

typedef enum
{
    JGFX_MENU_ITEM_STATUS_SELECTED = 0x00,
    JGFX_MENU_ITEM_STATUS_UNSELECTED,
    JGFX_MENU_ITEM_STATUS_CLICKED,
} jgfx_menu_item_status_t;

// typedef enum{
//     JGFX_MENU_STATUS_
// }

typedef struct jgfx_menu_item_t
{
    jgfx_menu_item_type_t item_type;
    jgfx_menu_ptr parent;
    uint8_t item_id;

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
    uint8_t item_show_num;
    uint8_t page;
    uint8_t Initial_flag;
    jgfx_menu_status_t status;

    jgfx_menu_item_ptr head_item;
    jgfx_menu_item_ptr cur_item;
} jgfx_menu_t;

typedef struct submenu_item_t
{
    uint8_t *item_name[5];
    uint16_t line_height;
    uint8_t itemlist_num;
} submenu_item_t;

typedef struct corner_index_num_t
{
    uint8_t index_num1;
    uint8_t index_num2;
    uint16_t index_jump_count1;
    uint16_t index_jump_count2;
    void (*refresh)(void);
} corner_index_num_t;

void jgfx_menu_init(jgfx_menu_t *menu_ptr);

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

bool jgfx_menu_index(jgfx_menu_ptr menu_ptr, uint8_t index);

void jgfx_menu_destory(jgfx_menu_ptr menu_ptr);

void submenu_item_show(jgfx_menu_ptr menu_ptr, uint8_t item_num, submenu_item_ptr submenu_ptr, ...);

void index_num_display(jgfx_menu_ptr menu_ptr);

void corner_index_init(corner_index_num_ptr corner_ptr);

void corner_index_refresh(jgfx_menu_ptr menu_ptr, corner_index_num_ptr corner_ptr);

void corner_index_flicker(jgfx_menu_ptr menu_ptr, corner_index_num_ptr corner_ptr);

uint8_t index_num_cb(corner_index_num_ptr corner_ptr, jgfx_menu_ptr menu_ptr, uint8_t key);

static void _l_destory_menu_item(jgfx_menu_item_ptr item);

static void _r_menu_draw_items(jgfx_menu_ptr menu_ptr, uint8_t order);

static void _l_menu_append_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr);

static void _r_menu_draw_selector(jgfx_menu_ptr menu_ptr);

static void _r_menu_draw_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr, jgfx_menu_item_status_t status, uint8_t index);

static void _r_clear_item_area(jgfx_menu_ptr menu_ptr);

static void submenu_init(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr, uint8_t num);

static void submenu_cb(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr);

#endif