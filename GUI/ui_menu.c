#include "ui_menu.h"
#include "comp_menu.h"

#define MENU_MAX 3

// static uint8_t i = 0;
ui_page_t ui_menu;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
static jgfx_menu_t jgfx_menu;
corner_index_num_t jgfx_menu_corner;
extern ui_main_channel_t jgfx_channel;
uint16_t Timer = 0;
Display_Timer_t Display_Timer;
Brigthtness_setting_t Brigthtness_setting;

uint8_t *menu_list[MENU_MAX] = {
    "Common",
    "Tone",
    "Truncking",
};

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

void Radio_Settings(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 1, exit_flag = 0;
    sub_menu.cur_item = 1;
    // uint8_t *hz = "12.5";
    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " Step"))
        {
        case 1:
            Step_callback();
            break;
        default:
            return_to_menu(ptr);
            exit_flag = 1;
            break;
        }
    }
}

void Step_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 3;
    sub_menu.cur_item = 1;
    while (1)
    {
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " 10.0kHZ", " 15.0KHZ", " 20.0KHZ"))
        {
        case 1:
            jgfx_channel.step = 10; // 10.0KHZ
            // index = 1;
            break;
        case 2:
            jgfx_channel.step = 15; // 15.0KHZ
            // index = 2;
            break;
        case 3:
            jgfx_channel.step = 20; // 20.0KHZ
            // index = 3;
            break;
        case 0:
            return;
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
    // uint8_t *hz = "12.5";
    while (!exit_flag)
    {
        // Create submenu
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " Brightness", " Timer"))
        {
        case 1:
            Brightness_callback();
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

void Brightness_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = 1;
    while (1)
    {
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  20", "  50", "  80", "  100"))
        {
        case 1:
            timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, 20);
            // index = 1;
            break;
        case 2:
            timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, 100);
            // index = 2;
            break;
        case 3:
            timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, 200);
            // index = 3;
            break;
        case 4:
            timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, 500);
            break;
        case 0:
            return;
        }
    }
}

void Timer_callback()
{
    submenu_item_t sub_menu;
    uint8_t submenu_list_num = 4;
    sub_menu.cur_item = 1;
    while (1)
    {
        switch (submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, "  10s", " 30S", " 1min", " 2min"))
        {
        case 1:
            Display_Timer.screen_off = 10; // 10s
            // index = 1;
            break;
        case 2:
            Display_Timer.screen_off = 30; // 20s
            // index = 2;
            break;
        case 3:
            Display_Timer.screen_off = 60; // 60s
            // index = 3;
            break;
        case 4:
            Display_Timer.screen_off = 120; // 120s
            break;
        case 0:
            return;
        }
    }
}

void cb3(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item3", " menu_item5", " menu_item6");
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
    jgfx_menu_set_title(&jgfx_menu, "Menu");
    jgfx_menu_set_title_height(&jgfx_menu, 20);
    // jgfx_menu_set_divide(&jgfx_menu, 10);

    jgfx_menu_append_text(&jgfx_menu, "Radio Setting", Radio_Settings);
    jgfx_menu_append_text(&jgfx_menu, "Display", Display_Settings);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing3", cb3);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing4", cb4);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing5", cb5);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing6", cb6);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing7", cb7);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing8", cb8);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing9", cb9);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing10", cb10);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing11", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing12", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing13", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing14", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing15", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing16", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing17", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing18", NULL);
    // jgfx_menu_append_text(&jgfx_menu, "Menu_Testing19", NULL);

    main_channel_init(&jgfx_channel);
    jgfx_menu_show(&jgfx_menu);

    corner_index_init(&jgfx_menu_corner);

    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);
    jgfx_set_font(JGFX_FONT_EN_8X16_BLOD);
    jgfx_draw_text_en(130, 5, "01");

    Display_Timer_Init(&Display_Timer);
}

void ui_menu_refresh(void)
{
}

void ui_menu_destory(void)
{
}

void ui_menu_event_cb(void)
{
    // static int i = 0;
    Display_Timer_count(&Display_Timer);
    corner_index_refresh(&jgfx_menu, &jgfx_menu_corner);
    // corner_index_flicker(&jgfx_menu,&jgfx_menu_corner);

    key_map_t key = key_get();
    if (key != KEY_MAP_NONE)
    {
        wakeup_screen(&Brigthtness_setting, &Display_Timer);
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