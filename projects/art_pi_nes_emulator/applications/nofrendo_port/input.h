#ifndef _INPUT_H_
#define _INPUT_H_

#include "lvgl.h"

extern void input_lv_event_key_handler_cb(lv_event_cb_t *e);

int input_joypad_get_event(void);
void input_init();

#endif