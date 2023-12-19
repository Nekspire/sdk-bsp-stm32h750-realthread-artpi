#include <rtthread.h>

#include <osd.h>
#include <nes.h>

#include <video.h>

#include <emulator_ui.h>

bitmap_t *bitmap_p;
static char bitmap_buff[1]; // dummy
uint16 palette[256];

static int init(int width, int height);
static void shutdown(void);
static int set_mode(int width, int height);
static void set_palette(rgb_t *pal);
static void clear(uint8 color);
static bitmap_t *lock_write(void);
static void free_write(int num_dirties, rect_t *dirty_rects);
static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects);

viddriver_t vid_drv = 
{
   "Custom Video Driver",
   init,
   shutdown,
   set_mode,
   set_palette,
   clear,
   lock_write,
   free_write,
   custom_blit,
   false
};

/* set a video mode */
static int set_mode(int width, int height)
{
   emulator_ui_init(width, height);
   return 0;
}

/* initialise video */
static int init(int width, int height)
{
   set_mode(width, height);
}

/* squash memory leaks */
static void shutdown(void)
{
   /* TODO */
}

/* copy nes palette over to hardware */
static void set_palette(rgb_t *pal)
{
   for (uint32_t i = 0; i < 256; i++)
   {
      palette[i] = (pal[i].b>>3)+((pal[i].g>>2)<<5)+((pal[i].r>>3)<<11);
   }
}

/* clear all frames to particular color */
static void clear(uint8 color)
{
   emulator_ui_bitmap_clear(color);
}

/* acquire the directbuffer for writing */
static bitmap_t *lock_write(void)
{
   bitmap_p = bmp_createhw((uint8*) bitmap_buff, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_WIDTH * 2);

   return bitmap_p;
}

/* release the resource */
static void free_write(int num_dirties, rect_t *dirty_rects)
{
   bmp_destroy(&bitmap_p);
}

static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects)
{
   emulator_ui_bitmap_draw(bmp);
}

viddriver_t * video_get_driver()
{
   return &vid_drv;
}