#include <rtthread.h>
#include <file_browser.h>
#include <lv_port_indev.h>
#include <nofrendo/nofrendo.h>

#define THREAD_PRIORITY 24
#define THREAD_STACK_SIZE 120000
#define THREAD_TIMESLICE 5

static rt_thread_t emulator_thread = RT_NULL;
static DIR *rootp;
static file_browser_file_t fb_file;
static uint8_t fb_file_buff[1024];
static bool start = false;


static void f_open_cb()
{
   char *fs;
   const char sufoption1[] = ".NES";
   const char sufoption2[] = ".nes";

   if (fb_file.fopen_file_name != NULL)
   {
      rt_kprintf("[f_open_cb] File name: %s\n", fb_file.fopen_file_name);
      rt_kprintf("[f_open_cb] File size: %u KB\n", fb_file.fopen_size / 1024);

      /* Case sensitive option 1 */
      fs = strstr(fb_file.fopen_file_name, sufoption1);

      if (fs == NULL)
      {
         /* Case sensitive option 2 */
         fs = strstr(fb_file.fopen_file_name, sufoption2);
      }
      if (fs == NULL)
      {
         rt_kprintf("[f_open_cb] It's not NES file!\n");
      }
      else
      {
         start = true;
      }
   }
}

static void event_key_handler_cb(lv_event_cb_t *e)
{
   lv_event_code_t event_code = lv_event_get_code((lv_event_t *)e);
   lv_obj_t *event_button = lv_event_get_current_target((lv_event_t *)e);

   if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *)e) == LV_KEY_DOWN)
   {
      file_browser_dir_next(rootp);
   }

   if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *)e) == LV_KEY_UP)
   {
      file_browser_dir_prev(rootp);
   }

   if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *)e) == LV_KEY_ENTER)
   {
      file_browser_open(rootp, &fb_file);
   }

   if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *)e) == LV_KEY_ESC)
   {
      file_browser_dir_close(rootp);
   }
}

/* Entry Function for Thread 1 */
static void emualator_thread_entry(void *parameter)
{
   fb_file.fopen_cb = f_open_cb;
   fb_file.fopen_read_buffer = fb_file_buff;
   fb_file.fopen_read_buff_len = sizeof(fb_file_buff);

   lv_port_indev_t lv_port_indev = lv_port_indev_init();
   rootp = file_browser_init(lv_port_indev.indevp, lv_port_indev.indev_drv.type, (lv_event_cb_t *)event_key_handler_cb);

   while (1)
   {
      if (start == true)
      {
         rt_kprintf("[f_open_cb] nofrendo started\n");
         nofrendo_main(NULL);
         rt_kprintf("[f_open_cb] nofrendo stopped\n");
         start = false;
      }
      
   }
}

int thread_init(void)
{
   emulator_thread = rt_thread_create("emulator",
                           emualator_thread_entry,
                           RT_NULL,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY,
                           THREAD_TIMESLICE);

   if (emulator_thread != RT_NULL)
   {
      rt_thread_startup(emulator_thread);
   }

   return 0;
}

INIT_ENV_EXPORT(thread_init);