#include "ui_menu.h"
#include "comp_menu.h"

#define MENU_MAX 3

ui_page_t ui_menu;
extern ui_stack_t ui_stack;
extern ui_page_ptr temp_page;
static jgfx_menu_t jgfx_menu;

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

void cb(jgfx_menu_ptr ptr)
{
    jgfx_menu_t m;
    jgfx_menu_init(&m);
    jgfx_menu_set_position(&m, 50, 80);
    jgfx_menu_set_size(&m, 50, 50);
    jgfx_menu_append_text(&m, "Submenu1", NULL);
    jgfx_menu_append_text(&m, "Submenu2", NULL);
    jgfx_menu_append_text(&m, "Submenu3", NULL);
    jgfx_menu_show(&m);
}

void ui_menu_init(void)
{
    jgfx_clear_screen();
    vTaskDelay(100);
    jgfx_menu_init(&jgfx_menu);
    jgfx_menu_set_size(&jgfx_menu, DISPLAY_W, DISPLAY_H);
    jgfx_menu_set_title(&jgfx_menu, "Menu");
    jgfx_menu_set_title_height(&jgfx_menu, 20);
    // jgfx_menu_set_divide(&jgfx_menu, 10);

    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing1", cb);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing2", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing3", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing4", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing5", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing6", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing7", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing8", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing9", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing10", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing11", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing12", NULL);
    jgfx_menu_append_text(&jgfx_menu, "Menu_Testing13", NULL);

    jgfx_menu_show(&jgfx_menu);
}

void ui_menu_refresh(void)
{
}

void ui_menu_destory(void)
{
}

void ui_menu_event_cb(void)
{
    key_map_t key = key_get();
    if (key != KEY_MAP_NONE)
    {
        if (key == KEY_MAP_1)
        {
            jgfx_menu_click(&jgfx_menu);
        }
        else if (key == KEY_MAP_2)
        {
            jgfx_menu_item_previous(&jgfx_menu);
        }
        else if (key == KEY_MAP_3)
        {
            jgfx_menu_item_next(&jgfx_menu);
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
        }else{
            jgfx_menu_index(&jgfx_menu, KEY_GET_NUM(key));
        }


        vTaskDelay(500);
    }
}