#ifndef _EMULATOR_UI_H_
#define _EMULATOR_UI_H_

#include <lvgl.h>

void emulator_ui_init(uint32_t w, uint32_t h);
void emulator_ui_bitmap_draw(bitmap_t *bmp);
void emulator_ui_bitmap_clear(uint8_t color);

#endif