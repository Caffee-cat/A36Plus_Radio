/**
 * @file comp_menu.c
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-23
 *
 * @copyright MIT License
 *              Copyright (c) 2024 (Jamiexu or Jamie793)
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 *
 */

#include "comp_menu.h"

/**
 * @brief Init menu
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_init(jgfx_menu_ptr menu_ptr)
{
    menu_ptr->menu_x = 0;
    menu_ptr->menu_y = 0;
    menu_ptr->menu_width = 0;
    menu_ptr->menu_height = 0;
    menu_ptr->menu_divide_height = 0;
    menu_ptr->menu_item_height = 20;
    menu_ptr->menu_use_tiltle = 0;
    menu_ptr->index = 0;
    menu_ptr->item_size = 0;
}

/**
 * @brief Set menu size
 *
 * @param menu_ptr menu pointer
 * @param width menu width
 * @param height menu height
 */
void jgfx_menu_set_size(jgfx_menu_ptr menu_ptr, uint16_t width, uint16_t height)
{
    menu_ptr->menu_width = width;
    menu_ptr->menu_height = height;
}

/**
 * @brief Set menu item size
 *
 * @param menu_ptr menu pointer
 * @param width item width
 * @param height item height
 */
void jgfx_menu_set_item_size(jgfx_menu_ptr menu_ptr, uint16_t width, uint16_t height)
{
    menu_ptr->menu_item_width = width;
    menu_ptr->menu_item_height = height;
}

/**
 * @brief Set menu position
 *
 * @param menu_ptr menu pointer
 * @param x menu x
 * @param y meun y
 */
void jgfx_menu_set_position(jgfx_menu_ptr menu_ptr, uint16_t x, uint16_t y)
{
    menu_ptr->menu_x = x;
    menu_ptr->menu_y = y;
}

/**
 * @brief Set menu item divide height
 *
 * @param menu_ptr menu pointer
 * @param height divide height
 */
void jgfx_menu_set_divide(jgfx_menu_ptr menu_ptr, uint8_t height)
{
    menu_ptr->menu_divide_height = height;
}

/**
 * @brief Append text style menu
 *
 * @param menu_ptr menu pointer
 * @param str menu text
 * @param cb click callback
 */
void jgfx_menu_append_text(jgfx_menu_ptr menu_ptr, uint8_t *str, jgfx_menu_item_event_cb cb)
{
    jgfx_menu_item_ptr item = (jgfx_menu_item_ptr)malloc(sizeof(jgfx_menu_item_t));
    if (item == NULL)
        return;
    memset(item, 0, sizeof(jgfx_menu_item_t));
    memcpy(item->item_name, str, strlen(str));
    item->parent = menu_ptr;
    item->item_event_cb = cb;	

    item->item_type = JGFX_MENU_TYPE_TXT;
    _l_menu_append_item(menu_ptr, item);
}

/**
 * @brief Show menu
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_show(jgfx_menu_ptr menu_ptr)
{
    /** Init item selected */
    menu_ptr->show_item_num = (menu_ptr->menu_height - (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0)) / (menu_ptr->menu_item_height + menu_ptr->menu_divide_height);
    menu_ptr->cur_item = menu_ptr->head_item;
    menu_ptr->index = 0;

    uint8_t i = 0;

    /** Draw menue box*/
    jgfx_set_color_hex(0xFFFF);
    jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 30, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    /** Show title if use title*/
    if (menu_ptr->menu_use_tiltle)
    {
        jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 30, menu_ptr->menu_y + menu_ptr->menu_title_height + 2);
        jgfx_set_font(JGFX_FONT_EN_8X16);
        jgfx_draw_text_en(32 + menu_ptr->menu_x + (menu_ptr->menu_width / 2) - (jgfx_measure_text_width(menu_ptr->menu_title) / 2), menu_ptr->menu_y + 1, menu_ptr->menu_title);
    }

    /** Draw items */
    _r_menu_draw_items(menu_ptr);
}

/**
 * @brief Set menu title
 *
 * @param menu_ptr menu pointer
 * @param str title text
 */
void jgfx_menu_set_title(jgfx_menu_ptr menu_ptr, uint8_t *str)
{
    menu_ptr->menu_use_tiltle = 1;
    memcpy(menu_ptr->menu_title, str, strlen(str));
}

/**
 * @brief Set title height
 *
 * @param menu_ptr menu pointer
 * @param height title height
 */
void jgfx_menu_set_title_height(jgfx_menu_ptr menu_ptr, uint8_t height)
{
    menu_ptr->menu_title_height = height;
}

/**
 * @brief Set current item to next item
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_item_next(jgfx_menu_ptr menu_ptr)
{
    if (menu_ptr->cur_item->item_next == NULL)
        return;

    menu_ptr->index++;
    menu_ptr->cur_item = menu_ptr->cur_item->item_next;
    if (menu_ptr->index == menu_ptr->show_item_num)
    {
        menu_ptr->index = 0;
        _r_clear_item_area(menu_ptr);
        _r_menu_draw_items(menu_ptr);
    }
    _r_menu_draw_selector(menu_ptr);
}

/**
 * @brief Set current to previous item
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_item_previous(jgfx_menu_ptr menu_ptr)
{
    if (menu_ptr->cur_item->item_pre == NULL)
        return;

    menu_ptr->cur_item = menu_ptr->cur_item->item_pre;
    menu_ptr->index--;
    if (menu_ptr->index == 0)
    {
        menu_ptr->index = menu_ptr->show_item_num;
        _r_clear_item_area(menu_ptr);
        _r_menu_draw_items(menu_ptr);
    }
    _r_menu_draw_selector(menu_ptr);
}

/**
 * @brief Update status
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_update(jgfx_menu_ptr menu_ptr)
{
    _r_menu_draw_selector(menu_ptr);
}

/**
 * @brief Menu click listener
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_click(jgfx_menu_ptr menu_ptr)
{
    _r_menu_draw_item(menu_ptr, menu_ptr->cur_item->item_pre, JGFX_MENU_ITEM_STATUS_CLICKED, menu_ptr->index - 1);
    if (menu_ptr->cur_item->item_event_cb != NULL)
        menu_ptr->cur_item->item_event_cb(menu_ptr);
}

/**
 * @brief Destory menu and free memory
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_destory(jgfx_menu_ptr menu_ptr)
{
    jgfx_menu_item_ptr cur = menu_ptr->head_item;
    jgfx_menu_item_ptr next;

    if (menu_ptr == NULL)
        return;

    while (cur != NULL)
    {
        next = cur->item_next;
        _l_destory_menu_item(cur);
        cur = next;
    }

    _l_destory_menu_item(menu_ptr->head_item);
    _l_destory_menu_item(menu_ptr->cur_item);

    // free(menu_ptr);
}

/**
 * @brief Destory item
 *
 * @param item item needed to destory
 */
static void _l_destory_menu_item(jgfx_menu_item_ptr item)
{
    if (item == NULL)
        return;

    // if (item->item_img_data != NULL)
    // {
    //     free(item->item_img_data);
    // }

    // if (item->combox_menu != NULL)
    // {
    //     for (int i = 0; item->combox_menu[i] != NULL; ++i)
    //     {
    //         free(item->combox_menu[i]);
    //     }
    //     free(item->combox_menu);
    // }

    // if (item->switch_item != NULL)
    // {
    //     for (int i = 0; item->switch_item[i] != NULL; ++i)
    //     {
    //         free(item->switch_item[i]);
    //     }
    //     free(item->switch_item);
    // }

    free(item);
}

/**
 * @brief Append item to menu
 *
 * @param menu_ptr menu pointer
 * @param item_ptr item neeeded to append
 */
static void _l_menu_append_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr)
{
    menu_ptr->head_item == NULL;
    if (menu_ptr->head_item == NULL)
    {
        menu_ptr->head_item = menu_ptr->cur_item = item_ptr;
    }
    else
    {
        // menu_ptr
        menu_ptr->cur_item->item_next = item_ptr;
        item_ptr->item_pre = menu_ptr->cur_item;
        menu_ptr->cur_item = item_ptr;
    }
    menu_ptr->item_size++;
}

/**
 * @brief Draw all of items
 *
 * @param menu_ptr menu pointer
 */
static void _r_menu_draw_items(jgfx_menu_ptr menu_ptr)
{
    jgfx_menu_item_ptr item = menu_ptr->cur_item;
    uint8_t i = 0;

    while (item != NULL)
    {
        _r_menu_draw_item(menu_ptr, item, JGFX_MENU_ITEM_STATUS_UNSELECTED, i++);
        item = item->item_next;
        if (i >= menu_ptr->show_item_num)
            break;
    }

    _r_menu_draw_selector(menu_ptr);
}

/**
 * @brief Draw selector
 *
 * @param menu_ptr menu pointer
 */
static void _r_menu_draw_selector(jgfx_menu_ptr menu_ptr)
{
    if (menu_ptr->index != 0)
    {
        _r_menu_draw_item(menu_ptr, menu_ptr->cur_item->item_pre, JGFX_MENU_ITEM_STATUS_UNSELECTED, menu_ptr->index - 1);
    }

    _r_menu_draw_item(menu_ptr, menu_ptr->cur_item, JGFX_MENU_ITEM_STATUS_SELECTED, menu_ptr->index);

    if (menu_ptr->index != menu_ptr->item_size - 1)
    {
        _r_menu_draw_item(menu_ptr, menu_ptr->cur_item->item_next, JGFX_MENU_ITEM_STATUS_UNSELECTED, menu_ptr->index + 1);
    }
}

/**
 * @brief Draw one item
 *
 * @param menu_ptr menu pointer
 * @param item_ptr item pointed to item structure
 * @param status current item status
 * @param index
 */
static void _r_menu_draw_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr, jgfx_menu_item_status_t status, uint8_t index)
{
    /** Draw item base on item status*/
    if (status == JGFX_MENU_ITEM_STATUS_SELECTED)
    {
        jgfx_set_color_hex(0x0000);
        jgfx_set_color_back_hex(0xFFFF);
    }
    else if (status == JGFX_MENU_ITEM_STATUS_UNSELECTED)
    {
        jgfx_set_color_hex(0xFFFF);
        jgfx_set_color_back_hex(0x0000);
    }
    else if (status == JGFX_MENU_ITEM_STATUS_CLICKED)
    {
        jgfx_set_color_hex(0xFFFF);
        jgfx_set_color_back_hex(0x0000);

        /** Draw item box */
        jgfx_draw_rect(35 + menu_ptr->menu_x,
                       (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
                       menu_ptr->menu_x + menu_ptr->menu_width + 30,
                       menu_ptr->menu_y + menu_ptr->menu_item_height);
    }

    /** Draw item */
    jgfx_set_font(JGFX_FONT_EN_8X16);
    if (item_ptr->item_type == JGFX_MENU_TYPE_TXT)
    {
        jgfx_draw_text_en(36 + menu_ptr->menu_x,
                          menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
                          item_ptr->item_name);
    }
    else if (item_ptr->item_type == JGFX_MENU_TYPE_COMBOBOX)
    {
    }
    else if (item_ptr->item_type == JGFX_MENU_TYPE_SWITCH)
    {
    }

    // jgfx_draw_line(36 + menu_ptr->menu_x,
    //                menu_ptr->menu_item_height + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
    //                menu_ptr->menu_width + 30,
    //                menu_ptr->menu_item_height + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index);
}

static void _r_clear_item_area(jgfx_menu_ptr menu_ptr)
{
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(33 + menu_ptr->menu_x, menu_ptr->menu_y + menu_ptr->menu_title_height + 2, menu_ptr->menu_width - 1, menu_ptr->menu_height - menu_ptr->menu_title_height);
}