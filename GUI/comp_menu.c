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

extern SemaphoreHandle_t xMainChannelTalking, xMainChannelListening, xMainChannelInput, xMainChannelDTMFSending;
extern SemaphoreHandle_t xMainListeningRender, xMainListeningUnrender, xMainChannelDraw;
extern SemaphoreHandle_t xChannelScan;
extern uint8_t channel_A[];
extern uint8_t channel_B[];


void _r_clear_item_area(jgfx_menu_ptr menu_ptr)
{
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(32 + menu_ptr->menu_x, menu_ptr->menu_y + menu_ptr->menu_title_height + 5, menu_ptr->menu_width, menu_ptr->menu_height - menu_ptr->menu_title_height - (menu_ptr->menu_divide_height * menu_ptr->item_show_num) - 10);
}

/**
 * @brief Only displayed at startup
 *
 */
void Startup_display(void)
{
    jgfx_clear_screen();
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    jgfx_draw_text_en(32 + DISPLAY_W / 2 - (7 * 8 / 2), DISPLAY_H / 2 - 20, "A36Plus");
    jgfx_draw_text_en(32 + DISPLAY_W / 2 - (4 * 8 / 2), DISPLAY_H / 2 + 10, "RTOS");
    vTaskDelay(5000);
}

/**
 * @brief Init menu
 *
 * @param menu_ptr menu pointer
 */
void jgfx_menu_init(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr main_channel)
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
    menu_ptr->page = 0;
    menu_ptr->channel_ptr = main_channel;
    menu_ptr->menu_draw_completed = FALSE;

    menu_item_param_init(menu_ptr, main_channel);
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
    static jgfx_menu_item_ptr cur_item;
    static bool menu_index_finish = FALSE;
    /** Init item selected */
    menu_ptr->item_show_num = (menu_ptr->menu_height - (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0)) / (menu_ptr->menu_item_height + menu_ptr->menu_divide_height);
    menu_ptr->cur_item = menu_ptr->head_item;
    if(menu_index_finish == FALSE)
    {
        cur_item = menu_ptr->cur_item;
        menu_index_finish = TRUE;
    }
    menu_ptr->index = 0;

    /** Draw menu box*/
    // jgfx_set_color_hex(0xFFFF);
    // jgfx_set_color_back_hex(0x0000);
    // jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 31, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    /** Show title if use title*/
    if (menu_ptr->menu_use_tiltle)
    {
        jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 30, menu_ptr->menu_y + menu_ptr->menu_title_height + 2);
        jgfx_set_font(JGFX_FONT_EN_8X16);
        jgfx_draw_text_en(32 + menu_ptr->menu_x + (menu_ptr->menu_width / 2) - (jgfx_measure_text_width(menu_ptr->menu_title) / 2), menu_ptr->menu_y + 1, menu_ptr->menu_title);
#ifdef USE_FONT_LOADED_IN_MCU
        delay_1us(3);
        jgfx_draw_img(32 + 5, 5, 10, 10, (menu_ptr->channel_ptr->channel == 1 ? (channel_A) : (channel_B)));
#else
        uint32_t addr = menu_ptr->channel_ptr->channel == 1 ? (FLASH_ICON_CHANNELA_ADDR) : (FLASH_ICON_CHANNELB_ADDR);
        jgfx_draw_img_byaddr(32 + 5, 5, 10, 10, addr);
#endif
    }

    /** Draw items */
    vTaskDelay(15);
    _r_menu_draw_items(menu_ptr, 0);

    /** Draw selector */
    _r_menu_draw_selector(menu_ptr);

    menu_ptr->menu_draw_completed = TRUE;
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
        menu_ptr->page += 1;
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
        menu_ptr->page -= 1;
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
    while (key_get() != KEY_MAP_NONE)
        ;
    delay_1us(10);
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
 * @brief Prevent miscontact caused by excessive speed
 *
 */
void key_press_delay(void)
{
    uint16_t timecount = 10000;
    while (timecount--)
    {
        if (key_get() == KEY_MAP_NONE)
            break;
    }
}

/**
 * @brief Index menu item
 *
 * @param menu_ptr menu pointer
 * @param index item index
 */
bool jgfx_menu_index(jgfx_menu_ptr menu_ptr, uint8_t index)
{
    if (menu_ptr == NULL || index > (menu_ptr->item_size))
        return FALSE;

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

    return TRUE;
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
 * @brief show submenu when click from menu items
 *
 * @param menu_ptr menu pointer
 * @param index current selector
 * @return uint8_t 0: no execute  x: param of list of submenu
 */
uint8_t submenu_item_show(jgfx_menu_ptr menu_ptr, uint8_t item_num, submenu_item_ptr submenu_ptr, ...)
{
    uint8_t index = 1, i;
    uint8_t *data;
    // Clean index num on the top right corner
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);

    submenu_init(menu_ptr, submenu_ptr, item_num);
    // draw item menu's title in the first line
    _r_clear_item_area(menu_ptr);
    jgfx_set_color_hex(0xFFFF);
    _r_menu_draw_item(menu_ptr, menu_ptr->cur_item, JGFX_MENU_ITEM_STATUS_UNSELECTED, 0);

    // Init items
    va_list pointer;
    va_start(pointer, submenu_ptr);
    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    for (i = 0; i < submenu_ptr->itemlist_num; i++)
    {
        data = va_arg(pointer, uint8_t *);
        if (data == NULL)
            return -1;
        submenu_ptr->item_name[i] = data;
    }
    va_end(pointer);

    // Draw items
    submenu_items_refresh(menu_ptr, submenu_ptr);

    // // redraw the line in the botton to fix some error
    // jgfx_set_color_hex(0xFFFF);
    // jgfx_set_color_back_hex(0x0000);
    // jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 31, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    return submenu_cb(menu_ptr, submenu_ptr);
}

uint8_t submenu_item_show_with_array(jgfx_menu_ptr menu_ptr, uint8_t item_num, submenu_item_ptr submenu_ptr, uint8_t string[][MENU_NAME_LEN_LIM])
{
    uint8_t index = 1, i;
    uint8_t *data;
    // Clean index num on the top right corner
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);

    submenu_init(menu_ptr, submenu_ptr, item_num);
    // draw item menu's title in the first line
    _r_clear_item_area(menu_ptr);
    jgfx_set_color_hex(0xFFFF);
    _r_menu_draw_item(menu_ptr, menu_ptr->cur_item, JGFX_MENU_ITEM_STATUS_UNSELECTED, 0);

    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    for (i = 0; i < submenu_ptr->itemlist_num; i++)
    {
        submenu_ptr->item_name[i] = string[i];
    }

    // Draw items
    submenu_items_refresh(menu_ptr, submenu_ptr);

    // redraw the line in the botton to fix some error
    // jgfx_set_color_hex(0xFFFF);
    // jgfx_set_color_back_hex(0x0000);
    // jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 31, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    return submenu_cb(menu_ptr, submenu_ptr);
}

/**
 * @brief Draw line box for menu item after clicking
 * 
 * @param menu_ptr Menu pointer
 * @param string Menu item's name drawed on the left side without param
 * @param index position counted from 0
 */
void render_submenu_with_param(jgfx_menu_ptr menu_ptr, uint8_t *string, uint8_t index)
{

    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    
     jgfx_draw_text_en(36 + menu_ptr->menu_x,
                      menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
                      string);

    jgfx_draw_rect(32 + menu_ptr->menu_x,
                   menu_ptr->menu_item_height / 2 - 5 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
                   32 + menu_ptr->menu_x + menu_ptr->menu_width,
                   menu_ptr->menu_item_height / 2 - 5 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index + menu_ptr->menu_item_height);
}

/**
 * @brief By means of cur_item' id  to draw index number on the top right corner.
 * @param menu_ptr menu pointer
 */
void index_num_display(jgfx_menu_ptr menu_ptr)
{
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);
    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    jgfx_set_font(JGFX_FONT_EN_8X16);
    int i = 0;
    char count_num_str[3];
    snprintf(count_num_str, sizeof(count_num_str), "%02d", menu_ptr->cur_item->item_id + 1);
    count_num_str[2] = '\0';
    jgfx_draw_text_en(130, 3, count_num_str);

    switch (menu_ptr->menu_item[menu_ptr->cur_item->item_id].show_channel_pic)
    {
    case 1:

#ifdef USE_FONT_LOADED_IN_MCU
        delay_1us(3);
        jgfx_draw_img(32 + 5, 5, 10, 10, (menu_ptr->channel_ptr->channel == 1 ? (channel_A) : (channel_B)));
#else
        uint32_t addr = menu_ptr->channel_ptr->channel == 1 ? (FLASH_ICON_CHANNELA_ADDR) : (FLASH_ICON_CHANNELB_ADDR);
        jgfx_draw_img_byaddr(32 + 5, 5, 10, 10, addr);
#endif
        break;
    case 0:
        jgfx_fill_react(32 + 5, 5, 10, 10);
        break;
    }

}

void menu_item_param_init(jgfx_menu_ptr menu_ptr,ui_main_channel_ptr main_channel)
{
    static menu_list_t menu_item[] =
    {
        {"Radio Setting",   1,          MENU_RADIO_SETTING  },
        {"SQL",             0,          MENU_SQUELCH        },
        {"Ch Save",         1,          MENU_CHANNEL_SAVE   },
        {"Ch Select",       1,          MENU_CHANNEL_SELECT },
        {"Ch Delete",       1,          MENU_CHANNEL_DELETE },
        {"Display",         0,          MENU_DISPLAY        },
        {"Banks",           0,          MENU_BANKS          },
        {"B/W",             0,          MENU_BANDWIDTH      },
        {"Button",          0,          MENU_BUTTON         },
        {"DTMF",            0,          MENU_DTMF           },
    };

    static bool initial_finished = FALSE;
    uint8_t menu_item_size = sizeof(menu_item) / sizeof(menu_item[0]);
    uint8_t index, i;

    if(initial_finished == TRUE)
        return;

    for(i = 0; i < sizeof(menu_item)/sizeof(menu_item[0]); i++)
    {
        menu_ptr->menu_item[i] = menu_item[i];
    }

#ifdef SUQELCH_MENU_SHOW_WITH_PARAM
    //Init SQL menu name
    uint8_t sql_val = main_channel->cur_channel->sql;
    index = 0;
    while(index < menu_item_size)
    {
        if(menu_ptr->menu_item[index].menu_id == MENU_SQUELCH)
            break;
        index++;
    }
    i = 0;
    while(menu_ptr->menu_item[index].menu_name[i] != '\0')
        i++;
    
    while(i < MENU_NAME_LEN_LIM - 1)
        menu_ptr->menu_item[index].menu_name[i++] = ' ';
    
    menu_ptr->menu_item[index].menu_name[i] = '\0';

    sprintf(&menu_ptr->menu_item[index].menu_name[i - 1], "%d",sql_val);
#endif


    initial_finished = TRUE;
    // channel_ptr->cur_channel->sql = param - 1;

}

/**
 * @brief Reset corner_ptr's parameters
 *
 * @param corner_ptr corner pointer
 */
void corner_index_init(corner_index_num_ptr corner_ptr)
{
    corner_ptr->index_num1 = 0;
    corner_ptr->index_num2 = 0;
    corner_ptr->index_jump_count1 = 0;
    corner_ptr->index_jump_count2 = 0;
}

/**
 * @brief Reset the jump by counting down the refresh time,also flicker continuously during the countdown of the jump
 *
 * @param menu_ptr menu pointer
 * @param corner_ptr corner pointer
 */
void corner_index_refresh(jgfx_menu_ptr menu_ptr, corner_index_num_ptr corner_ptr)
{
    if (corner_ptr->index_jump_count1 == 0 && corner_ptr->index_jump_count2 == 0)
    {
        corner_ptr->index_num1 = corner_ptr->index_num2 = 0;
        return;
    }
    else
    {
        corner_ptr->index_jump_count1--;
        if (corner_ptr->index_jump_count1 == 0 && corner_ptr->index_jump_count2 != 0)
        {
            corner_ptr->index_jump_count1 = 30;
            corner_ptr->index_jump_count2--;
            if (corner_ptr->index_jump_count2 % 2 != 0)
            {
                jgfx_set_color_back_hex(0x0000);
                jgfx_fill_react(130, 5, 16, 16);
            }
            else
            {
                index_num_display(menu_ptr);
            }
        }
    }
}

/**
 * @brief Flicker continuously during the countdown of the jump
 *
 * @param menu_ptr menu pointer
 * @param corner_ptr corner pointer
 */
void corner_index_flicker(jgfx_menu_ptr menu_ptr, corner_index_num_ptr corner_ptr)
{
    if (corner_ptr->index_jump_count1 == 0 && corner_ptr->index_jump_count2 == 0)
        return;

    if (corner_ptr->index_jump_count2 % 2 != 0)
    {
        jgfx_set_color_back_hex(0x0000);
        jgfx_fill_react(130, 5, 16, 16);
    }
    else
    {
        index_num_display(menu_ptr);
    }
}

/**
 * @brief Use the dual cache mechanism to achieve page jumping.
 *        The arguments index_jump_count1 and index_jump_count2 are used to count backwards.
 *
 * @param corner_ptr corner pointer
 * @param menu_ptr menu pointer
 * @param key The key value passed in
 * @return uint8_t Page needed to jump
 */
uint8_t index_num_cb(corner_index_num_ptr corner_ptr, jgfx_menu_ptr menu_ptr, uint8_t key)
{
    // Give a value to index_num1 when the first time trigger this func.
    if (corner_ptr->index_jump_count1 == 0 && corner_ptr->index_jump_count2 == 0)
    {
        corner_ptr->index_jump_count1 = 30;
        corner_ptr->index_jump_count2 = 6;
        corner_ptr->index_num1 = key;
        // Check whether the flicker can be triggered
        if (corner_ptr->index_num1 > menu_ptr->item_size / 10)
        {
            corner_ptr->index_jump_count1 = corner_ptr->index_jump_count2 = 0;
        }
        return corner_ptr->index_num1;
    }
    // Give a value to index_num1 when the second time trigger this func and clear countdown timer.
    else
    {
        delay_1us(100);
        corner_ptr->index_num2 = key;
        // check whether the combination value out of zone
        if (corner_ptr->index_num1 * 10 + corner_ptr->index_num2 > menu_ptr->item_size)
        {
            corner_ptr->index_jump_count1 = 30;
            corner_ptr->index_jump_count2 = 6;
            corner_ptr->index_num1 = key;
            corner_ptr->index_num2 = 0;

            if (corner_ptr->index_num1 > menu_ptr->item_size / 10)
            {
                corner_ptr->index_jump_count1 = corner_ptr->index_jump_count2 = 0;
            }

            return corner_ptr->index_num1;
        }
        corner_ptr->index_jump_count1 = corner_ptr->index_jump_count2 = 0;
        return corner_ptr->index_num1 * 10 + corner_ptr->index_num2;
    }
}





/**
 * @brief main channel change
 *
 * @param channel_ptr channel pointer
 * @param step stepped frequency
 */
void radio_channel_change(ui_main_channel_ptr channel_ptr, uint8_t step)
{
    if (step == 0)
        return;
    channel_ptr->cur_index = step;
    channel_ptr->step = channel_ptr->ch_pra[step - 1];
}

/**
 * @brief draw frames when PTT press in main interface
 *
 */
void main_channel_speaking(ui_main_channel_ptr channel_ptr)
{
    if (channel_ptr->channel == 1)
    {
        // jgfx_draw_line(32, DISPLAY_H / 2 + 10, DISPLAY_W + 32, DISPLAY_W / 2 + 10);
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLUE);
        jgfx_fill_react(32, DISPLAY_H / 2 + 11, DISPLAY_W, DISPLAY_H / 2);
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_draw_text_en(76, DISPLAY_H / 2 + 33, "A  TX");
    }
    else
    {
        jgfx_set_color_hex(JGFXF_COLOR_BLACK);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLUE);
        jgfx_fill_react(32, 10, DISPLAY_W, DISPLAY_H / 2);
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_draw_text_en(76, DISPLAY_H / 2 - 23, "B  TX");
    }
}

/**
 * @brief execute a serials operations while PTT pressed
 *
 * @param channel_ptr channel pointer
 */
void main_PTT_transmit(ui_main_channel_ptr channel_ptr)
{
    uint16_t reg_temp;

    channel_offset_preload(channel_ptr);
    draw_channel();
    reg_temp = bk4819_read_reg(BK4819_REG_36);
#ifndef LOAD_IN_A36PLUS
    bk4819_Tx_Power(channel_ptr->cur_channel->power);
#else 
    bk4819_setTxPower(channel_ptr->cur_channel->power, channel_ptr->cur_channel->frequency);
#endif
    TxAmplifier_enable(channel_ptr);

    bk4819_set_freq(channel_ptr->cur_channel->frequency);
    bk4819_tx_on();
    bk4819_TxCTDCSS_set_auto(channel_ptr);
    draw_status_tx();

    main_channel_speaking(channel_ptr);

    if(channel_ptr->ANI_enable == TRUE)
        FSK_Info_TX();

    while (key_get() != 0)
        ;
    
    bk4819_tx_off();
    TxAmplifier_disable();
    bk4819_write_reg(BK4819_REG_36, reg_temp);
    channel_offset_unload(channel_ptr);
}

/**
 * @brief draw window ready to send DTMF
 *
 * @param channel_ptr
 */
void main_DTMF_init(ui_main_channel_ptr channel_ptr)
{
    uint8_t str[15];
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    jgfx_fill_react(32 + 3, channel_ptr->cur_channel == &channel_ptr->channel_1 ? (channel_ptr->block_height1 + 1) : (channel_ptr->block_height2 + 5), 128, 36);

    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    if (channel_ptr->cur_channel == &channel_ptr->channel_1)
    {
        jgfx_draw_text_en(35, DISPLAY_H / 10 + 30, "DTMF");
        jgfx_draw_text_en(72, DISPLAY_H / 10 + 30, "----------");
    }
    else if (channel_ptr->cur_channel == &channel_ptr->channel_2)
    {
        jgfx_draw_text_en(35, DISPLAY_H / 2 + 35, "DTMF");
        jgfx_draw_text_en(72, DISPLAY_H / 2 + 35, "----------");
    }
}



/**
 *@brief Accept value from keyboard press.If not empty,press PTT to send DTMF
 *
 * @param channel_ptr channel_pointer
 */
void main_DTMF_input(ui_main_channel_ptr channel_ptr)
{
    
    // channel_ptr->PF1 = param;
    uint8_t key, key1, offset, StringLength = 0;
    uint8_t x, y;
    x = 72;
    y = channel_ptr->cur_channel == &channel_ptr->channel_1 ? (DISPLAY_H / 10 + 30) : (DISPLAY_H / 2 + 35);
    char code;
    char string[11];
    strncpy(string, "----------", sizeof(string) - 1);

    while (key_get())
        ;
    while (1)
    {
        vTaskDelay(10);
        key = key_get();
        if (key == KEY_MAP_NONE)
            continue;
        if (key == 18)
        {
            if(StringLength == 0)
                break;

            char DTMF_String[15];
            for(int i = 0;i < StringLength; i++)
            {
                DTMF_String[i] = string[i];
            }
            DTMF_String[StringLength] = '\0';

            bk4819_set_freq(channel_ptr->cur_channel->frequency);
            Send_DTMF_String(DTMF_String);
            // Show param
            vTaskDelay(2000);
            break;
        }
        if (key == 19 && channel_ptr->PF1 == 1)
        {
            if (StringLength > 0)
            {
                string[StringLength - 1] = '-';
                StringLength -= 1;
                jgfx_fill_react(x, y, 80, 16);
                jgfx_draw_text_en(x, y, string);
                while (key_get())
                    ;
            }
            else if (StringLength == 0)
                break;
            // exit or sub
        }
        // if (key == 20 && channel_ptr->PF2 == 1)
        // {
        //     // exit or sub
        // }

        if (StringLength == 10)
            continue;

        if (key == 0 || key > 16)
            continue;
        key1 = KEY_GET_NUM(key);

        if (key1 < 10)
            code = 48 + key1;

        else if (key1 >= 10)
        {
            if (key >= 1 && key <= 4)
            {
                code = 64 + key;
            }
            else if (key == 8)
                code = '*';
            else if (key == 16)
                code = '#';
        }

        if (StringLength < 10)
            string[StringLength + 1] = string[StringLength];
        string[StringLength] = code;

        jgfx_fill_react(x, y, 80, 16);
        jgfx_draw_text_en(x, y, string);
        StringLength += 1;
        while (key_get())
            ;
            
    }
}

/**
 * @brief Accept DTMF code from keyboard press
 * 
 * @param channel_ptr channel pointer
 * @param Direction 0: DOWN   1: UP
 */
void main_DTMF_input_MENU(ui_main_channel_ptr channel_ptr,uint8_t Direction)
{
    
    // channel_ptr->PF1 = param;
    uint8_t key, key1, offset, StringLength = 0;
    uint8_t x, y;
    x = 72;
    y = channel_ptr->cur_channel == &channel_ptr->channel_1 ? (DISPLAY_H / 10 + 30) : (DISPLAY_H / 2 + 35);
    char code;
    char string[11];
    strncpy(string, "----------", sizeof(string) - 1);

    while (key_get())
        ;
    while (1)
    {
        vTaskDelay(10);
        key = key_get();
        if (key == KEY_MAP_NONE)
            continue;
        if (key == 20)
        {
            if(StringLength == 0)
                break;

            uint8_t *DTMF_String;

            switch (Direction)
            {
            case 0:
                DTMF_String = channel_ptr->DTMF_DownCode;
                break;
            case 1:
                DTMF_String = channel_ptr->DTMF_UPCode;
                break;
            default:
                break;
            }

            for(int i = 0;i < StringLength; i++)
            {
                DTMF_String[i] = string[i];
            }
            DTMF_String[StringLength] = '\0';

            break;
        }
        if (key == 19 && channel_ptr->PF1 == 1)
        {
            if (StringLength > 0)
            {
                string[StringLength - 1] = '-';
                StringLength -= 1;
                jgfx_fill_react(x, y, 80, 16);
                jgfx_draw_text_en(x, y, string);
                while (key_get())
                    ;
            }
            else if (StringLength == 0)
                break;
            // exit or sub
        }
        // if (key == 20 && channel_ptr->PF2 == 1)
        // {
        //     // exit or sub
        // }

        if (StringLength == 10)
            continue;

        if (key == 0 || key > 16)
            continue;
        key1 = KEY_GET_NUM(key);

        if (key1 < 10)
            code = 48 + key1;

        else if (key1 >= 10)
        {
            if (key >= 1 && key <= 4)
            {
                code = 64 + key;
            }
            else if (key == 8)
                code = '*';
            else if (key == 16)
                code = '#';
        }

        if (StringLength < 10)
            string[StringLength + 1] = string[StringLength];
        string[StringLength] = code;

        jgfx_fill_react(x, y, 80, 16);
        jgfx_draw_text_en(x, y, string);
        StringLength += 1;
        while (key_get())
            ;
            
    }
}

/**
 * @brief detect whether in the input state and flicker the selected channel number
 *
 * @param channel_ptr channel pointer
 * @param state input state of ui_main
 */
void channel_input_flicker(ui_main_channel_ptr channel_ptr, uint8_t state)
{
    if (state == 1)
    {
        channel_ptr->flash_count_num1 -= 1;
        if (channel_ptr->flash_count_num1 == 0)
        {
            channel_ptr->flash_count_num1 = 3000;
            channel_ptr->flash_count_num2 = !channel_ptr->flash_count_num2;
            if (channel_ptr->flash_count_num2 == 1)
            {
                draw_channel();
            }
            else
            {
                jgfx_set_color_back_hex(0x0000);
                jgfx_fill_react(32 + 3, channel_ptr->cur_channel == &channel_ptr->channel_1 ? (channel_ptr->block_height1 + 1) : (channel_ptr->block_height2 + 5), 128, 26);
            }
        }
    }
}

/**
 * @brief store channel value when changed
 *
 * @param channel_ptr channel pointer
 *
 */
void channel_store(ui_main_channel_ptr channel_ptr)
{
    if (channel_ptr->channel_changed == FALSE)
        return;
    channel_ptr->channel_changed = FALSE;
    if (channel_ptr->cur_channel == &channel_ptr->channel_1)
        channel_A_store(channel_ptr->channel_1.frequency);
    else if (channel_ptr->cur_channel == &channel_ptr->channel_2)
        channel_B_store(channel_ptr->channel_2.frequency);
}



/**
 * @brief Redraw the main menu when returning from the submenu,making sure that the selector points to the original location
 *
 * @param menu_ptr menu pointer
 */
void return_to_menu(jgfx_menu_ptr menu_ptr)
{

    _r_clear_item_area(menu_ptr);
    jgfx_menu_item_ptr cur_item = menu_ptr->cur_item;
    jgfx_menu_item_ptr tem_item = cur_item;
    uint8_t i = 0;
    while ((tem_item->item_id) % 5 != 0)
    {
        i += 1;
        tem_item = tem_item->item_pre;
    }
    menu_ptr->cur_item = tem_item;
    _r_menu_draw_items(menu_ptr, 0);
    menu_ptr->cur_item = cur_item;
    _r_menu_draw_selector(menu_ptr);
}

void Brightness_init(Brightness_setting_ptr bri_ptr)
{

    bri_ptr->bri_pra = brightness_param;
    bri_ptr->bri_val = bri_ptr->bri_pra;

#ifdef BRIGHTNESS_MENU_SHOW_WITH_PARAM
    bri_ptr->cur_bri = 15;
    bri_ptr->bri_val += 15;
#else 
    bri_ptr->cur_bri = 3;
    bri_ptr->bri_val += 3;
#endif

}

void Brightness_change(Brightness_setting_ptr bri_ptr, uint8_t bri_num)
{
    if (bri_num == 0)
        return;
    bri_ptr->cur_bri = bri_num;
    bri_ptr->bri_val = &bri_ptr->bri_pra[bri_num];
    // printf("%d", bri_ptr->bri_val);
    timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, *bri_ptr->bri_val);
}

void Display_Timer_Init(Display_Timer_ptr Timer_ptr)
{
    // Init for only once in the hole program
    // if (Timer_ptr->Timer_init_flag == TRUE)
    //     return;
    Timer_ptr->index = 3;
    Timer_ptr->Timer_limit = 33; // count 33 times for one second
    Timer_ptr->Timer_count = 0;
    Timer_ptr->Second_count = 0;
    Timer_ptr->screen_off = 30; // 30 seconds
    Timer_ptr->Timer_init_flag = TRUE;
    Timer_ptr->Tim_pra = Display_Timer_param;
    Timer_ptr->Tim_val = Timer_ptr->Tim_pra;
    Timer_ptr->Tim_val += 1;
}

void Display_Timer_count(Display_Timer_ptr Timer_ptr)
{
    if (Timer_ptr->Timer_init_flag == FALSE || Timer_ptr->screen_off == 999)
        return;
    timer_interrupt_disable(TIMER16, TIMER_INT_UP);
    if (Timer_ptr->Timer_count >= Timer_ptr->Timer_limit)
    {
        Timer_ptr->Second_count += 1;
        Timer_ptr->Timer_count = 0;
        // printf("refresh now!\n");
        if (Timer_ptr->Second_count >= Timer_ptr->screen_off)
        {
            timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, 5);
        }
    }
    timer_interrupt_enable(TIMER16, TIMER_INT_UP);
}

void Display_Timer_change(Display_Timer_ptr Timer_ptr, uint8_t Tim_num)
{
    if (Tim_num == 0)
        return;
    Timer_ptr->index = Tim_num;
    Timer_ptr->screen_off = Timer_ptr->Tim_pra[Tim_num - 1];
    // Display_Timer.screen_off = 10;
}

/**
 * @brief Light up the screen when trigger any key press
 *
 * @param Brightness_ptr Brightness pointer
 * @param Timer_ptr     Timer pointer
 */
void wakeup_screen(Brightness_setting_ptr Brightness_ptr, Display_Timer_ptr Timer_ptr)
{
    // waiting for setting back true brightness by using  brightness_ptr here
    timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, *Brightness_ptr->bri_val);
    Timer_ptr->Second_count = 0;
    Timer_ptr->Timer_count = 0;
}

/**
 * @brief Create an input window for add channel freatue
 *
 * @param menu_ptr
 */
void input_window_init(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    uint8_t str[20];
    _r_clear_item_area(menu_ptr);
    jgfx_set_font(JGFX_FONT_EN_16X32);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

    sprintf(str, "%04d", channel_ptr->cur_channel->offset / 1000);
    jgfx_draw_text_en(32 + 3, DISPLAY_H / 10 + 25, str);
    sprintf(str, "%03d", channel_ptr->cur_channel->offset % 1000);
    jgfx_draw_text_en(32 + 3 + 4 * 16 + 5, DISPLAY_H / 10 + 25, str);

    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_draw_text_en(32 + 3 + 3 * 16 + 14, DISPLAY_H / 10 + 25 + 13, ".");
}

/**
 * @brief set subchannel offset
 *
 * @param channel_ptr channel pointer
 */
void offset_setting(ui_main_channel_ptr channel_ptr)
{
    uint32_t input_channel = 0;
    while (1)
    {
        key_map_t key = key_get();
        if (key != KEY_MAP_NONE)
        {
            if (key == 1)
            {
                if (input_channel == 0)
                {

                    break;
                }
                else
                {
                    while (!(input_channel / (1000)))
                        input_channel *= 10;
                    channel_offset_draw(input_channel);
                    delay_1ms(1); // as a timer for confirm setting
                    channel_ptr->cur_channel->offset = input_channel;
                    // set offset direction to add as default
                    channel_ptr->cur_channel->direction = ADDITION;
                    break;

                    // draw input success
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
            }
            else if (key == 4)
            {
                if (input_channel != 0)
                {
                    input_channel /= 10;
                    channel_offset_draw(input_channel);
                }
                else
                {
                    // draw input cancel
                    // draw_info(ADD_OFFSET_CANCEL);
                    break;
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
            }
            // detect number key press
            else if (key != 2 && key != 3 && key != 8 && key != 16 && key != 17 && key != 18 && key != 19 && key != 20)
            {
                input_channel = input_channel * 10 + KEY_GET_NUM(key);
                if (input_channel > (1300 * 1000))
                {
                    input_channel = 0;
                    draw_info(ADD_OFFSET_ERROR);
                }
                channel_offset_draw(input_channel);
                if (input_channel >= 200 * 1000)
                {
                    break;
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
            }
        }
        delay_1us(100);
    }
}

/**
 * @brief initialize the offset input window
 *
 * @param input_channel
 */
void channel_offset_draw(uint32_t input_channel)
{
    uint8_t input_window[8];

    // jgfx_draw_text_en(32 + 3, DISPLAY_H / 10 + 25, "----");
    // jgfx_draw_text_en(32 + 3 + 3 * 16 + 14, DISPLAY_H / 10 + 25 + 13, ".");
    // jgfx_draw_text_en(32 + 3 + 4 * 16 + 5, DISPLAY_H / 10 + 25, "---");
    jgfx_set_font(JGFX_FONT_EN_16X32);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
    jgfx_fill_react(35, DISPLAY_H / 10 + 23, DISPLAY_W - 5, 34);

    sprintf(input_window, "%03d", input_channel / 1000);
    jgfx_draw_text_en(32 + 3 + 16, DISPLAY_H / 10 + 25, input_window);

    // draw point
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_draw_text_en(32 + 3 + 3 * 16 + 14, DISPLAY_H / 10 + 25 + 13, ".");

    // draw khz
    jgfx_set_font(JGFX_FONT_EN_16X32);
    sprintf(input_window, "%03d", input_channel % 1000);
    jgfx_draw_text_en(32 + 3 + 4 * 16 + 5, DISPLAY_H / 10 + 25, input_window);
}

/**
 * @brief Pop-up information box just for offset input temporarily
 *
 * @param status offset input status
 */
void draw_info(jgfx_add_channel_status_t status)
{
    jgfx_set_font(JGFX_FONT_EN_8X16);
    if (status == ADD_OFFSET_CANCEL)
    {
        jgfx_set_color_hex(JGFXF_COLOR_CYAN);
        jgfx_set_color_back_hex(JGFXF_COLOR_YELLOW);
        jgfx_draw_text_en(32 + 8, DISPLAY_H - 35, "  Set cancel  ");
        delay_1ms(2);
    }
    if (status == ADD_OFFSET_ERROR)
    {
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_set_color_back_hex(JGFXF_COLOR_RED);
        jgfx_draw_text_en(32 + 8, DISPLAY_H - 35, " Offset error ");
        delay_1ms(1);
        jgfx_set_color_hex(JGFXF_COLOR_WHITE);
        jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(32 + 7, DISPLAY_H - 34, 14 * 8, 18);
    }
}

/**
 * @brief set offset direction in submenu
 *
 * @param channel_ptr channel pointer
 * @param param nothing sepctial,depending on submenu function callback
 */
void offset_direction(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param == 0)
        return;
    switch (param)
    {
    case 1:
    {
        channel_ptr->cur_channel->offset = 0;
        channel_ptr->cur_channel->direction = OFF;
        break;
    }
    case 2:
        channel_ptr->cur_channel->direction = ADDITION;
        break;
    case 3:
        channel_ptr->cur_channel->direction = SUBTRACTION;
        break;
    }
}


/**
 * @brief set TX power parameter for radio_channel
 *
 * @param channel_ptr channel pointer
 * @param param refer from Tx_Power_t
 */
void TxPower_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param == 0)
        return;
    channel_ptr->TxPower_index = param;
    switch (param)
    {
    case 1:
        channel_ptr->cur_channel->power = TXP_HIGH;
        break;
    case 2:
        channel_ptr->cur_channel->power = TXP_MID;
        break;
    case 3:
        channel_ptr->cur_channel->power = TXP_LOW;
        break;
    }
}

/**
 * @brief scan frequency and CxCSS
 *
 * @param menu_ptr menu pointer
 * @param channel_ptr channel pointer
 */
void frequency_scan(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    uint8_t str[20];
    uint8_t mode = 0;
    uint32_t count = 100;
    uint32_t CTDCSS_result = 0;
    uint32_t frequency = NULL;

    bool FrequencyScan = FALSE;
    bool CTDCSSSCAN = FALSE;

    xSemaphoreTake(xChannelScan, portMAX_DELAY);
    // vTaskDelay(20);
    _r_clear_item_area(menu_ptr);
    jgfx_set_font(JGFX_FONT_EN_8X16);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

    jgfx_draw_text_en(40, 30, "fre: ");
    jgfx_draw_text_en(40, 60, "CXCSS: ");

    bk4819_init();
    BK4819_EnableFrequencyScan();
    // Scan_test();
    while (count--)
    {
        if (FrequencyScan == FALSE)
        {
            frequency = FrequencyScanResult(&mode, &CTDCSS_result);
            if (frequency != NULL)
            {
                FrequencyScan = TRUE;
                if (mode != 0)
                {
                    snprintf(str, sizeof(str), "%7d", frequency * 10);
                    jgfx_draw_text_en(80, 30, str);

                    if (mode == 1)
                        jgfx_draw_text_en(100, 60, "NF");
                    else if (mode == 2 || mode == 3)
                    {
                        snprintf(str, sizeof(str), "%6d", CTDCSS_result);
                        jgfx_draw_text_en(100, 60, str);
                        if (mode == 2)
                            jgfx_draw_text_en(40, 90, "mode : CDCSS");
                        if (mode == 3)
                            jgfx_draw_text_en(40, 90, "mode : CTCSS");
                    }
                    BK4819_DisableFrequencyScan();
                    break;
                }
            }
        }
        uint8_t key = key_get();
        if (key != NULL)
        {
            if (key == 4)
            {
                BK4819_DisableFrequencyScan();
                break;
            }
        }
    }

    if (count == 0)
    {
        jgfx_draw_text_en(80, 30, "T");
        jgfx_draw_text_en(100, 60, "T");
    }

    xSemaphoreGive(xChannelScan);
    BK4819_DisableFrequencyScan();
    vTaskDelay(5000);
}

/**
 * @brief change bandwidth setting for one of both subchannel
 *
 * @param channel_ptr channel pointer
 * @param param 1:width 2:narrow 3:narrower
 *
 */
void channel_bandwidth_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param <= 0 || param > 3)
        return;
    channel_ptr->cur_channel->channnel_bandwidth = param;
    bk4819_set_BandWidth(channel_ptr->cur_channel->channnel_bandwidth);
}

void channel_squelch_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param <= 0 || param > 10)
        return;
    channel_ptr->sql = param - 1;
    bk4819_Squelch_val_change(channel_ptr->sql);
}

/**
 * @brief preset offset before draw
 *
 * @param channel_ptr channel pointer
 */
void channel_offset_preload(ui_main_channel_ptr channel_ptr)
{
    if (channel_ptr->cur_channel->direction == OFF)
        return;
    if (channel_ptr->cur_channel->direction == ADDITION)
        channel_ptr->cur_channel->frequency = channel_ptr->cur_channel->frequency + channel_ptr->cur_channel->offset * 100;
    if (channel_ptr->cur_channel->direction == SUBTRACTION)
        channel_ptr->cur_channel->frequency = channel_ptr->cur_channel->frequency - channel_ptr->cur_channel->offset * 100;
}

/**
 * @brief unload offset after draw main_channel_speaking
 *
 * @param channel_ptr
 */
void channel_offset_unload(ui_main_channel_ptr channel_ptr)
{
    if (channel_ptr->cur_channel->direction == OFF)
        return;
    if (channel_ptr->cur_channel->direction == ADDITION)
        channel_ptr->cur_channel->frequency = channel_ptr->cur_channel->frequency - channel_ptr->cur_channel->offset * 100;
    if (channel_ptr->cur_channel->direction == SUBTRACTION)
        channel_ptr->cur_channel->frequency = channel_ptr->cur_channel->frequency + channel_ptr->cur_channel->offset * 100;
}

/**
 * @brief set TxCTCSS
 *
 * @param channel_ptr main channel pointer
 * @param prarm CTCSS status needed to be set,refer to CTCSS_param
 */
void channel_TxCTCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param == 0)
        return;
    if (param - 1 != 0)
    {
        bk4819_CTDCSS_enable(1);
        channel_ptr->cur_channel->Tx_CDCSS = 0;
    }
    bk4819_CTDCSS_set(0, CTCSS_param[param - 1]);
    channel_ptr->cur_channel->Tx_CTCSS = CTCSS_param[param - 1];
    channel_ptr->cur_index = param;
}

/**
 * @brief set RxCTCSS
 *
 * @param channel_ptr main channel pointer
 * @param param CTCSS status needed to be set,refer to CTCSS_param
 */
void channel_RxCTCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param == 0)
        return;
    if (param - 1 != 0)
    {
        bk4819_CTDCSS_enable(1);
        channel_ptr->cur_channel->Rx_CDCSS = 0;
    }
    channel_ptr->cur_channel->Rx_CTCSS = CTCSS_param[param - 1];
    channel_ptr->cur_index = param;
}

/**
 * @brief set TxCDCSS
 *
 * @param channel_ptr main channel pointer
 * @param param the parameter submenu pass back
 */
void channel_TxCDCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    DCS_CodeType_t CodeType;
    if (param == 0)
        return;
    if (param - 1 != 0)
    {
        if (param == 1)
        {
            channel_ptr->cur_channel->Tx_CDCSS = 0;
            // bk4819_CTDCSS_disable();
            return;
        }
        bk4819_CTDCSS_enable(0);
        channel_ptr->cur_channel->Tx_CTCSS = 0;
    }
    channel_ptr->cur_index = param;
    if (param <= 5)
        CodeType = CODE_TYPE_DIGITAL;
    else
        CodeType = CODE_TYPE_REVERSE_DIGITAL;

    channel_ptr->cur_channel->Tx_CDCSS = DCS_GetGolayCodeWord(CodeType, param - 2);

    // bk4819_CDCSS_set_v2(Code);
}

/**
 * @brief set RxCDCSS
 *
 * @param channel_ptr main channel pointer
 * @param param the parameter submenu pass back
 */
void channel_RxCDCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    DCS_CodeType_t CodeType;
    if (param == 0)
        return;

    if (param == 1)
    {
        channel_ptr->cur_channel->Rx_CDCSS = 0;
        // bk4819_CTDCSS_disable();
        return;
    }
    else
    {
        bk4819_CTDCSS_enable(0);
        channel_ptr->cur_channel->Rx_CTCSS = 0;
    }

    channel_ptr->cur_index = param;
    if (param <= 5)
        CodeType = CODE_TYPE_DIGITAL;
    else
        CodeType = CODE_TYPE_REVERSE_DIGITAL;

    channel_ptr->cur_channel->Rx_CDCSS = DCS_GetGolayCodeWord(CodeType, param - 2);
}

/**
 * @brief Change the function when pressing PF1 in mian interface
 *
 * @param channel_ptr channel pointer
 * @param param the parameter submenu pass back
 */
void PF1_funtion_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param == 0)
        return;
    switch (param)
    {
    case 1:
        channel_ptr->PF1 = PF_DTMF;
        break;
    case 2:
        channel_ptr->PF1 = PF_NOAA;
        break;
    case 3:
        channel_ptr->PF1 = PF_FM;
        break;
    case 4:
        channel_ptr->PF1 = PF_OFF;
    default:
        break;
    }
}


void bk4819_TxCTDCSS_set_auto(ui_main_channel_ptr channel_ptr)
{
    if (channel_ptr->cur_channel->Tx_CTCSS == 0 && channel_ptr->cur_channel->Tx_CDCSS == 0)
        return;
    if (channel_ptr->cur_channel->Tx_CTCSS != 0)
        bk4819_CTDCSS_set(0, channel_ptr->cur_channel->Tx_CTCSS);
    else if (channel_ptr->cur_channel->Tx_CDCSS != 0)
    {
        bk4819_CDCSS_set_v2(channel_ptr->cur_channel->Tx_CDCSS);
    }
}


void key_press_function(ui_main_channel_ptr channel_ptr, uint8_t key_press)
{
    if(key_press == 1)
    {
        switch (channel_ptr->PF1)
        {
        case PF_DTMF:
            main_DTMF_init(channel_ptr);
            xSemaphoreTake(jgfx_mutex, portMAX_DELAY);
            main_DTMF_input(channel_ptr);
            xSemaphoreGive(jgfx_mutex);
            ui_main_refresh();
            break;
        case PF_NOAA:
            break;
        case PF_FM:
            FM_main_init();
            break;
        default:
            break;
        }
    }
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
            // vTaskDelay(15);
            _r_menu_draw_item(menu_ptr, item, JGFX_MENU_ITEM_STATUS_UNSELECTED, i);
            vTaskDelay(15);
            // delay_1ms(1);
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
                          menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
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

        // draw box cling to the items
        //  jgfx_draw_rect(36 + menu_ptr->menu_x,
        //                 menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
        //                 36 + menu_ptr->menu_x + 104 - 1,
        //                 menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index + 15);


        /**draw box cling to the right side of the screen*/
        // jgfx_draw_rect(32 + menu_ptr->menu_x,
        //                menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
        //                32 + menu_ptr->menu_x + menu_ptr->menu_width,
        //                menu_ptr->menu_item_height / 2 - 4 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index + menu_ptr->menu_item_height);
    }

    // jgfx_draw_line(36 + menu_ptr->menu_x,
    //                menu_ptr->menu_item_height + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index,
    //                menu_ptr->menu_width + 30,
    //                menu_ptr->menu_item_height + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + 3 + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * index);
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
    submenu_ptr->num_in_page = 4;
    submenu_ptr->cur_page = ((submenu_ptr->cur_item - 1) / submenu_ptr->num_in_page) + 1; // Count from 1
}

/**
 * @brief Redraw submenu when trigger page-turn
 *
 * @param menu_ptr menu pointer
 * @param submenu_ptr   submenu pointer
 */
static void submenu_items_refresh(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr)
{

    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(35 + menu_ptr->menu_x, menu_ptr->menu_y + menu_ptr->menu_title_height + 5 + 17, menu_ptr->menu_width - 5, menu_ptr->menu_height - menu_ptr->menu_title_height - (menu_ptr->menu_divide_height * menu_ptr->item_show_num) - 4);
    // Draw items
    for (uint8_t i = 0; i < submenu_ptr->num_in_page; i++)
    {
        if (((submenu_ptr->cur_page - 1) * submenu_ptr->num_in_page + i + 1) > submenu_ptr->itemlist_num)
            break;
        jgfx_set_font(FLASH_FONT_EN_8X16_ADDR);
        jgfx_draw_text_en(36 + menu_ptr->menu_x,
                          submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (i + 1),
                          submenu_ptr->item_name[((submenu_ptr->cur_page - 1) * submenu_ptr->num_in_page + i)]);
    }

    // draw selector
    jgfx_set_color_hex(0x0000);
    jgfx_set_color_back_hex(0xFFFF);
    jgfx_draw_text_en(36 + menu_ptr->menu_x,
                      submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * ((submenu_ptr->cur_item - 1) % submenu_ptr->num_in_page + 1),
                      submenu_ptr->item_name[submenu_ptr->cur_item - 1]);
    submenu_show_channel_icon(menu_ptr,submenu_ptr);
}

/**
 * @brief Loop in submenu and wait for key-press order
 *
 * @param menu_ptr menu pointer
 * @param index current selector
 */
static uint8_t submenu_cb(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr)
{
    uint8_t item_num = submenu_ptr->itemlist_num;
    uint8_t index = (submenu_ptr->cur_item - 1) % 4 + 1;
    while (1)
    {
        key_map_t key = key_get();
        if (key != NULL)
        {
            if (key == 1)
            {
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(100);
                return submenu_ptr->cur_item;
            }
            else if (key == 2 && submenu_ptr->cur_item != 1)
            {
                submenu_ptr->cur_item -= 1;
                index -= 1;
                if (submenu_ptr->cur_item % 4 == 0)
                {
                    submenu_ptr->cur_page -= 1;
                    submenu_items_refresh(menu_ptr, submenu_ptr);
                    index = 4;
                }
                else
                {
                    jgfx_set_color_hex(0xFFFF);
                    jgfx_set_color_back_hex(0x0000);
                    jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (index + 1), submenu_ptr->item_name[submenu_ptr->cur_item]);
                    jgfx_set_color_hex(0x0000);
                    jgfx_set_color_back_hex(0xFFFF);
                    jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (index), submenu_ptr->item_name[submenu_ptr->cur_item - 1]);
                    submenu_show_channel_icon(menu_ptr,submenu_ptr);
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(100);
            }
            else if (key == 3 && submenu_ptr->cur_item != submenu_ptr->itemlist_num)
            {
                submenu_ptr->cur_item += 1;
                index += 1;
                if ((submenu_ptr->cur_item - 1) % 4 == 0)
                {
                    submenu_ptr->cur_page += 1;
                    submenu_items_refresh(menu_ptr, submenu_ptr);
                    index = 1;
                }
                else
                {

                    jgfx_set_color_hex(0xFFFF);
                    jgfx_set_color_back_hex(0x0000);
                    jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (index - 1), submenu_ptr->item_name[submenu_ptr->cur_item - 2]);
                    jgfx_set_color_hex(0x0000);
                    jgfx_set_color_back_hex(0xFFFF);
                    jgfx_draw_text_en(36 + menu_ptr->menu_x, submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (index), submenu_ptr->item_name[submenu_ptr->cur_item - 1]);
                    submenu_show_channel_icon(menu_ptr,submenu_ptr);
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(100);
            }
            else if (key == 4)
            {
                // return_to_menu(menu_ptr);
                menu_ptr->status = JGFX_MENU_STATUS_SELECTED;
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(100);
                return 0;
                // break;
            }
        }
    }
    return -1;
}


static void submenu_show_channel_icon(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr)
{
    if(submenu_ptr->show_icon[submenu_ptr->cur_item] == 1)
    {
        uint32_t addr = menu_ptr->channel_ptr->channel == 1 ? (FLASH_ICON_CHANNELA_ADDR) : (FLASH_ICON_CHANNELB_ADDR);
        jgfx_draw_img_byaddr(32 + 5, 5, 10, 10, addr);
    }
    else
    {
        jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);
        jgfx_fill_react(32 + 5, 5, 10, 10);
    }
}