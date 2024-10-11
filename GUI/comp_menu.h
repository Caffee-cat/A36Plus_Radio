#ifndef __COMP_MENU_JAMIEXU_H__
#define __COMP_MENU_JAMIEXU_H__
#include "main.h"
#include "ui.h"
// #include "radio.h"
#define JGFX_MENU_STR_MAX_LEN 10

typedef struct jgfx_menu_item_t *jgfx_menu_item_ptr;
typedef struct jgfx_menu_t *jgfx_menu_ptr;
typedef struct submenu_item_t *submenu_item_ptr;
typedef struct corner_index_num_t *corner_index_num_ptr;
typedef void (*jgfx_menu_item_event_cb)(jgfx_menu_ptr);
typedef struct sub_channel_t *sub_channel_ptr;
typedef struct ui_main_channel_t *ui_main_channel_ptr;
typedef struct Display_Timer_t *Display_Timer_ptr;
typedef struct Brightness_setting_t *Brightness_setting_ptr;

typedef enum
{
    MENU_RADIO_SETTING = 0x00,
    MENU_SQUELCH,
    MENU_CHANNEL_SAVE,
    MENU_CHANNEL_SELECT,
    MENU_CHANNEL_DELETE,
    MENU_DISPLAY,
    MENU_BANKS,
    MENU_BANDWIDTH,
    MENU_BUTTON,
    MENU_DTMF,
}menu_item_ID_t;

typedef enum
{
    JGFX_MENU_TYPE_TXT = 0x00,
    JGFX_MENU_TYPE_COMBOBOX,
    JGFX_MENU_TYPE_SWITCH
} jgfx_menu_item_type_t;

typedef enum
{
    JGFX_MENU_STATUS_SELECTED = 0x00,
    JGFX_MENU_STATUS_UNSELECTED,
    JGFX_MENU_STATUS_CLICKED,
    JGFX_MENU_STATUS_SHUMENU,
} jgfx_menu_status_t;

typedef enum
{
    JGFX_MENU_ITEM_STATUS_SELECTED = 0x00,
    JGFX_MENU_ITEM_STATUS_UNSELECTED,
    JGFX_MENU_ITEM_STATUS_CLICKED,
} jgfx_menu_item_status_t;

// typedef enum{
//     JGFX_MENU_STATUS_
// }

// typedef enum
// {
//     MAIN_CHANNEL_NONE = 0x00,
//     MAIN_CHANNEL_10KHZ,
//     MAIN_CHANNEL_15KHZ,
//     MAIN_CHANNEL_20KHZ
// } radio_channel_step_t;

typedef enum
{
    ADD_OFFSET_CANCEL = 0x00,
    ADD_OFFSET_SUCCESS,
    ADD_OFFSET_ERROR
} jgfx_add_channel_status_t;

typedef enum
{
    NONE_CHANNEL_SPEAKING = 0x00,
    CHANNEL_SPEAKING,
    CHANNAL_A_SPEAKING,
    CHANNEL_B_SPEAKING,
    CTDCSS_INCORRENT
} main_channel_speak_t;

typedef enum
{
    OFF = 0x00,
    ADDITION,
    SUBTRACTION
} offset_direction_t;

// typedef enum
// {
//     PF_OFF = 0x00,
//     PF_DTMF,
//     PF_NOAA,
//     PF_FM
// }main_PF_select_t;

typedef struct jgfx_menu_item_t
{
    jgfx_menu_item_type_t item_type;
    jgfx_menu_ptr parent;
    uint8_t item_id;

    uint8_t item_name[JGFX_MENU_STR_MAX_LEN];
    uint8_t *item_img_data;

    uint8_t **combox_menu;
    uint8_t combox_index;

    uint8_t **switch_item;
    uint8_t switch_status;

    jgfx_menu_item_ptr item_pre;
    jgfx_menu_item_ptr item_next;

    jgfx_menu_item_event_cb item_event_cb;

} jgfx_menu_item_t;

typedef struct
{
    uint8_t menu_name[MENU_NAME_LEN_LIM];
    bool show_channel_pic;
    menu_item_ID_t menu_id;
}menu_list_t;

typedef struct jgfx_menu_t
{
    uint16_t menu_x;
    uint16_t menu_y;
    uint16_t menu_width;
    uint16_t menu_height;

    uint8_t menu_item_width;
    uint8_t menu_item_height;

    uint8_t menu_divide_height;

    uint8_t menu_use_tiltle;
    uint8_t menu_title[JGFX_MENU_STR_MAX_LEN];
    uint8_t menu_title_height;

    menu_list_t menu_item[15];
    uint8_t index;
    uint8_t item_size;
    uint8_t item_show_num;
    uint8_t page;
    uint8_t Initial_flag;
    bool menu_draw_completed;
    jgfx_menu_status_t status;

    jgfx_menu_item_ptr head_item;
    jgfx_menu_item_ptr cur_item;
    ui_main_channel_ptr channel_ptr;
} jgfx_menu_t;

typedef struct submenu_item_t
{
    uint8_t *item_name[80];
    uint16_t line_height;
    uint8_t itemlist_num;       //Count from 1
    uint8_t cur_item;           //Count from 1
    uint8_t cur_page;           //Count from 1
    uint8_t num_in_page;        //Count from 1
    bool    show_icon[10];
    bool    channel_select;
} submenu_item_t;

typedef struct corner_index_num_t
{
    uint8_t index_num1;
    uint8_t index_num2;
    uint16_t index_jump_count1;
    uint16_t index_jump_count2;
    void (*refresh)(void);
} corner_index_num_t;

typedef enum
{
    TXP_STANDBY,
    TXP_HIGH,
    TXP_MID,
    TXP_LOW
}Tx_Power_t;

typedef struct sub_channel_t
{
    uint32_t frequency;
    uint16_t Tx_CTCSS;
    uint16_t Rx_CTCSS;
    uint32_t Tx_CDCSS;
    uint32_t Rx_CDCSS;
    uint8_t channnel_bandwidth;
    
    uint16_t offset;
    Tx_Power_t power;
    offset_direction_t direction;
} sub_channel_t;

typedef struct ui_main_channel_t
{

    sub_channel_t channel_1;
    sub_channel_t channel_2;

    uint8_t sql;
    uint8_t DTMF_UPCode[20];
    uint8_t DTMF_DownCode[20];
    bool DTMF_up_enable;
    bool DTMF_dowm_enable;
    bool ANI_enable;
    bool ANI_Receive;

    // for main channel input logic
    uint32_t ch_bak;
    sub_channel_t *cur_channel;

    // for submenu cursor
    uint8_t cur_index;
    uint8_t SFT_D_index;
    uint8_t TxPower_index;
    uint8_t PF1;    //1: DTMF 2: NOAA
    uint8_t PF2;
    uint8_t TopKey;

    const float *ch_pra;
    const float *ch_val;

    uint8_t block_height1;
    uint8_t block_height2;
    uint8_t block_width;
    // count for flicker
    uint16_t flash_count_num1;
    uint16_t flash_count_num2;
    float step;
    bool channel;      // 1:A,0:B
    bool dual_channel; // 1:A,0:B
    bool Initial_flag;
    bool channel_changed; // FALSE:not , TRUE:yes
    bool channel_listening;
} ui_main_channel_t;

typedef struct Brightness_setting_t
{
    uint8_t cur_bri;         // as a parameter for submenu index
    const uint32_t *bri_pra; // point to the configurable parameters
    const uint32_t *bri_val; // point to the current brightness setting
} Brightness_setting_t;

typedef struct Display_Timer_t
{
    uint16_t Timer_count;
    uint16_t Timer_limit;
    uint16_t Second_count;
    uint8_t index;
    uint8_t screen_off;
    bool Timer_init_flag;

    const uint16_t *Tim_pra;
    const uint16_t *Tim_val;
} Display_Timer_t;

extern menu_list_t menu_item[];
extern uint32_t *cur_ch, ch1, ch2;


void _r_clear_item_area(jgfx_menu_ptr menu_ptr);

void Startup_display(void);

void jgfx_menu_init(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr main_channel);

void jgfx_menu_set_size(jgfx_menu_ptr menu_ptr, uint16_t width, uint16_t height);

void jgfx_menu_set_item_size(jgfx_menu_ptr menu_ptr, uint16_t width, uint16_t height);

void jgfx_menu_set_position(jgfx_menu_ptr menu_ptr, uint16_t x, uint16_t y);

void jgfx_menu_set_divide(jgfx_menu_ptr menu_ptr, uint8_t height);

void jgfx_menu_append_text(jgfx_menu_ptr menu_ptr, uint8_t *str, jgfx_menu_item_event_cb cb);

void jgfx_menu_show(jgfx_menu_ptr menu_ptr);

void jgfx_menu_set_title(jgfx_menu_ptr menu_ptr, uint8_t *str);

void jgfx_menu_set_title_height(jgfx_menu_ptr menu_ptr, uint8_t height);

void jgfx_menu_item_next(jgfx_menu_ptr menu_ptr);

void jgfx_menu_item_previous(jgfx_menu_ptr menu_ptr);

void jgfx_menu_update(jgfx_menu_ptr menu_ptr);

void jgfx_menu_click(jgfx_menu_ptr menu_ptr);

void key_press_delay(void);

bool jgfx_menu_index(jgfx_menu_ptr menu_ptr, uint8_t index);

void jgfx_menu_destory(jgfx_menu_ptr menu_ptr);

uint8_t submenu_item_show(jgfx_menu_ptr menu_ptr, uint8_t item_num, submenu_item_ptr submenu_ptr, ...);

uint8_t submenu_item_show_with_array(jgfx_menu_ptr menu_ptr, uint8_t item_num, submenu_item_ptr submenu_ptr, uint8_t (*string)[]);

void render_submenu_with_param(jgfx_menu_ptr menu_ptr, uint8_t *string, uint8_t index);

void index_num_display(jgfx_menu_ptr menu_ptr);

void menu_item_param_init(jgfx_menu_ptr menu_ptr,ui_main_channel_ptr main_channel);

void corner_index_init(corner_index_num_ptr corner_ptr);

void corner_index_refresh(jgfx_menu_ptr menu_ptr, corner_index_num_ptr corner_ptr);

void corner_index_flicker(jgfx_menu_ptr menu_ptr, corner_index_num_ptr corner_ptr);

uint8_t index_num_cb(corner_index_num_ptr corner_ptr, jgfx_menu_ptr menu_ptr, uint8_t key);

void radio_channel_change(ui_main_channel_ptr channel_ptr, uint8_t step);

void main_channel_speaking(ui_main_channel_ptr channel_ptr);

void main_PTT_transmit(ui_main_channel_ptr channel_ptr);

void main_DTMF_init(ui_main_channel_ptr channel_ptr);

void main_DTMF_input(ui_main_channel_ptr channel_ptr);

void main_DTMF_input_MENU(ui_main_channel_ptr channel_ptr,uint8_t Direction);

void channel_input_flicker(ui_main_channel_ptr channel_ptr, uint8_t state);

void channel_store(ui_main_channel_ptr channel_ptr);

void offset_direction(ui_main_channel_ptr channel_ptr, uint8_t param);

// void bk4819_Tx_Power(Tx_Power_t power);

void TxPower_change(ui_main_channel_ptr channel_ptr, uint8_t param);

void frequency_scan(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr);

void TxAmplifier_enable(ui_main_channel_ptr channel_ptr);

void channel_offset_preload(ui_main_channel_ptr channel_ptr);

void channel_offset_unload(ui_main_channel_ptr channel_ptr);

void channel_TxCTCSS_change(ui_main_channel_ptr channel_ptr, uint8_t prarm);

void channel_RxCTCSS_change(ui_main_channel_ptr channel_ptr, uint8_t prarm);

void channel_TxCDCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param);

void channel_RxCDCSS_change(ui_main_channel_ptr channel_ptr, uint8_t param);

void return_to_menu(jgfx_menu_ptr menu_ptr);

void Brightness_init(Brightness_setting_ptr bri_ptr);

void Brightness_change(Brightness_setting_ptr bri_ptr, uint8_t bri_num);

void Display_Timer_Init(Display_Timer_ptr Timer_ptr);

void Display_Timer_count(Display_Timer_ptr Timer_ptr);

void Display_Timer_change(Display_Timer_ptr Timer_ptr, uint8_t Tim_num);

void wakeup_screen(Brightness_setting_ptr Brightness_ptr, Display_Timer_ptr Timer_ptr);

void input_window_init(jgfx_menu_ptr menu_ptr, ui_main_channel_ptr channel_ptr);

void offset_setting(ui_main_channel_ptr channel_ptr);

void channel_offset_draw(uint32_t input_channel);

void draw_info(jgfx_add_channel_status_t status);

void bk4819_TxCTDCSS_set_auto(ui_main_channel_ptr channel_ptr);

void PF1_funtion_change(ui_main_channel_ptr channel_ptr, uint8_t param);

void key_press_function(ui_main_channel_ptr channel_ptr, uint8_t key_press);

void channel_bandwidth_change(ui_main_channel_ptr channel_ptr, uint8_t param);

void channel_squelch_change(ui_main_channel_ptr channel_ptr, uint8_t param);

static void _l_destory_menu_item(jgfx_menu_item_ptr item);

static void _r_menu_draw_items(jgfx_menu_ptr menu_ptr, uint8_t order);

static void _l_menu_append_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr);

static void _r_menu_draw_selector(jgfx_menu_ptr menu_ptr);

static void _r_menu_draw_item(jgfx_menu_ptr menu_ptr, jgfx_menu_item_ptr item_ptr, jgfx_menu_item_status_t status, uint8_t index);


static void submenu_init(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr, uint8_t num);

static void submenu_items_refresh(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr);

static uint8_t submenu_cb(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr);


static void submenu_show_channel_icon(jgfx_menu_ptr menu_ptr, submenu_item_ptr submenu_ptr);

#endif
