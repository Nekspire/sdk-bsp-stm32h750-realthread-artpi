#include "ui.h"
#include "lvgl.h"
#include "lv_btn.h"

void ui_init(void)
{
    lv_obj_t *ui_label;
    lv_obj_t *ui_panel;

    lv_obj_t *ui_screen1 = lv_obj_create(NULL);
    lv_scr_load(ui_screen1);

    lv_obj_t *ui_list1 = lv_list_create(ui_screen1);

    lv_obj_set_pos(ui_list1, 0, 48);
    lv_obj_set_size(ui_list1, 800, 480 - 48); // fix size

    lv_obj_set_layout(ui_list1, LV_LAYOUT_FLEX); // uneeded maybe
    lv_obj_set_flex_flow(ui_list1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(ui_list1, 5, 0);

    ui_panel = lv_obj_create(ui_screen1);
    lv_obj_set_pos(ui_panel, 0, 0);
    lv_obj_set_size(ui_panel, lv_pct(100), 47);

    ui_label = lv_label_create(ui_panel);
    lv_label_set_text(ui_label, "/SDCARD/PATH TO FILE");
    lv_obj_set_align(ui_label, LV_ALIGN_OUT_LEFT_MID);

    for (size_t i = 0; i < 9; i++)
    {
        lv_obj_t *ui_button;

        ui_button = lv_list_add_btn(ui_list1, LV_SYMBOL_FILE, "FILE X");
        lv_obj_set_size(ui_button, lv_pct(100), 43);
    }
}