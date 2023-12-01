#include "ui.h"
#include "lvgl.h"
#include "lv_btn.h"

#define UI_STYLE1_BG_COLOR 0x0000

const lv_indev_type_t UI_SUPPORTED_INDEV[] = {LV_INDEV_TYPE_KEYPAD};

const lv_style_const_prop_t ui_style_props[] = 
{
   LV_STYLE_CONST_BG_COLOR(UI_STYLE1_BG_COLOR),
   LV_STYLE_CONST_BORDER_COLOR(UI_STYLE1_BG_COLOR),
   LV_STYLE_CONST_RADIUS(0)
};

LV_STYLE_CONST_INIT(ui_style_widgets_default, ui_style_props);

lv_indev_t *ui_indevp;

lv_obj_t *ui_screen;
lv_obj_t *ui_label1;
lv_obj_t *ui_panel1;
lv_obj_t *ui_panel2;
lv_obj_t *ui_list;
lv_obj_t *ui_label3;
lv_obj_t *ui_label2;
lv_obj_t *ui_label_icon1;

lv_group_t *ui_group;

lv_event_cb_t *ui_list_button_event_cb;

void ui_set_path(const char *path)
{
    lv_label_set_text(ui_label1, path);
}

void ui_dir_add_entity(ui_list_entity_t type, const char *txt)
{
    lv_obj_t *ui_list_button;

    if (0 == lv_obj_get_child_cnt(ui_list))
    {
        ui_group = lv_group_create();
        lv_indev_set_group(ui_indevp, ui_group);
    }

    switch (type)
    {
    case UI_LIST_ENTITY_FILE:
        ui_list_button = lv_list_add_btn(ui_list, LV_SYMBOL_FILE, txt);
        break;
    case UI_LIST_ENTITY_DIR:
        ui_list_button = lv_list_add_btn(ui_list, LV_SYMBOL_DIRECTORY, txt);    
    default:
        break;
    }

    lv_obj_set_size(ui_list_button, lv_pct(100), 50);
    lv_obj_add_flag(ui_list_button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_text_color(ui_list_button, lv_color_white(), LV_PART_MAIN);
    lv_obj_add_style(ui_list_button, &ui_style_widgets_default, LV_PART_MAIN);

    lv_group_add_obj(ui_group, ui_list_button);
    lv_obj_add_event_cb(ui_list_button, ui_list_button_event_cb, LV_EVENT_ALL, NULL);
}

void ui_empty_dir_add_entity(void)
{
    lv_obj_t *ui_list_button;

    ui_group = lv_group_create();
    lv_indev_set_group(ui_indevp, ui_group);
    ui_list_button = lv_list_add_btn(ui_list, NULL, " ");
    lv_obj_set_size(ui_list_button, lv_pct(100), 50);
    lv_group_add_obj(ui_group, ui_list_button);
    lv_obj_add_event_cb(ui_list_button, ui_list_button_event_cb, LV_EVENT_ALL, NULL);
}

void ui_dir_clear()
{
    lv_obj_del(ui_list);

    ui_list = lv_list_create(ui_screen);

    lv_obj_set_align(ui_list, LV_ALIGN_LEFT_MID);
    lv_obj_set_size(ui_list, lv_pct(100), 370);
    lv_obj_set_scrollbar_mode(ui_list, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_add_style(ui_list, &ui_style_widgets_default, LV_PART_MAIN);
}

void ui_dir_add_count(const char *txt)
{
    lv_label_set_text(ui_label3, txt);
}

void ui_dir_focus_next(void)
{
    lv_group_focus_next(ui_group);
}

void ui_dir_focus_prev(void)
{
    lv_group_focus_prev(ui_group);
}

uint8_t ui_init(lv_indev_t *indevp, lv_event_cb_t *ep, lv_indev_type_t type)
{
    uint8_t ret = 1;

    if (NULL != indevp && NULL != ep)
    {
        ui_screen = lv_obj_create(NULL);
        ui_panel1 = lv_obj_create(ui_screen);
        ui_label_icon1 = lv_label_create(ui_panel1);
        ui_label1 = lv_label_create(ui_panel1);
        ui_list = lv_list_create(ui_screen);
        ui_panel2 = lv_obj_create(ui_screen);
        ui_label2 = lv_label_create(ui_panel2);
        ui_label3 = lv_label_create(ui_panel2);

        lv_scr_load(ui_screen);
        lv_obj_set_scrollbar_mode(ui_screen, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_style(ui_screen, &ui_style_widgets_default, LV_PART_MAIN);

        lv_obj_set_size(ui_panel1, lv_pct(100), 50);
        lv_obj_set_align(ui_panel1, LV_ALIGN_TOP_LEFT);
        lv_obj_set_scrollbar_mode(ui_panel1, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_layout(ui_panel1, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(ui_panel1, LV_FLEX_FLOW_ROW);
        lv_obj_add_style(ui_panel1, &ui_style_widgets_default, LV_PART_MAIN);

        lv_obj_set_align(ui_label_icon1, LV_ALIGN_LEFT_MID);
        lv_label_set_text(ui_label_icon1, LV_SYMBOL_DRIVE);
        lv_obj_set_style_text_color(ui_label_icon1, lv_color_white(), LV_PART_MAIN);

        lv_label_set_long_mode(ui_label1, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(ui_label1, lv_pct(100));
        lv_obj_set_align(ui_label1, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_text_color(ui_label1, lv_color_white(), LV_PART_MAIN); 

        lv_obj_set_align(ui_list, LV_ALIGN_TOP_LEFT);
        lv_obj_set_size(ui_list, lv_pct(100), 370);
        lv_obj_set_scrollbar_mode(ui_list, LV_SCROLLBAR_MODE_AUTO);
        lv_obj_add_style(ui_list, &ui_style_widgets_default, LV_PART_MAIN);

        lv_obj_set_align(ui_panel2, LV_ALIGN_BOTTOM_LEFT);
        lv_obj_set_size(ui_panel2, lv_pct(100), 50);
        lv_obj_set_scrollbar_mode(ui_panel2, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_layout(ui_panel2, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(ui_panel2, LV_FLEX_FLOW_ROW);
        lv_obj_add_style(ui_panel2, &ui_style_widgets_default, LV_PART_MAIN);

        lv_label_set_text(ui_label2, "Total:");
        lv_obj_set_align(ui_label2, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_text_color(ui_label2, lv_color_white(), LV_PART_MAIN); 

        lv_obj_set_align(ui_label3, LV_ALIGN_LEFT_MID);
        lv_obj_set_style_text_color(ui_label3, lv_color_white(), LV_PART_MAIN); 

        for (uint32_t i = 0; i < sizeof(UI_SUPPORTED_INDEV) / sizeof(lv_indev_type_t); i++)
        {
            if (type == UI_SUPPORTED_INDEV[i])
            {
                ui_indevp = indevp;
                ui_list_button_event_cb = ep;
                ret = 0;
                break;
            }
        }
    }

    return ret;
}