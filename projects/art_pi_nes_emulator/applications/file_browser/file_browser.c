#include "file_browser.h"
#include "ui/ui.h"

char *pathp = NULL;
uint32_t path_cnt = 0;
uint32_t path_size = 0;
uint32_t dir_pos = 0;
uint32_t dir_pos_max = 0;
uint32_t dir_offset = 0;
DIR dir;
uint8_t file_open_lock = 0;

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

        while ((entp = readdir(dirp)) != 0)
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
        rewinddir(dirp);
    }
}

static void path_add_file(const char* txt)
{
    path_add_dir(txt);
}

static void path_remove_file()
{
    path_remove_dir();
}

static void file_read(char *file_name, void (*fopen_cb)(), void *buff, uint32_t len)
{
    int fd, size;

    path_add_file(file_name);
    ui_set_path(pathp);
    fd = open(pathp, O_RDONLY);
    rt_kprintf("[file_read] path: %s\n", pathp);
    path_remove_file();

    if (fd >= 0)
    {
        size = read(fd, buff, len);
        close(fd);

        if (size < 0)
        {
            ui_set_path(pathp);
            rt_kprintf("[file_read] error: file read failed\n");
        }
        else
        {
            file_open_lock = 1;
            rt_kprintf("[file_read] name: %s\n", file_name);
            rt_kprintf("[file_read] size = %d\n", size);
            
            if (fopen_cb != NULL)
            {
                fopen_cb();
            }
            else
            {
                rt_kprintf("[file_read] error: file_browser_open_file_handle_cb NULL\n");
            }
        }
    }
    else
    {
        ui_set_path(pathp);
        rt_kprintf("[file_read] error: file open failed\n");
    }
}

void file_browser_dir_next(DIR *rootp)
{
    struct dirent *entp;

    if (file_open_lock == 0)
    {
        ui_dir_focus_next();

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
            entp = readdir(&dir);  
        }
        if (entp != NULL)
        {
            rt_kprintf("[file_browser_dir_next] dir_pos = %u\n", dir_pos);
        }
    }
}

void file_browser_dir_prev(DIR *rootp)
{
    struct dirent *entp;

    if (file_open_lock == 0)
    {
        ui_dir_focus_prev();

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
            seekdir(&dir, dir_pos); 
        }
        if (entp != NULL)
        {
            rt_kprintf("[file_browser_dir_prev] dir_pos = %u\n", dir_pos);
        }
    }
}

void file_browser_open(DIR *rootp, void (*fopen_cb)(), void *buff, uint32_t len)
{
    struct dirent *entp;

    if (file_open_lock == 0)
    {
        if (0 == path_cnt)
        {
            /* use root pointer */
            entp = readdir(rootp);
        }
        else
        {
            entp = readdir(&dir);
        }
        if (NULL != entp)
        {
            if (FT_DIRECTORY == entp->d_type)
            {
                path_add_dir(entp->d_name);

                rt_kprintf("[file_browser_dir_open] %s\n", entp->d_name);
                rt_kprintf("[file_browser_dir_open] path: %s\n", pathp);
                rt_kprintf("[file_browser_dir_open] path_cnt: %u\n", path_cnt);

                DIR *p = opendir(pathp);
                
                if (NULL != p)
                {
                    dir = *p;
                    dir_read(&dir);  
                }
            }
            else if ((FT_REGULAR == entp->d_type) || (FT_SOCKET == entp->d_type) || (FT_USER == entp->d_type))
            {
                file_read(entp->d_name, fopen_cb, buff, len);
            }
            
        }
    }
}

void file_browser_dir_close(DIR *rootp)
{
    if (0 != path_cnt)
    {
        if (file_open_lock == 1)
        {
            file_open_lock = 0;
            dir_read(&dir);
        }
        else
        {
            path_remove_dir();

            rt_kprintf("[file_browser_dir_close] path: %s\n", pathp);
            rt_kprintf("[file_browser_dir_close] path_cnt: %u\n", path_cnt);

            if (0 == path_cnt)
            {
                dir_read(rootp);
            }
            else
            {
                DIR *p = opendir(pathp);
                    
                if (NULL != p)
                {
                    dir = *p;
                    dir_read(&dir);
                }
            }
        }
    }
}

DIR * file_browser_init(lv_indev_t *indevp, lv_indev_type_t type, lv_event_cb_t *eventp)
{ 
    static DIR *rootp;

    /* Initialize file browser ui */
    ui_init(indevp, eventp, type);

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