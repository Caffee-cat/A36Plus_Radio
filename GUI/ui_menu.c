#include "ui_menu.h"
#include "comp_menu.h"

#define MENU_MAX 3

// static bool sys_start = FALSE;
ui_page_t ui_menu;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
static jgfx_menu_t jgfx_menu;
corner_index_num_t jgfx_menu_corner;
extern ui_main_channel_t jgfx_channel;
Display_Timer_t Display_Timer;
Brightness_setting_t Display_brightness;

// uint8_t *menu_list[MENU_MAX] = {
//     "Common",
//     "Tone",
//     "Truncking",
// };

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
void Step_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 8;
    sub_menu.cur_item = jgfx_channel.cur_index;
    jgfx_channel_change(&jgfx_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " 10.0kHZ", " 15.0KHZ", " 20.0KHZ", " 25.0KHZ", " 30.0KHZ", " 35.0KHZ", " 40.0KHZ", " 45.0KHZ"));
}

void CTCSS_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = jgfx_channel.cur_index;
    channel_CTCSS_change(&jgfx_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                          " OFF", " 67.0HZ", " 69.3HZ", " 71.9HZ", " 74.4HZ", "77.0KHZ", "79.7HZ", " 82.5HZ", " 85.4HZ", " 88.5HZ"));
}

void CDCSS_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 10;
    sub_menu.cur_item = jgfx_channel.cur_index;
    channel_CTCSS_change(&jgfx_channel, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu,
                                                          " OFF", " 67.0HZ", " 69.3HZ", " 71.9HZ", " 74.4HZ", "77.0KHZ", "79.7HZ", " 82.5HZ", " 85.4HZ", " 88.5HZ"));
}

void Offset_callback(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr)
{
    input_window_init(menu_ptr, channel_ptr);
    // bool input_state = FALSE;        //seems unuseful here
    offset_setting(channel_ptr);
}

// Set screen Brightness
void Brightness_callback_v1()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = Display_brightness.cur_bri;
    Brightness_change(&Display_brightness, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  20", "  50", "  80", "  100"));
}

// Unused now,waiting to be changed that add current Brightness value on the right side without opening a new submenu
void Brightness_callback_v2()
{
}

// Set Timer to enabling the screen-off feature
void Timer_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = Display_Timer.index;
    Display_Timer_change(&Display_Timer, submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  10s", "  30S", "  1min", "  2min"));
}

void all_channel_callback()
{
}

void Radio_Settings(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 2, exit_flag = 0;
    sub_menu.cur_item = 1;
    while (!exit_flag)
    {
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " Step", " T-CTCSS", " T-CDCSS"))
        {
        case 1:
            Step_callback();
            break;
        case 2:
            CTCSS_callback();
            break;
        case 3:
            CDCSS_callback();
            break;
        default:
            return_to_menu(ptr);
            exit_flag = 1;
            break;
        }
    }
}

void Display_Settings(jgfx_menu_ptr ptr)
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

void Banks(jgfx_menu_ptr menu_ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 1, exit_flag = 0;
    sub_menu.cur_item = 1;
    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "Offset"))
        {
        case 1:
            Offset_callback(menu_ptr, &jgfx_channel);
            ;
            break;
        case 2:
            all_channel_callback();
            break;
        default:
            return_to_menu(menu_ptr);
            exit_flag = 1;
            break;
        }
    }
}

void cb4(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item4", " menu_item4", " menu_item4");
}

void cb5(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item5", " menu_item5", " menu_item6");
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
    jgfx_menu_init(&jgfx_menu, &jgfx_channel);
    jgfx_menu_set_size(&jgfx_menu, DISPLAY_W, DISPLAY_H);

    if (jgfx_menu.sys_start == FALSE)
    {
        jgfx_menu.sys_start = TRUE;
        jgfx_set_font(JGFX_FONT_EN_8X16);
        jgfx_set_color_hex(0xFFFF);
        jgfx_set_color_back_hex(0x0000);
        jgfx_draw_text_en(32 + DISPLAY_W / 2 - (7 * 8 / 2), DISPLAY_H / 2 - 20, "A36Plus");
        jgfx_draw_text_en(32 + DISPLAY_W / 2 - (4 * 8 / 2), DISPLAY_H / 2 + 10, "RTOS");
        vTaskDelay(1500);
    }
    jgfx_clear_screen();
    jgfx_menu_set_title(&jgfx_menu, "Menu");
    jgfx_menu_set_title_height(&jgfx_menu, 20);
    // jgfx_menu_set_divide(&jgfx_menu, 10);

    jgfx_menu_append_text(&jgfx_menu, "Radio Setting", Radio_Settings);
    jgfx_menu_append_text(&jgfx_menu, "Show ", Display_Settings);
    // Don't delete the space below,it works
    jgfx_menu_append_text(&jgfx_menu, "Banks ", Banks);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing4", cb4);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing5", cb5);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing6", cb6);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing7", cb7);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing8", cb8);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing9", cb9);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing10", cb10);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing11", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing12", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing13", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing14", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing15", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing16", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing17", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing18", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing19", NULL);

    main_channel_init(&jgfx_channel);
    corner_index_init(&jgfx_menu_corner);
    Brightness_init(&Display_brightness);

    Display_Timer_Init(&Display_Timer);

    jgfx_menu_show(&jgfx_menu);
    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);
    jgfx_set_font(JGFX_FONT_EN_8X16);
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