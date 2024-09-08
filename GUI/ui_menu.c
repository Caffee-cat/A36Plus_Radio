#include "ui_menu.h"
#include "comp_menu.h"

static jgfx_menu_t jgfx_menu;
extern ui_main_channel_t radio_channel;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
ui_page_t ui_menu;
corner_index_num_t jgfx_menu_corner;
Brightness_setting_t Display_brightness;
Display_Timer_t Display_Timer;

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

// Set screen Brightness
static void Brightness_callback_v1(void)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = Display_brightness.cur_bri;
    Brightness_change(&Display_brightness, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  20", "  50", "  80", "  100"));
}

// Unused now,waiting to be changed that add current Brightness value on the right side without opening a new submenu
static void Brightness_callback_v2(void)
{
}

// Set Timer to enabling the screen-off feature
static void Timer_callback(void)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 5;
    sub_menu.cur_item = Display_Timer.index;
    Display_Timer_change(&Display_Timer, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  OFF", "  10s", "  30S", "  1min", "  2min"));
}

static void all_channel_callback(void)
{
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
    // jgfx_set_font(JGFX_FONT_EN_8X16);
    // jgfx_draw_text_en(36,3,"______");
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 2, exit_flag = 0;
    sub_menu.cur_item = 1;
    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " Brightness", " Timer"))
        {
        case 1:
            Brightness_callback_v1();
            break;
        case 2:
            Timer_callback();
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

void Squelch(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = 1;
    
    uint8_t param = submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                     " 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9");

    channel_bandwidth_change(&radio_channel, param);
    ptr->status = JGFX_MENU_STATUS_SELECTED;
    return_to_menu(ptr);
}

void cb6(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item6", " menu_item5", " menu_item6");
}

void cb7(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item7", " menu_item5", " menu_item6");
}

void cb8(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item8", " menu_item5", " menu_item6");
}

void cb9(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item9", " menu_item5", " menu_item6");
}

void cb10(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item10", " menu_item5", " menu_item6");
}

void ui_menu_init(void)
{

    jgfx_clear_screen();
    vTaskDelay(100);

    jgfx_menu_init(&jgfx_menu, &radio_channel);
    jgfx_menu_set_size(&jgfx_menu, DISPLAY_W, DISPLAY_H);

    jgfx_menu_set_title(&jgfx_menu, "Menu");
    jgfx_menu_set_title_height(&jgfx_menu, 20);
    jgfx_menu_append_text(&jgfx_menu, "Radio Setting", Radio_Settings);
    jgfx_menu_append_text(&jgfx_menu, "Display ", Display_Settings);
    // Don't delete the space below,it works
    jgfx_menu_append_text(&jgfx_menu, "Banks ", Banks);
    jgfx_menu_append_text(&jgfx_menu, "B/W", BandWidth);
    // jgfx_menu_append_text(&jgfx_menu, "SQL", Squelch);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing6", cb6);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing7", cb7);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing8", cb8);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing9", cb9);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing10", cb10);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing11", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing12", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing13", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing14", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing15", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing16", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing17", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing18", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing19", NULL);

    corner_index_init(&jgfx_menu_corner);

    jgfx_menu_show(&jgfx_menu);

    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);
    jgfx_set_font(JGFX_FONT_EN_8X16);
    vTaskDelay(10);
    jgfx_draw_text_en(130, 5, "01");
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
    if (jgfx_menu.menu_draw_complete == FALSE)
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
        }
        else if (key == KEY_MAP_3)
        {
            jgfx_menu_item_next(&jgfx_menu);
            index_num_display(&jgfx_menu);
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
        }

        vTaskDelay(500);
    }
}