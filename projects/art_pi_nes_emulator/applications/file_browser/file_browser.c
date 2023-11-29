#include "file_browser.h"
#include "ui.h"
#include "rtthread.h"

char *pathp = NULL;
uint32_t path_cnt = 0;
uint32_t path_size = 0;
uint32_t dir_pos = 0;
uint32_t dir_offset = 0;
DIR gdir;

static void path_add_dir(const char* txt)
{
    path_size = path_size + strlen(txt);

    if (0 != path_cnt)
    {
        path_size += strlen("/");
    }

    pathp = (char *) realloc(pathp, (path_size * sizeof(char)));

    if (NULL != pathp)
    {
        if (0 != path_cnt)
        {
            strcat(pathp, "/");
        }

        strcat(pathp, txt);
        path_cnt++;
    }
}

uint8_t strcut(char *txt, uint32_t len)
{
    uint8_t ret = 0;

    uint32_t str_len = strlen(txt);
    
    if (str_len >= len)
    {
        for (int i = len; i >= 0; i--)
        {
            if (i == 0)
            {
                txt[str_len] = '\0';
            }
            else
            {
                txt[str_len] = 0;
            }
            str_len--;
        }
    }
    else
    {
        ret = 1;
    }

    return ret;
}

static void path_remove_dir()
{
    uint32_t pchar = 0;
    uint32_t i = strlen(pathp);
    uint8_t ret;

    if (0 != path_cnt)
    {
        while (pathp[i] != '/')
        {
            pchar++;
            i--;
        }
        /* Do not remove root symbol */
        if (path_cnt == 1)
        {
            pchar -= 1;
        }
        if (0 ==  strcut(pathp, pchar))
        {
            path_size -= pchar;
            pathp = (char *) realloc(pathp, (path_size * sizeof(char)));

            if (NULL != pathp)
            {
                path_cnt--; 
            }
        }
    }
}

static void file_browser_read_dir(DIR* dirp)
{
    struct dirent *entp;
    uint32_t cnt = 0;

    if (NULL != dirp)
    {
        ui_set_path(pathp);
        ui_dir_clear();
        dir_offset = 0;
        dir_pos = 0;
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
            if (0 == dir_offset)
            {
                dir_offset = telldir(dirp);
            }
            cnt++;
        }
        if (0 == cnt)
        {
            ui_empty_dir_add_entity();
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

void file_browser_run(DIR *rootp)
{ 
    struct dirent *entp;

    switch (ui_event)
    {
        case UI_EVENT_NONE:

            break;
        case UI_EVENT_DOWN:

            dir_pos += dir_offset;

            if (0 == path_cnt)
            {
                /* use root pointer */
                entp = readdir(rootp);
            }
            else
            {
                entp = readdir(&gdir);  
            }
            if (entp != NULL)
            {
                rt_kprintf("[file_browser_run] UI_EVENT_DOWN dir_pos = %u\n", dir_pos);
            }
            ui_event = UI_EVENT_NONE;

            break;
        case UI_EVENT_UP:

            dir_pos -= dir_offset;

            if (0 == path_cnt)
            {
                /* use root pointer */
                seekdir(rootp, dir_pos);
            }
            else
            {
                seekdir(&gdir, dir_pos); 
            }
            if (entp != NULL)
            {
                rt_kprintf("[file_browser_run] UI_EVENT_UP dir_pos = %u\n", dir_pos);
            }
            
            ui_event = UI_EVENT_NONE;

            break;
        case UI_EVENT_OPEN:

            if (0 == path_cnt)
            {
                /* use root pointer */
                entp = readdir(rootp);
            }
            else
            {
                entp = readdir(&gdir);
            }
            if (NULL != entp)
            {
                rt_kprintf("[file_browser_run] UI_EVENT_OPEN %s\n", entp->d_name);
                if (FT_DIRECTORY == entp->d_type)
                {
                    path_add_dir(entp->d_name);

                    DIR *p = opendir(pathp);
                    
                    if (NULL != p)
                    {
                        gdir = *p;
                        file_browser_read_dir(&gdir);
                    }
                }
                else
                {
                    /* TODO */ 
                }
            }

            ui_event = UI_EVENT_NONE;

            break;
        
        case UI_EVENT_CLOSE:

            if (0 != path_cnt)
            {
                path_remove_dir();

                rt_kprintf("[file_browser_run] UI_EVENT_CLOSE path: %s\n", pathp);
                rt_kprintf("[file_browser_run] UI_EVENT_CLOSE path_cnt: %u\n", path_cnt);

                if (0 == path_cnt)
                {
                    file_browser_read_dir(rootp);
                }
                else
                {
                    DIR *p = opendir(pathp);
                        
                    if (NULL != p)
                    {
                        gdir = *p;
                        file_browser_read_dir(&gdir);
                    }
                }
            }
            
            ui_event = UI_EVENT_NONE;

            break;

        default:
            break;
    }
}