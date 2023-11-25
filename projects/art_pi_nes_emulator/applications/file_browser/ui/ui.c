#include "ui.h"
#include "lvgl.h"
#include "lv_btn.h"

lv_obj_t *ui_screen;
lv_obj_t *ui_label;
lv_obj_t *ui_panel;
lv_obj_t *ui_list;

void ui_set_path(const char *path)
{
    lv_label_set_text(ui_label, path);
}

void ui_dir_add_entity(ui_list_entity_t type, const char *name)
{
    lv_obj_t *ui_list_button;

    switch (type)
    {
    case UI_LIST_ENTITY_FILE:
        ui_list_button = lv_list_add_btn(ui_list, LV_SYMBOL_FILE, name);
        break;
    case UI_LIST_ENTITY_DIR:
        ui_list_button = lv_list_add_btn(ui_list, LV_SYMBOL_DIRECTORY, name);    
    default:
        break;
    }

    lv_obj_set_size(ui_list_button, lv_pct(100), 50);
}

void ui_dir_clear()
{
    lv_obj_del(ui_list);

    ui_list = lv_list_create(ui_screen);

    lv_obj_set_align(ui_list, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(ui_list, lv_pct(100), 430);
    lv_obj_set_scrollbar_mode(ui_list, LV_SCROLLBAR_MODE_AUTO);
}

void ui_init(void)
{
    ui_screen = lv_obj_create(NULL);
    ui_panel = lv_obj_create(ui_screen);
    ui_label = lv_label_create(ui_panel);
    ui_list = lv_list_create(ui_screen);

    lv_scr_load(ui_screen);
    lv_obj_set_layout(ui_screen, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(ui_screen, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_size(ui_panel, lv_pct(100), 50);
    lv_obj_set_align(ui_panel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_scrollbar_mode(ui_panel, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_align(ui_label, LV_ALIGN_OUT_LEFT_MID);

    lv_obj_set_align(ui_list, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(ui_list, lv_pct(100), 430);
    lv_obj_set_scrollbar_mode(ui_list, LV_SCROLLBAR_MODE_AUTO);

    ui_set_path("/SDCARD/PATH TO FILE");

    for (size_t i = 0; i < 2; i++)
    {
        ui_dir_add_entity(UI_LIST_ENTITY_FILE, "FILE X");
    }
    
    for (size_t i = 0; i < 2; i++)
    {
        ui_dir_add_entity(UI_LIST_ENTITY_DIR, "DIR X");
    }

    for (size_t i = 0; i < 2; i++)
    {
        ui_dir_add_entity(UI_LIST_ENTITY_FILE, "FILE X");
    }

    for (size_t i = 0; i < 2; i++)
    {
        ui_dir_add_entity(UI_LIST_ENTITY_DIR, "DIR X");
    }

    ui_dir_clear();
}