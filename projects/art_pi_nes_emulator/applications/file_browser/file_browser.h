#ifndef _FILE_BROWSER_H
#define _FILE_BROWSER_H

#include "dfs.h"
#include "lvgl.h"

DIR * file_browser_init(lv_indev_t *indevp, lv_indev_type_t type, lv_event_cb_t *eventp);
void file_browser_dir_next(DIR *rootp);
void file_browser_dir_prev(DIR *rootp);
void file_browser_open(DIR *rootp, void (*fopen_cb)(), void *buff, uint32_t len);
void file_browser_dir_close(DIR *rootp);

#endif