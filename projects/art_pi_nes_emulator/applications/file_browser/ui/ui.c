#include "ui.h"
#include "lvgl.h"
#include "lv_btn.h"
#include "lv_port_indev.h"

lv_obj_t *ui_screen;
lv_obj_t *ui_label;
lv_obj_t *ui_panel;
lv_obj_t *ui_list;
lv_obj_t *ui_list_curr_button;
uint32_t ui_list_cnt;

static void event_key_handler(lv_event_cb_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (ui_list_curr_button == NULL) 
        return;

    uint32_t index = lv_obj_get_index(ui_list_curr_button);

    if (event_code == LV_EVENT_KEY && lv_event_get_key(e) == LV_KEY_UP)
    {
        if (index != 0)
        {
            lv_obj_move_to_index(ui_list_curr_button, index - 1);
            lv_obj_scroll_to_view(ui_list_curr_button, LV_ANIM_ON);
        }
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key(e) == LV_KEY_DOWN)
    {
        if (index != ui_list_cnt)
        {
            lv_obj_move_to_index(ui_list_curr_button, index + 1);
            lv_obj_scroll_to_view(ui_list_curr_button, LV_ANIM_ON);
        }
    }
}

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

    for (size_t i = 0; i < 4; i++)
    {
        ui_dir_add_entity(UI_LIST_ENTITY_DIR, "DIR X");
    }


    ui_list_cnt = lv_obj_get_child_cnt(ui_list);
    ui_list_curr_button = lv_obj_get_child(ui_list, 0);

    lv_group_t *group = lv_group_create();
    lv_group_add_obj(group, ui_list_curr_button);
    lv_indev_set_group(indev, group);

    lv_obj_add_state(ui_list_curr_button, LV_STATE_CHECKED);
    lv_obj_add_event_cb(ui_list_curr_button, event_key_handler, LV_EVENT_ALL, NULL);
}