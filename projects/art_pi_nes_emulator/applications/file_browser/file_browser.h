#include "dfs.h"
#include "lv_hal_indev.h"

DIR * file_browser_init(lv_indev_t *indevp, lv_indev_type_t type);
void file_browser_run(DIR *rootp);