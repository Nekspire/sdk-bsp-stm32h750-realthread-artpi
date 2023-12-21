#ifndef _FILE_BROWSER_H
#define _FILE_BROWSER_H

#include "dfs.h"
#include "lvgl.h"

typedef struct 
{
   void (*fopen_cb)();
   void *fopen_read_buffer;
   uint32_t fopen_read_buff_len;
   uint32_t fopen_size;
   char *fopen_path;
} file_browser_file_t;

DIR * file_browser_init(lv_indev_t *indevp, lv_indev_type_t type, lv_event_cb_t *eventp);
void file_browser_dir_next(DIR *rootp);
void file_browser_dir_prev(DIR *rootp);
void file_browser_open(DIR *rootp, file_browser_file_t *f_ctx);
void file_browser_dir_close(DIR *rootp);

#endif