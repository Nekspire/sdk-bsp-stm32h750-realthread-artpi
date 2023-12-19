#include <emulator_ui.h>

#include <bitmap.h>

#define CANVAS_WIDTH 256 
#define CANVAS_HEIGHT 224 

uint32_t defualt_width = 0;
uint32_t defualt_height = 0;

lv_obj_t *ui_canvas;
lv_draw_img_dsc_t img_dsc;

lv_color_t canvas_palette[CANVAS_WIDTH];

void emulator_ui_init(uint32_t w, uint32_t h)
{
   static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];

   ui_canvas = lv_canvas_create(lv_scr_act());

   lv_canvas_set_buffer(ui_canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
   lv_obj_center(ui_canvas);

   lv_draw_img_dsc_init(&img_dsc);

   defualt_width = w;
   defualt_height = h;
}

void emulator_ui_bitmap_draw(bitmap_t *bmp)
{
   for (size_t i = 0; i < defualt_height; i++)
   {
      lv_canvas_copy_buf(ui_canvas, bmp->line[i], 0, i, defualt_width, 1);
   };
}

void emulator_ui_bitmap_clear(uint8_t color)
{
   lv_canvas_fill_bg(ui_canvas, lv_color_black(), LV_OPA_100);
}