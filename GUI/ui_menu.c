#include "ui_menu.h"
#include "comp_menu.h"

static jgfx_menu_t jgfx_menu;
ui_page_t ui_menu;
corner_index_num_t jgfx_menu_corner;
Brightness_setting_t Display_brightness;


void ui_menu_initial(void)
{
    ui_menu.ui_init = ui_menu_init;
    ui_menu.ui_refresh = ui_menu_refresh;
    ui_menu.ui_event_cb = ui_menu_event_cb;
    ui_menu.ui_destory = ui_menu_destory;
}

static void draw_menu(void)
{
}

static void draw_slider(void)
{
}

// Set main channel step
static void Step_callback(void)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 7;
    sub_menu.cur_item = radio_channel.cur_index;
    jgfx_set_color_back_hex(0x0000);
    uint8_t temp = submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " 5.0kHZ", " 6.25KHZ", " 10.0KHZ", " 12.5KHZ", " 20.0KHZ", " 25.0KHZ", " 50.0KHZ");
    radio_channel_change(&radio_channel, temp);
    // radio_channel_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " 5.0kHZ", " 6.25KHZ", " 10.0KHZ", " 12.5KHZ", " 20.0KHZ", " 25.0KHZ", " 50.0KHZ"));
}

static void T_CTCSS_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = radio_channel.cur_index;
    channel_TxCTCSS_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                             " OFF", " 67.0HZ", " 69.3HZ", " 71.9HZ", " 74.4HZ", " 77.0HZ", " 79.7HZ", " 82.5HZ", " 85.4HZ", " 88.5HZ"));
}

static void R_CTCSS_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = radio_channel.cur_index;
    channel_RxCTCSS_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                             " OFF", " 67.0HZ", " 69.3HZ", " 71.9HZ", " 74.4HZ", " 77.0HZ", " 79.7HZ", " 82.5HZ", " 85.4HZ", " 88.5HZ"));
}

static void T_CDCSS_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = radio_channel.cur_index;
    channel_TxCDCSS_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                             " OFF", " D023N", " D025N", " D026N", " D031N", " D023I", "D025I", " D026I", " D031I", " D032I"));
}

static void R_CDCSS_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = radio_channel.cur_index;
    channel_RxCDCSS_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                             " OFF", " D023N", " D025N", " D026N", " D031N", " D023I", "D025I", " D026I", " D031I", " D032I"));
}

static void Offset_callback(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    input_window_init(menu_ptr, channel_ptr);
    // bool input_state = FALSE;        //seems unuseful here
    offset_setting(channel_ptr);
}

static void SFT_D_callback(ui_main_channel_ptr channel_ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 3;
    sub_menu.cur_item = radio_channel.SFT_D_index;
    offset_direction(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                       " OFF", " +", " -"));
}

static void TxPower_callback(ui_main_channel_ptr channel_ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 3;
    sub_menu.cur_item = radio_channel.TxPower_index;
    TxPower_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                     " HIGH", " MID", " LOW"));
}

static void Scan_callback(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    // Scan_test();
    frequency_scan(menu_ptr, channel_ptr);
}

#ifndef BRIGHTNESS_MENU_SHOW_WITH_PARAM
// Set screen Brightness
static void Brightness_callback(void)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = Display_brightness.cur_bri;
    Brightness_change(&Display_brightness, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  20", "  50", "  80", "  100"));
}

#else
// Unused now,waiting to be changed that add current Brightness value on the right side without opening a new submenu
static void Brightness_callback(uint8_t *string, jgfx_menu_ptr menu_ptr)
{
    submenu_item_t sub_menu;
    uint8_t Brightness_value = (*(string + 13) - 48) * 10 + (*(string + 14) - 48);
    uint8_t bri_num = Display_brightness.cur_bri;

    render_submenu_with_param(menu_ptr, string, 1);

    while(key_get());
    vTaskDelay(50);
    while (1)
    {
        uint8_t key = key_get();
        if (key != KEY_MAP_NONE)
        {
            if (key == 1 || key == 4)
            {
                jgfx_set_color_hex(0x0000);
                jgfx_set_color_back_hex(0xFFFF);
                jgfx_draw_text_en(36 + menu_ptr->menu_x,
                menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 1,
                string);
                while(key_get());
                break;
            }
            else if (key == 2 && Brightness_value < 100)
            {
                Brightness_value += 5;
                Brightness_change(&Display_brightness, ++bri_num);
            }
            else if (key == 3 && Brightness_value > 5)
            {
                Brightness_value -= 5;
                Brightness_change(&Display_brightness, --bri_num);
            }
            else
                continue;
            if(Brightness_value == 100)
                sprintf(&string[12], "%d", Brightness_value / 10); 
            else
                sprintf(&string[12], " %d", Brightness_value / 10); 

            sprintf(&string[14], "%d", Brightness_value % 10); 
            jgfx_draw_text_en(36 + menu_ptr->menu_x,
                menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 1,
                string);
        }
        while(key_get());
    }
}
#endif

#ifndef TIMER_MENU_SHOW_WITH_PARAM
// Set Timer to enabling the screen-off feature
static void Timer_callback(void)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 5;
    sub_menu.cur_item = Display_Timer.index;
    Display_Timer_change(&Display_Timer, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  OFF", "  10s", "  30S", "  1min", "  2min"));
}
#else
static void Timer_callback(uint8_t *string, jgfx_menu_ptr menu_ptr)
{
    submenu_item_t sub_menu;
    uint8_t Timer_value = (*(string + 11) - 48) * 100 + (*(string + 12) - 48) * 10 + (*(string + 13) - 48);
    uint8_t cur_item = Display_Timer.index;

    render_submenu_with_param(menu_ptr, string, 2);
    while(key_get());
    vTaskDelay(50);
    while (1)
    {
        vTaskDelay(20);
        uint8_t key = key_get();
        if (key != KEY_MAP_NONE)
        {
            if (key == 1 || key == 4)
            {
                jgfx_set_color_hex(0x0000);
                jgfx_set_color_back_hex(0xFFFF);
                jgfx_draw_text_en(36 + menu_ptr->menu_x,
                menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 2,
                string);
                while(key_get());
                break;
            }
            else if (key == 2 && cur_item < 5)
            {
                Display_Timer_change(&Display_Timer, ++cur_item);
            }
            else if (key == 3 && cur_item > 1)
            {
                Display_Timer_change(&Display_Timer, --cur_item);
            }
            else
                continue;
            if(cur_item == 1)
            {
                sprintf(&string[12], "OFF"); 
            }
            else
            {
                if(Display_Timer.screen_off >= 100)
                    sprintf(&string[11], "%d", Display_Timer.screen_off / 10); 
                else
                    sprintf(&string[11], " %d", Display_Timer.screen_off / 10); 

                sprintf(&string[13], "%dS", Display_Timer.screen_off % 10); 
            }
            jgfx_draw_text_en(36 + menu_ptr->menu_x,
                menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 2,
                string);
        }
        while(key_get());
    }

}
#endif

static void all_channel_callback(void)
{
}

static void PF1_Button_callback(void)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = radio_channel.PF1;
    PF1_funtion_change(&radio_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  DTMF", "  NOAA", " FM", " OFF"));
}

static void PF2_Button_callback(void)
{
}

static void TopKey_Button_callback(void)
{
}

static void DTMF_Up_Code_setting_callback(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    sub_channel_t *temp_channel = channel_ptr->cur_channel;
    channel_ptr->cur_channel = &channel_ptr->channel_2;

    _r_clear_item_area(menu_ptr);
    main_DTMF_init(channel_ptr);
    main_DTMF_input_MENU(channel_ptr,1);
    channel_ptr->cur_channel = temp_channel;
}

static void DTMF_Down_Code_setting_callback(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    sub_channel_t *temp_channel = channel_ptr->cur_channel;
    channel_ptr->cur_channel = &channel_ptr->channel_2;

    _r_clear_item_area(menu_ptr);
    main_DTMF_init(channel_ptr);
    main_DTMF_input_MENU(channel_ptr,0);
    channel_ptr->cur_channel = temp_channel;
}

static void Radio_Settings(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 5, exit_flag = 0;
    sub_menu.cur_item = 1;
    while (!exit_flag)
    {
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " Step", " T-CTCSS", " R-CTCSS", " T-CDCSS", " R-CDCSS"))
        {
        case 1:
            Step_callback();
            break;
        case 2:
            T_CTCSS_callback();
            break;
        case 3:
            R_CTCSS_callback();
            break;
        case 4:
            T_CDCSS_callback();
            break;
        case 5:
            R_CDCSS_callback();
            break;
        default:
            return_to_menu(ptr);
            exit_flag = 1;
            break;
        }
    }
}

static void Display_Settings(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t *pString;
    uint8_t submenu_list_num = 2, exit_flag = 0;
    sub_menu.cur_item = 1;
    // flash_channel
    static uint8_t Display_item[][MENU_NAME_LEN_LIM] =
        {

#ifdef  BRIGHTNESS_MENU_SHOW_WITH_PARAM
            "Brightness   80",
#else 
            "Brightness",
#endif

#ifdef  TIMER_MENU_SHOW_WITH_PARAM
            "Timer       30S",
#else       
            "Timer",
#endif
        };
    
    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show_with_array(&jgfx_menu, submenu_list_num, &sub_menu, Display_item))
        {
        case 1:
#ifndef BRIGHTNESS_MENU_SHOW_WITH_PARAM
                Brightness_callback();
#else
                pString = Display_item[0];
                Brightness_callback(pString, ptr);
#endif
            break;
        case 2:
#ifndef TIMER_MENU_SHOW_WITH_PARAM
                Timer_callback();
#else
                pString = Display_item[1];
                Timer_callback(pString, ptr);
#endif
            break;
        default:
            return_to_menu(ptr);
            exit_flag = 1;
            break;
        }
    }
}

static void Banks(jgfx_menu_ptr menu_ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4, exit_flag = 0;
    sub_menu.cur_item = 1;

    menu_list_t Banks_item[] =
        {
            {"Offset",  1},
            {"SFT-D",   1},
            {"TxPower", 1},
            {"Scan",    0},
        };

    for (int i = 0; i < sizeof(Banks_item) / sizeof(Banks_item[0]); i++)
        sub_menu.show_icon[i] = Banks_item->show_channel_pic;

    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "Offset", "SFT-D", "TxPower", "Scan"))
        {
        case 1:
            Offset_callback(menu_ptr, &radio_channel);
            ;
            break;
        case 2:
            SFT_D_callback(&radio_channel);
            break;
        case 3:
            TxPower_callback(&radio_channel);
            break;
        case 4:
            Scan_callback(menu_ptr, &radio_channel);
            break;
        default:
            return_to_menu(menu_ptr);
            exit_flag = 1;
            break;
        }
    }
}

static void Button_press(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 1, exit_flag = 0;
    sub_menu.cur_item = 1;
    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " PF1", " PF2", " TOP KEY"))
        {
        case 1:
            PF1_Button_callback();
            ;
            break;
        case 2:
            PF2_Button_callback();
            break;
        case 3:
            TopKey_Button_callback();
            break;
        default:
            return_to_menu(ptr);
            exit_flag = 1;
            break;
        }
    }
}

void BandWidth(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 3;
    sub_menu.cur_item = 1;
    uint8_t param = submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " wide", " narrow", " narrower");
    channel_bandwidth_change(&radio_channel, param);
    ptr->status = JGFX_MENU_STATUS_SELECTED;
    return_to_menu(ptr);
}

#ifndef SUQELCH_MENU_SHOW_WITH_PARAM
void Squelch(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = radio_channel.cur_channel->sql + 1;
    uint8_t param = submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                      " 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9");

    channel_squelch_change(&radio_channel, param);
    ptr->status = JGFX_MENU_STATUS_SELECTED;
    return_to_menu(ptr);
}
#else

static void Squelch(jgfx_menu_ptr menu_ptr)
{
    submenu_item_t sub_menu;
    uint8_t *string;
    uint8_t sql_value;
    uint8_t cur_item;
    uint8_t inidex;
    sub_menu.cur_item = radio_channel.cur_channel->sql;

    for(int i = 0; i < sizeof(jgfx_menu.menu_item) / sizeof(jgfx_menu.menu_item[0]); i++)
    {
        if(jgfx_menu.menu_item[i].menu_id == MENU_SQUELCH)
            string = jgfx_menu.menu_item[i].menu_name;
    }
    sql_value = (*(string + 14) - 48);

    render_submenu_with_param(menu_ptr, string, 1);
    while(key_get());
    vTaskDelay(50);
    while (1)
    {
        vTaskDelay(20);
        uint8_t key = key_get();
        if (key != KEY_MAP_NONE)
        {
            if (key == 1 || key == 4)
            {
                jgfx_set_color_hex(0x0000);
                jgfx_set_color_back_hex(0xFFFF);
                jgfx_draw_text_en(36 + menu_ptr->menu_x,
                menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 1,
                string);
                menu_ptr->status = JGFX_MENU_STATUS_SELECTED;
                while(key_get());
                break;
            }
            else if (key == 2 &&  sub_menu.cur_item < 9)
            {
                channel_squelch_change(&radio_channel, (++sub_menu.cur_item + 1));
            }
            else if (key == 3 &&  sub_menu.cur_item > 0)
            {
                channel_squelch_change(&radio_channel, (--sub_menu.cur_item + 1));
            }
            else
                continue;

            sprintf(&string[14], "%d", radio_channel.cur_channel->sql); 
            
            jgfx_draw_text_en(36 + menu_ptr->menu_x,
                menu_ptr->menu_item_height / 2 - 1 + (menu_ptr->menu_use_tiltle == 1 ? menu_ptr->menu_title_height : 0) + menu_ptr->menu_y + (menu_ptr->menu_item_height + menu_ptr->menu_divide_height) * 1,
                string);
        }
        while(key_get());
    }
}
#endif

static void Channel_Save(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 64;
    sub_menu.cur_item = 1;
    flash_channel_init();
    uint8_t param = submenu_item_show_with_array(&jgfx_menu, submenu_list_num, &sub_menu, flash_channel);
    flash_channel_save(param, radio_channel.cur_channel->frequency);
    ptr->status = JGFX_MENU_STATUS_SELECTED;
    return_to_menu(ptr);
}

static void Channel_Select(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 64;
    sub_menu.cur_item = 1;
    flash_channel_init();
    uint8_t param = submenu_item_show_with_array(&jgfx_menu, submenu_list_num, &sub_menu, flash_channel);
    flash_channel_read(param, &radio_channel.cur_channel->frequency);

    radio_channel.channel_changed == TRUE;
    channel_store(&radio_channel);

    ptr->status = JGFX_MENU_STATUS_SELECTED;
    return_to_menu(ptr);
}

static void Channel_Delete(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 64;
    sub_menu.cur_item = 1;
    flash_channel_init();
    uint8_t param = submenu_item_show_with_array(&jgfx_menu, submenu_list_num, &sub_menu, flash_channel);
    flash_channel_delete(param);

    uint32_t d_frequency = 0;
    flash_channel_read(1, &d_frequency);
    // printf("frequency equal to : %d \n", d_frequency);

    ptr->status = JGFX_MENU_STATUS_SELECTED;
    return_to_menu(ptr);
}

static void DTMF_Setting(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3, exit_flag = 0;
    sub_menu.cur_item = 1;
    while(!exit_flag)
    {
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "UPCode", "DWCode", "DTMF Code"))
        {
        case 1:
            DTMF_Up_Code_setting_callback(ptr, &radio_channel);
            break;
        case 2:
            DTMF_Down_Code_setting_callback(ptr, &radio_channel);
            break;
        default:
            return_to_menu(ptr);
            exit_flag = 1;
            break;
        }
    }
}

void ui_menu_init(void)
{

    jgfx_clear_screen();
    vTaskDelay(100);

    jgfx_menu_init(&jgfx_menu, &radio_channel);
    jgfx_menu_set_size(&jgfx_menu, DISPLAY_W, DISPLAY_H);

    jgfx_menu_set_title(&jgfx_menu, "Menu");
    jgfx_menu_set_title_height(&jgfx_menu, 20);

    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[0].menu_name, Radio_Settings);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[1].menu_name, Squelch);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[2].menu_name, Channel_Save);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[3].menu_name, Channel_Select);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[4].menu_name, Channel_Delete);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[5].menu_name, Display_Settings);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[6].menu_name, Banks);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[7].menu_name, BandWidth);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[8].menu_name, Button_press);
    jgfx_menu_append_text(&jgfx_menu, jgfx_menu.menu_item[9].menu_name, DTMF_Setting);

    corner_index_init(&jgfx_menu_corner);

    jgfx_menu_show(&jgfx_menu);

    index_num_display(&jgfx_menu);
}

void ui_menu_refresh(void)
{
}

void ui_menu_destory(void)
{
}

void ui_menu_event_cb(void)
{
    // Deactivate the keyboard until menu draw completely
    if (jgfx_menu.menu_draw_completed == FALSE)
        return;

    Display_Timer_count(&Display_Timer);
    corner_index_refresh(&jgfx_menu, &jgfx_menu_corner);
    // corner_index_flicker(&jgfx_menu,&jgfx_menu_corner);

    key_map_t key = key_get();
    if (key != KEY_MAP_NONE)
    {
        wakeup_screen(&Display_brightness, &Display_Timer);
        if (key == KEY_MAP_1)
        {
            jgfx_menu_click(&jgfx_menu);
            corner_index_init(&jgfx_menu_corner);
            index_num_display(&jgfx_menu);
        }
        else if (key == KEY_MAP_2)
        {
            jgfx_menu_item_previous(&jgfx_menu);
            index_num_display(&jgfx_menu);
            key_press_delay();
        }
        else if (key == KEY_MAP_3)
        {
            jgfx_menu_item_next(&jgfx_menu);
            index_num_display(&jgfx_menu);
            key_press_delay();
        }
        else if (key == KEY_MAP_4)
        {
            jgfx_menu_destory(&jgfx_menu);
            uiStackPop(&ui_stack);
            temp_page = uiStackGetTop(&ui_stack);
            if (temp_page != NULL)
                temp_page->ui_refresh();
        }
        else if (key == KEY_MAP_8)
        {
        }
        else if (key == KEY_MAP_16)
        {
        }
        else
        {
            // use index_num1 and index_num2 to realize jumping function,and use time counter to realize Two-digit jump.

            if (!jgfx_menu_index(&jgfx_menu, index_num_cb(&jgfx_menu_corner, &jgfx_menu, KEY_GET_NUM(key))))
            {
                // printf("%d %d\n", jgfx_menu_corner.index_jump_count1, jgfx_menu_corner.index_jump_count2);
                corner_index_init(&jgfx_menu_corner);
            }
            index_num_display(&jgfx_menu);
            key_press_delay();
        }

        vTaskDelay(500);
    }
}