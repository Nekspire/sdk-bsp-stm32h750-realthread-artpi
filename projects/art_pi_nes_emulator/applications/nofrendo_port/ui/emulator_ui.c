#include <emulator_ui.h>

#include <bitmap.h>
#include <math.h>

#if FULL_SCREEN
#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 480
#else
#define WIDTH_FACTOR   3
#define HEIGHT_FACTOR  2
#define DEFAULT_WIDTH  (256 * WIDTH_FACTOR)
#define DEFAULT_HEIGHT (224 * HEIGHT_FACTOR)
#endif

lv_obj_t *ui_screen1;
lv_obj_t *ui_image;

volatile uint8_t *img_buf; // image buffer fill be saved in external ram

static lv_img_dsc_t img_dsc = {
    .header.always_zero = 0,
    .header.w = DEFAULT_WIDTH,
    .header.h = DEFAULT_HEIGHT,
    .data_size = DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(lv_color_t),
    .header.cf = LV_IMG_CF_TRUE_COLOR,
};

void emulator_ui_init(uint32_t w, uint32_t h)
{
   img_buf = (uint8_t *) 0xc0400800;
   img_dsc.data = img_buf;

   ui_screen1 = lv_obj_create(NULL);
   lv_scr_load(ui_screen1);

   ui_image = lv_img_create(ui_screen1);
   lv_img_set_src(ui_image, &img_dsc);
   lv_obj_set_size(ui_image, DEFAULT_WIDTH, DEFAULT_HEIGHT);

#if FULL_SCREEN
   lv_obj_set_x(ui_image, 0);
   lv_obj_set_y(ui_image, 0);
#else
   static lv_style_t style;
   lv_style_init(&style);
   lv_style_set_bg_color(&style, lv_color_black());
   lv_obj_add_style(ui_screen1, &style, 0);
   lv_obj_center(ui_image);
#endif
}

static inline uint32_t calc_index(size_t index, double factor)
{
   return (uint32_t)(round((double) index / factor));
}

void emulator_ui_bitmap_draw(bitmap_t *bmp, rgb_t *palette)
{
   lv_color_t c;

   for (size_t x = 0; x < DEFAULT_WIDTH; x++)
   {
      for (size_t y = 0; y < DEFAULT_HEIGHT; y++)
      {      
#if FULL_SCREEN
         c.full = (palette[bmp->line[calc_index(y, 2.145)][calc_index(x, 3.125)]].b >> 3) + 
                  ((palette[bmp->line[calc_index(y, 2.145)][calc_index(x, 3.125)]].g >> 2) << 5) +
                  ((palette[bmp->line[calc_index(y, 2.145)][calc_index(x, 3.125)]].r >> 3) << 11);
#else
         c = lv_color_make((uint8_t) palette[bmp->line[y / HEIGHT_FACTOR][x / WIDTH_FACTOR]].r,
                           (uint8_t) palette[bmp->line[y / HEIGHT_FACTOR][x / WIDTH_FACTOR]].g,
                           (uint8_t) palette[bmp->line[y / HEIGHT_FACTOR][x / WIDTH_FACTOR]].b);
#endif
         lv_img_buf_set_px_color(&img_dsc, x, y, c);
      }
   }
   lv_img_set_src(ui_image, &img_dsc);
}

void emulator_ui_bitmap_clear(uint8_t color)
{
   rt_memset(img_buf, 0x00, DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(lv_color_t));
   lv_img_set_src(ui_image, &img_dsc);
}