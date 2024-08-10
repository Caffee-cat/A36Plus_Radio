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

void cb1(jgfx_menu_ptr ptr)
{
    // The original method of creating a new object has fail.function malloc return NULL.
    //  jgfx_menu_t m;
    //  jgfx_menu_init(&m);
    //  jgfx_menu_set_position(&m, 50, 80);
    //  jgfx_menu_set_size(&m, 50, 50);
    //  jgfx_menu_append_text(&m, "Submenu1", NULL);
    //  jgfx_menu_append_text(&m, "Submenu2", NULL);
    //  jgfx_menu_append_text(&m, "Submenu3", NULL);
    //  jgfx_menu_show(&m);

    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item1", " menu_item2", " menu_item3");
}

void cb2(jgfx_menu_ptr ptr)
{
    submenu_item_t sub_menu;
    uint8_t index = 1, submenu_list_num = 3;
    submenu_item_show(&jgfx_menu, submenu_list_num, &sub_menu, " menu_item2", " menu_item5", " menu_item6");
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

    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing1", cb1);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing2", cb2);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing3", cb3);
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
    jgfx_menu_show(&jgfx_menu);

    corner_index_init(&jgfx_menu_corner);

    jgfx_set_color_back_hex(0x0000);
    jgfx_fill_react(130, 5, 16, 16);
    jgfx_set_font(JGFX_FONT_EN_8X16_BLOD);
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
    // static int i = 0;

    corner_index_refresh(&jgfx_menu, &jgfx_menu_corner);
    // corner_index_flicker(&jgfx_menu,&jgfx_menu_corner);

    key_map_t key = key_get();
    if (key != KEY_MAP_NONE)
    {
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
            // RAM's space is not enough,the program haven't run in this func,but it bloom.
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