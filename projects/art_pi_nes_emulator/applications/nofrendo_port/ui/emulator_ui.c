#include <emulator_ui.h>

#include <video.h>
#include <bitmap.h> 

lv_obj_t *ui_screen1;
lv_obj_t *ui_image;

uint8_t img_buf[DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(lv_color_t)] = {0u};

static lv_img_dsc_t img_dsc = {
  .header.always_zero = 0,
  .header.w = DEFAULT_WIDTH,
  .header.h = DEFAULT_HEIGHT,
  .data_size = DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(lv_color_t),
  .header.cf = LV_IMG_CF_TRUE_COLOR,
};

void emulator_ui_init(uint32_t w, uint32_t h)
{
   img_dsc.data = img_buf;

   ui_screen1 = lv_obj_create(NULL);
   lv_scr_load(ui_screen1);

   ui_image = lv_img_create(ui_screen1);
   lv_img_set_src(ui_image, &img_dsc);
   lv_obj_set_size(ui_image, DEFAULT_WIDTH, DEFAULT_HEIGHT);
   lv_obj_center(ui_image);
}

void emulator_ui_bitmap_draw(bitmap_t *bmp, rgb_t *palette)
{
   lv_color_t c;

   if (RT_NULL != img_buf)
   {
      for (size_t i = 0; i < DEFAULT_HEIGHT; i++)
      {
         for (size_t j = 0; j < DEFAULT_WIDTH; j++)
         {
            c.ch.red = (uint16_t) palette[bmp->line[i][j]].r;
            c.ch.green = (uint16_t) palette[bmp->line[i][j]].g;
            c.ch.blue = (uint16_t) palette[bmp->line[i][j]].b;

            lv_img_buf_set_px_color(&img_dsc, j , i, c);
         }
      }
      lv_img_set_src(ui_image, &img_dsc);
   }
}

void emulator_ui_bitmap_clear(uint8_t color)
{
   rt_memset(img_buf, 0x00, DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(lv_color_t));
   lv_img_set_src(ui_image, &img_dsc);
}