#include "file_browser.h"
#include "ui.h"
#include "rtthread.h"

char *pathp = NULL;
uint32_t path_cnt = 0;
uint32_t path_size = 0;
uint32_t dir_pos = 0;
uint32_t dir_pos_max = 0;
uint32_t dir_offset = 0;

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

static uint8_t strcut(char *txt, uint32_t len)
{
    uint8_t ret = 0;
    uint32_t txtlen = strlen(txt);
    
    if (txtlen >= len)
    {
        for (int i = len; i >= 0; i--)
        {
            if (i == 0)
            {
                txt[txtlen] = '\0';
            }
            else
            {
                txt[txtlen] = 0;
            }
            txtlen--;
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

static void dir_read(DIR* dirp)
{
    struct dirent *entp;
    uint32_t cnt = 0;
    static char cnt_str[10];

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
        else
        {
            dir_pos_max = dir_offset * (cnt - 1);
        }
        ui_dir_add_count(__itoa(cnt, cnt_str, 10));
        rt_kprintf("[dir_read] dir_pos_max = %u\n", dir_pos_max);
        rewinddir(dirp);
    }
}

static void dir_scroll_down(DIR *rootp, DIR *dirp)
{
    struct dirent *entp;

    if (dir_pos == dir_pos_max)
    {
        dir_pos = 0;
    }
    else
    {
        dir_pos += dir_offset;
    }
    if (0 == path_cnt)
    {
        /* use root pointer */
        entp = readdir(rootp);
    }
    else
    {
        entp = readdir(dirp);  
    }
    if (entp != NULL)
    {
        rt_kprintf("[dir_scroll_down] dir_pos = %u\n", dir_pos);
    }
}

static void dir_scroll_up(DIR *rootp, DIR *dirp)
{
    struct dirent *entp;

    if (dir_pos == 0)
    {
        dir_pos = dir_pos_max;
    }
    else
    {
        dir_pos -= dir_offset;
    }
    if (0 == path_cnt)
    {
        /* use root pointer */
        seekdir(rootp, dir_pos);
    }
    else
    {
        seekdir(dirp, dir_pos); 
    }
    if (entp != NULL)
    {
        rt_kprintf("[dir_scroll_up] dir_pos = %u\n", dir_pos);
    }
}

static void dir_open(DIR *rootp, DIR *dirp)
{
    struct dirent *entp;

    if (0 == path_cnt)
    {
        /* use root pointer */
        entp = readdir(rootp);
    }
    else
    {
        entp = readdir(dirp);
    }
    if (NULL != entp)
    {
        if (FT_DIRECTORY == entp->d_type)
        {
            path_add_dir(entp->d_name);

            rt_kprintf("[dir_open] %s\n", entp->d_name);
            rt_kprintf("[dir_open] path: %s\n", pathp);
            rt_kprintf("[dir_open] path_cnt: %u\n", path_cnt);

            DIR *p = opendir(pathp);
            
            if (NULL != p)
            {
                *dirp = *p;
                dir_read(dirp);  
            }
        }
        else
        {
            /* TODO */ 
        }
    }
}

static void dir_close(DIR *rootp, DIR *dirp)
{
    if (0 != path_cnt)
    {
        path_remove_dir();

        rt_kprintf("[dir_close] path: %s\n", pathp);
        rt_kprintf("[dir_close] path_cnt: %u\n", path_cnt);

        if (0 == path_cnt)
        {
            dir_read(rootp);
        }
        else
        {
            DIR *p = opendir(pathp);
                
            if (NULL != p)
            {
                *dirp = *p;
                dir_read(dirp);
            }
        }
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
            dir_read(rootp);
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
    static DIR dir;

    switch (ui_event)
    {
        case UI_EVENT_NONE:

            break;

        case UI_EVENT_DOWN:

            dir_scroll_down(rootp, &dir);
            ui_event = UI_EVENT_NONE;
            break;
    
        case UI_EVENT_UP:

            dir_scroll_up(rootp, &dir);
            ui_event = UI_EVENT_NONE;
            break;

        case UI_EVENT_OPEN:

            dir_open(rootp, &dir);
            ui_event = UI_EVENT_NONE;
            break;
        
        case UI_EVENT_CLOSE:
            
            dir_close(rootp, &dir);
            ui_event = UI_EVENT_NONE;
            break;

        default:

            break;
    }
}