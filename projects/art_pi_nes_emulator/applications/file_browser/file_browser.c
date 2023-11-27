#include "file_browser.h"
#include "ui.h"
#include "rtthread.h"

char *pathp = NULL;
uint32_t path_cnt = 0;
uint32_t path_size = 0;
uint32_t dir_cnt = 0;
long dir_old_index = 0;

static void path_add_dir(const char* txt)
{
    path_size = path_size + strlen(txt);

    if (0 != path_cnt)
    {
        path_size += strlen("/");
    }

    pathp = (char *) realloc(pathp, (path_size * sizeof(char)));

    if (0 != path_cnt)
    {
        strcat(pathp, "/");
    }
    
    strcat(pathp, txt);
    path_cnt++;
}

static void path_remove_dir()
{
    path_cnt--;   
}

static void file_browser_read_dir(DIR* dirp)
{
    struct dirent *entp;

    if (NULL != dirp)
    {
        ui_set_path(pathp);
        ui_dir_clear();

        rewinddir(dirp);
        while ((entp = readdir(dirp)) != RT_NULL)
        {
            if (FT_DIRECTORY == entp->d_type)
            {
                ui_dir_add_entity(UI_LIST_ENTITY_DIR, entp->d_name);
            }
            else
            {
                ui_dir_add_entity(UI_LIST_ENTITY_FILE, entp->d_name);
            }
        }
        rewinddir(dirp);
    }
}

DIR * file_browser_init(void)
{ 
    static DIR *rootp;

    /* Initialize file browser ui */
    ui_init();

    /* Reserve memory for root path string */
    path_size += sizeof("/");
    pathp = (char *) malloc(path_size);

    if (NULL != pathp)
    {
        /* Set root path string */
        strcpy(pathp, "/");
        /* Probe DFS  - open root directory */
        rootp = opendir(pathp);

        if (NULL != rootp)
        {
            file_browser_read_dir(rootp);
        }
        else
        {
            free(pathp);
        }
    }
    
    return rootp;
}

DIR gdir;

void file_browser_run(DIR *rootp)
{ 
    struct dirent *dp;

    switch (ui_event)
    {
        case UI_EVENT_NONE:

            break;
        case UI_EVENT_DOWN:
            
            if (0 == path_cnt)
            {
                /* use root pointer */
                dp = readdir(rootp);   
            }
            else
            {
                dp = readdir(&gdir);  
            }

            if (dp == NULL)
            {
                /* code */
            }
            else
            {
                rt_kprintf("[file_browser_run] UI_EVENT_DOWN %s\n", dp->d_name);
            }
            
            ui_event = UI_EVENT_NONE;

            break;
        case UI_EVENT_UP:
        
            ui_event = UI_EVENT_NONE;

            break;
        case UI_EVENT_OPEN:

            if (0 == path_cnt)
            {
                /* use root pointer */
                dp = readdir(rootp);
            }
            else
            {
                dp = readdir(&gdir);
            }
            
            if (NULL != dp)
            {
                rt_kprintf("[file_browser_run] UI_EVENT_OPEN %s\n", dp->d_name);
                if (FT_DIRECTORY == dp->d_type)
                {
                    path_add_dir(dp->d_name);

                    DIR *p = opendir(pathp);
                    
                    if (NULL != p)
                    {
                        gdir = *p;
                    }

                    file_browser_read_dir(&gdir);
                }
                else
                {
                    /* TODO */ 
                }
            }

            ui_event = UI_EVENT_NONE;

            break;
        
        case UI_EVENT_CLOSE:

            break;

        default:
            break;
    }
}