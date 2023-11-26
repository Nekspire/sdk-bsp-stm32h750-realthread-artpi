#include "file_browser.h"
#include "ui.h"
#include "rtthread.h"

DIR *file_browser_init(void)
{
    static DIR *dir;

    ui_init();

    /* Probe DFS  - open root directory */
    dir = opendir("/");

    if (NULL != dir)
    {
        closedir(dir);
    }

    return dir;
}

DIR *file_browser_open_dir(DIR *dir, const char *path)
{
    struct dirent *d;

    if (NULL != dir)
    {
        dir = opendir(path);

        if (NULL != dir)
        {
            ui_set_path(path);
            ui_dir_clear();

            while ((d = readdir(dir)) != RT_NULL)
            {
                if (FT_DIRECTORY == d->d_type)
                {
                    ui_dir_add_entity(UI_LIST_ENTITY_DIR, d->d_name);
                }
                else
                {
                    ui_dir_add_entity(UI_LIST_ENTITY_FILE, d->d_name);
                }
            }
            closedir(dir);
        }

    }

    return dir;
}

void lv_file_browser_init(void)
{
    DIR *dir = file_browser_init();

    if (NULL != dir)
    {
        file_browser_open_dir(dir, "/");
    }
    else
    {
        rt_kprintf("Cannot open data storage!");
    }
}