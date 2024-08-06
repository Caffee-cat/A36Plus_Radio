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
    menu_ptr->Initial_flag = 1;
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
    item->item_id = menu_ptr->item_size;
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
    menu_ptr->item_show_num = (menu_ptr->menu_height - (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0)) / (menu_ptr->menu_item_height + menu_ptr->menu_divide_height);
    menu_ptr->cur_item = menu_ptr->head_item;
    menu_ptr->index = 0;

    uint8_t i = 0;

    /** Draw menu box*/
    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 31, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    /** Show title if use title*/
    if (menu_ptr->menu_use_tiltle)
    {
        jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 30, menu_ptr->menu_y + menu_ptr->menu_title_height + 2);
        jgfx_set_font(JGFX_FONT_EN_8X16);
        jgfx_draw_text_en(32 + menu_ptr->menu_x + (menu_ptr->menu_width / 2) - (jgfx_measure_text_width(menu_ptr->menu_title) / 2), menu_ptr->menu_y + 1, menu_ptr->menu_title);
    }

    /** Draw items */
    _r_menu_draw_items(menu_ptr, 0);

    /** Draw selector */
    _r_menu_draw_selector(menu_ptr);
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
    if (menu_ptr->status == JGFX_MENU_STATUS_CLICKED)
        return;

    if (menu_ptr->cur_item->item_next == NULL)
        return;

    menu_ptr->cur_item = menu_ptr->cur_item->item_next;
    menu_ptr->index++;
    if (menu_ptr->index == menu_ptr->item_show_num)
    {
        menu_ptr->index = 0;
        _r_clear_item_area(menu_ptr);
        _r_menu_draw_items(menu_ptr, 0);
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
    if (menu_ptr->status == JGFX_MENU_STATUS_CLICKED)
        return;

    if (menu_ptr->cur_item->item_pre == NULL)
        return;

    menu_ptr->cur_item = menu_ptr->cur_item->item_pre;
    if (menu_ptr->index == 0)
    {
        menu_ptr->index = menu_ptr->item_show_num;
        _r_clear_item_area(menu_ptr);
        _r_menu_draw_items(menu_ptr, 1);
    }
    menu_ptr->index--;
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
    if (menu_ptr->status != JGFX_MENU_STATUS_CLICKED)
    {
        menu_ptr->status = JGFX_MENU_STATUS_CLICKED;
        _r_menu_draw_item(menu_ptr, menu_ptr->cur_item, JGFX_MENU_ITEM_STATUS_CLICKED, menu_ptr->index);
        if (menu_ptr->cur_item->item_event_cb != NULL)
            menu_ptr->cur_item->item_event_cb(menu_ptr);
    }
    else
    {
        menu_ptr->status = JGFX_MENU_STATUS_SELECTED;
        _r_menu_draw_item(menu_ptr, menu_ptr->cur_item, JGFX_MENU_ITEM_STATUS_SELECTED, menu_ptr->index);
    }
}

/**
 * @brief Index menu item
 *
 * @param menu_ptr menu pointer
 * @param index item index
 */
void jgfx_menu_index(jgfx_menu_ptr menu_ptr, uint8_t index)
{
    if (menu_ptr == NULL || index > (menu_ptr->item_size - 1))
        return;
    uint8_t i;
    jgfx_menu_item_ptr item = menu_ptr->head_item;
    jgfx_menu_item_ptr item2 = item;
    // pointing the item and item2 to the wanted item.
    for (i = 1; i < index; i++)
    {
        item = item2 = item->item_next;
    }
    // change the way to calculate,avoiding that i would have the risk to be reduce to minus.
    i = ((i - 1) % (menu_ptr->item_show_num));
    while (i--)
    {
        item2 = item2->item_pre;
    }
    // use item2 to point to the first item on the page for drawing list.
    menu_ptr->cur_item = item2;
    _r_clear_item_area(menu_ptr);
    _r_menu_draw_items(menu_ptr, 0);

    menu_ptr->index = ((index - 1) % menu_ptr->item_show_num);
    menu_ptr->cur_item = item;
    _r_menu_draw_selector(menu_ptr);
}

/**
 * @brief Destory menu and free memory
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_destory(jgfx_menu_ptr menu_ptr)
{
    if (menu_ptr == NULL)
        return;
    jgfx_menu_item_ptr cur = menu_ptr->head_item;
    jgfx_menu_item_ptr next;

    while (cur != NULL)
    {
        next = cur->item_next;
        _l_destory_menu_item(cur);
        cur = next;
    }
    // The program have some error here.Although it looks like would lead to memory overflow,but when the program running to free cur_item,it would stuck.
    //  _l_destory_menu_item(menu_ptr->head_item);
    //  _l_destory_menu_item(menu_ptr->cur_item);

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
    // Use Initial_flag to refresh linked list's head_item,avoiding that when go to main interface and return to menu,the selector can go up from Menu_Testing1
    if (menu_ptr->Initial_flag == 1)
    {
        menu_ptr->head_item = menu_ptr->cur_item = item_ptr;
        menu_ptr->Initial_flag = 0;
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
 * @brief Draw all of the items
 *
 * @param menu_ptr
 * @param order draw direction
 *                             0: forward
 *                             1: backward
 */
static void _r_menu_draw_items(jgfx_menu_ptr menu_ptr, uint8_t order)
{
    uint8_t i;
    jgfx_menu_item_ptr item = menu_ptr->cur_item;

    if (!order)
        i = 0;
    else
        i = menu_ptr->item_show_num;

    while (item != NULL)
    {
        if (!order)
        {
            _r_menu_draw_item(menu_ptr, item, JGFX_MENU_ITEM_STATUS_UNSELECTED, i);
            item = item->item_next;
            i++;
            if (i >= menu_ptr->item_show_num)
                break;
        }
        else
        {
            _r_menu_draw_item(menu_ptr, item, JGFX_MENU_ITEM_STATUS_UNSELECTED, i - 1);
            item = item->item_pre;
            i--;
            if (i <= 0)
                break;
        }
    }
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

    if ((menu_ptr->index + 1) < menu_ptr->item_show_num)
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

    // jgfx_set_color_back_hex(0x0000);
    // jgfx_fill_react(32 + menu_ptr->menu_x,
    //                 menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
    //                 menu_ptr->menu_width - 5, menu_ptr->menu_item_height + 5);
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

    if (status == JGFX_MENU_ITEM_STATUS_CLICKED)
    {

        /** Draw item box */
        jgfx_draw_rect(32 + menu_ptr->menu_x,
                       menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
                       32 + menu_ptr->menu_x + menu_ptr->menu_width,
                       menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index + menu_ptr->menu_item_height);
    }

    // jgfx_draw_line(36 + menu_ptr->menu_x,
    //                menu_ptr->menu_item_height + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
    //                menu_ptr->menu_width + 30,
    //                menu_ptr->menu_item_height + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index);
}

static void _r_clear_item_area(jgfx_menu_ptr menu_ptr)
{
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(35 + menu_ptr->menu_x, menu_ptr->menu_y + menu_ptr->menu_title_height + 5, menu_ptr->menu_width - 5, menu_ptr->menu_height - menu_ptr->menu_title_height - (menu_ptr->menu_divide_height * menu_ptr->item_show_num) - 10);
}

/**
 * @brief show submenu only for Menu_Testing1 now.
 *
 * @param menu_ptr menu pointer
 * @param index current selector
 * @return uint8_t
 */
void menu_item_show(jgfx_menu_ptr menu_ptr, uint8_t item_num, submenu_item_ptr submenu_ptr, ...)
{
    uint8_t index = 1, i;
    uint8_t *data;
    submenu_init(menu_ptr, submenu_ptr, item_num);
    // draw item menu's title in the first line
    _r_clear_item_area(menu_ptr);
    _r_menu_draw_item(menu_ptr, menu_ptr->cur_item, JGFX_MENU_ITEM_STATUS_UNSELECTED, 0);

    // draw items
    va_list pointer;
    va_start(pointer, submenu_ptr);
    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    for (i = 0; i < submenu_ptr->itemlist_num; i++)
    {
        data = va_arg(pointer, uint8_t *);
        if (data == NULL)
            return;
        submenu_ptr->item_name[i] = data;
        jgfx_draw_text_en(36 + menu_ptr->menu_x,
                          submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (i + 1),
                          submenu_ptr->item_name[i]);
    }
    va_end(pointer);

    // draw selector
    jgfx_set_color_hex(0x0000);
    jgfx_set_color_back_hex(0xFFFF);
    jgfx_draw_text_en(36 + menu_ptr->menu_x,
                      submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 1,
                      submenu_ptr->item_name[0]);

    // redraw the line in the botton to fix some error
    jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 31, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    submenu_cb(menu_ptr, submenu_ptr);
}

/**
 * @brief set defalut parameters for the submenu
 *
 * @param menu_ptr menu pointer
 * @param submenu_ptr submenu pointer
 * @param num the number of sentences submenu should receive
 */
static void submenu_init(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr, uint8_t num)
{
    submenu_ptr->line_height = menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y;
    submenu_ptr->itemlist_num = num;
}

/**
 * @brief Loop in submenu and wait for key-press order
 *
 * @param menu_ptr menu pointer
 * @param index current selector
 */
static void submenu_cb(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr)
{
    uint8_t item_num = submenu_ptr->itemlist_num, cur_item = 1;
    while (1)
    {
        key_map_t key = key_get();
        if (key != NULL)
        {
            if (key == 1)
            {
                // return cur_item;
            }
            else if (key == 2 && cur_item != 1)
            {
                {
                    jgfx_set_color_hex(0xFFFF);
                    jgfx_set_color_back_hex(0x0000);
                    jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (cur_item), submenu_ptr->item_name[cur_item - 1]);
                    jgfx_set_color_hex(0x0000);
                    jgfx_set_color_back_hex(0xFFFF);
                    jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (cur_item - 1), submenu_ptr->item_name[cur_item - 2]);
                    while (key_get() != KEY_MAP_NONE)
                        ;
                    delay_1us(10);
                    cur_item -= 1;
                }
            }
            else if (key == 3 && cur_item != submenu_ptr->itemlist_num)
            {
                jgfx_set_color_hex(0xFFFF);
                jgfx_set_color_back_hex(0x0000);
                jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (cur_item), submenu_ptr->item_name[cur_item - 1]);
                jgfx_set_color_hex(0x0000);
                jgfx_set_color_back_hex(0xFFFF);
                jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (cur_item + 1), submenu_ptr->item_name[cur_item]);
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(10);
                cur_item += 1;
            }
            else if (key == 4)
            {
                jgfx_menu_show(menu_ptr);
                menu_ptr->status = JGFX_MENU_STATUS_SELECTED;
                break;
            }
        }
    }
}
