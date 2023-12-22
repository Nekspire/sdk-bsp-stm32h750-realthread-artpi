#ifndef _EMULATOR_UI_H_
#define _EMULATOR_UI_H_

#define FULL_SCREEN  0

#include <lvgl.h>
#include <bitmap.h>

void emulator_ui_init(uint32_t w, uint32_t h);
void emulator_ui_bitmap_draw(bitmap_t *bmp, rgb_t *palette);
void emulator_ui_bitmap_clear(uint8_t color);

#endif