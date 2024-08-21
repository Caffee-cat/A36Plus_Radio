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

uint32_t main_channel_step[] = {10, 15, 20, 25, 30, 35, 40, 45};
uint32_t brightness_param[] = {5, 20, 100, 200, 500};
uint16_t Display_Timer_param[] = {10, 30, 60, 120};
uint16_t CTCSS_param[] = {0, 670, 693, 719, 744, 770, 797, 825, 854, 885, 915};

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
        // printf("%d",menu_ptr->channel_ptr->channel == 1?(FLASH_ICON_CHANNELA_ADDR) : (FLASH_ICON_CHANNELB_ADDR));
        // delay_1ms(1);
        uint32_t addr = menu_ptr->channel_ptr->channel == 1 ? (FLASH_ICON_CHANNELA_ADDR) : (FLASH_ICON_CHANNELB_ADDR);
        jgfx_draw_img_byaddr(32 + 5, 5, 10, 10, addr);
        // jgfx_draw_img_byaddr(32 + 5, 5, 10, 10, FLASH_ICON_CHANNELA_ADDR);
    }

    /** Draw items */
    delay_1us(10);
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
 * @return uint8_t
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
        // jgfx_draw_text_en(36 + menu_ptr->menu_x,
        //                   submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (i + 1),
        //                   submenu_ptr->item_name[i]);
    }
    va_end(pointer);

    // Draw items
    submenu_items_refresh(menu_ptr, submenu_ptr);

    // redraw the line in the botton to fix some error
    jgfx_set_color_hex(0xFFFF);
    jgfx_set_color_back_hex(0x0000);
    jgfx_draw_rect(32 + menu_ptr->menu_x, menu_ptr->menu_y, menu_ptr->menu_x + menu_ptr->menu_width + 31, menu_ptr->menu_y + menu_ptr->menu_height - 5);

    return submenu_cb(menu_ptr, submenu_ptr);
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
    jgfx_set_font(JGFX_FONT_EN_8X16_BLOD);
    int i = 0;
    char count_num_str[3];
    snprintf(count_num_str, sizeof(count_num_str), "%02d", menu_ptr->cur_item->item_id + 1);
    count_num_str[2] = '\0';
    jgfx_draw_text_en(130, 3, count_num_str);
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
            corner_ptr->index_jump_count1 == 65535;
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
        corner_ptr->index_jump_count1 = 30000;
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
            corner_ptr->index_jump_count1 = 30000;
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

void main_channel_init(ui_main_channel_ptr channel_ptr)
{
    if (channel_ptr->Initial_flag == TRUE)
        return;
    channel_ptr->Initial_flag = TRUE;
    // Achieve flicker the selected channel number
    channel_ptr->flash_count_num1 = 0;
    channel_ptr->flash_count_num2 = 0;

    channel_ptr->channel_1 = 430000;
    channel_ptr->channel_2 = 439460;
    channel_ptr->ch_bak = 0;
    bk4819_set_freq(43000000);

    // Point to channel 1 for initial setup
    channel_ptr->cur_channel = &channel_ptr->channel_1;

    channel_ptr->channel = (channel_ptr->cur_channel == &channel_ptr->channel_1 ? TRUE : FALSE);
    channel_ptr->step = 10; // 10.0KHZ
    channel_ptr->cur_index = 1;
    channel_ptr->ch_pra = main_channel_step;
    channel_ptr->ch_val = channel_ptr->ch_pra;
}

void jgfx_channel_change(ui_main_channel_ptr channel_ptr, jgfx_channel_step_t step)
{
    if (step == MAIN_CHANNEL_NONE)
        return;
    channel_ptr->cur_index = step;
    channel_ptr->step = channel_ptr->ch_pra[step - 1];
}

void menu_draw_rightside(uint8_t *string)
{
    // jgfx_draw_text_en(36 + menu_ptr->menu_x,
    //                       submenu_ptr->line_height + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * (i + 1),
    //                       submenu_ptr->item_name[i]);
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
    bri_ptr->cur_bri = 3;
    bri_ptr->bri_pra = brightness_param;
    bri_ptr->bri_val = bri_ptr->bri_pra;
    bri_ptr->bri_val += 3;
    // printf("Brightness vlaue now is %d\n",*(bri_ptr->bri_val));
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
    if (Timer_ptr->Timer_init_flag == TRUE)
        return;
    Timer_ptr->index = 1;
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
    if (Timer_ptr->Timer_init_flag == FALSE)
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
void input_window_init(jgfx_menu_ptr menu_ptr)
{
    _r_clear_item_area(menu_ptr);

    jgfx_set_font(JGFX_FONT_EN_16X32);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

    jgfx_draw_text_en(32 + 3, DISPLAY_H / 10 + 25, "----");
    jgfx_draw_text_en(32 + 3 + 3 * 16 + 14, DISPLAY_H / 10 + 25 + 13, ".");
    jgfx_draw_text_en(32 + 3 + 4 * 16 + 5, DISPLAY_H / 10 + 25, "---");
}

void create_channel_draw(uint32_t input_channel)
{
    uint8_t input_window[8];
    // jgfx_draw_text_en(32 + 3, DISPLAY_H / 10 + 25, "----");
    // jgfx_draw_text_en(32 + 3 + 3 * 16 + 14, DISPLAY_H / 10 + 25 + 13, ".");
    // jgfx_draw_text_en(32 + 3 + 4 * 16 + 5, DISPLAY_H / 10 + 25, "---");
    jgfx_set_font(JGFX_FONT_EN_16X32);
    jgfx_set_color_hex(JGFXF_COLOR_WHITE);
    jgfx_set_color_back_hex(JGFXF_COLOR_BLACK);

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

void draw_info(jgfx_add_channel_status_t status)
{
    jgfx_set_font(JGFX_FONT_EN_8X16);
    if (status == ADD_CHANNEL_CANCEL)
    {
        jgfx_set_color_hex(JGFXF_COLOR_CYAN);
        jgfx_set_color_back_hex(JGFXF_COLOR_YELLOW);
        jgfx_draw_text_en(32 + 8, DISPLAY_H - 35, "  Add cancel  ");
    }

    delay_1ms(10);
}

/**
 * @brief set TxCTCSS1 for temporarily
 *
 * @param channel_ptr main channel pointer
 * @param prarm CTCSS status needed to be set,refer to CTCSS_param
 */
void channel_CTCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param)
{
    if (param - 1 != 0)
        bk4819_CTDCSS_enable(1);
    else
        bk4819_CTDCSS_disable();
    bk4819_CTDCSS_set(0, CTCSS_param[param - 1]);
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
            delay_1us(5);
            _r_menu_draw_item(menu_ptr, item, JGFX_MENU_ITEM_STATUS_UNSELECTED, i);
            delay_1us(10);
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

        // draw box cling to the right side of the screen
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
    uint8_t index = submenu_ptr->cur_item % 4;
    while (1)
    {
        key_map_t key = key_get();
        if (key != NULL)
        {
            if (key == 1)
            {
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(10);
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
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(10);
            }
            else if (key == 3 && submenu_ptr->cur_item != submenu_ptr->itemlist_num)
            {
                submenu_ptr->cur_item += 1;
                index += 1;
                if (submenu_ptr->cur_item % (4 + 1) == 0)
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
                }
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(10);
            }
            else if (key == 4)
            {
                // return_to_menu(menu_ptr);
                menu_ptr->status = JGFX_MENU_STATUS_SELECTED;
                while (key_get() != KEY_MAP_NONE)
                    ;
                delay_1us(10);
                return 0;
                // break;
            }
        }
    }
    return -1;
}
