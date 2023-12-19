/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     the first version
 * 2022-05-10     Meco Man     improve rt-thread initialization process
 */

#ifdef __RTTHREAD__

#include <lvgl.h>
#include <rtthread.h>
#include <file_browser.h>
#include <lv_port_indev.h>
#include <nofrendo/nofrendo.h>

#define DBG_TAG    "LVGL"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef PKG_LVGL_THREAD_STACK_SIZE
#define PKG_LVGL_THREAD_STACK_SIZE 4096
#endif /* PKG_LVGL_THREAD_STACK_SIZE */

#ifndef PKG_LVGL_THREAD_PRIO
#define PKG_LVGL_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif /* PKG_LVGL_THREAD_PRIO */

extern void lv_port_disp_init(void);
extern void lv_user_gui_init(void);

static struct rt_thread lvgl_thread;
static DIR *rootp;
static file_browser_file_t fb_file;
static uint8_t fb_file_buff[1024];

#ifdef rt_align
rt_align(RT_ALIGN_SIZE)
#else
ALIGN(RT_ALIGN_SIZE)
#endif
static rt_uint8_t lvgl_thread_stack[PKG_LVGL_THREAD_STACK_SIZE];

#if LV_USE_LOG
static void lv_rt_log(const char *buf)
{
    LOG_I(buf);
}
#endif /* LV_USE_LOG */

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
            rt_kprintf("[f_open_cb] nofrendo started\n");
            nofrendo_main(NULL);
            rt_kprintf("[f_open_cb] nofrendo stopped\n");
        }
    }
}

static void event_key_handler_cb(lv_event_cb_t *e)
{
    lv_event_code_t event_code = lv_event_get_code((lv_event_t *) e);
    lv_obj_t *event_button = lv_event_get_current_target((lv_event_t *) e);

    if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *) e) == LV_KEY_DOWN)
    {
        file_browser_dir_next(rootp);
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *) e) == LV_KEY_UP)
    {
        file_browser_dir_prev(rootp);
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *) e) == LV_KEY_ENTER)
    {
        file_browser_open(rootp, &fb_file);
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key((lv_event_t *) e) == LV_KEY_ESC)
    {
        file_browser_dir_close(rootp);
    }
}

static void lvgl_thread_entry(void *parameter)
{

#if LV_USE_LOG
    lv_log_register_print_cb(lv_rt_log);
#endif /* LV_USE_LOG */
    lv_init();
    lv_port_disp_init();

    fb_file.fopen_cb = f_open_cb;
    fb_file.fopen_read_buffer = fb_file_buff;
    fb_file.fopen_read_buff_len = sizeof(fb_file_buff);

    lv_port_indev_t lv_port_indev = lv_port_indev_init();
    rootp = file_browser_init(lv_port_indev.indevp, lv_port_indev.indev_drv.type, (lv_event_cb_t *) event_key_handler_cb);

    /* handle the tasks of LVGL */
    while(1)
    {
        lv_task_handler();
        rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
    }
}

static int lvgl_thread_init(void)
{
    rt_err_t err;

    err = rt_thread_init(&lvgl_thread, "LVGL", lvgl_thread_entry, RT_NULL,
           &lvgl_thread_stack[0], sizeof(lvgl_thread_stack), PKG_LVGL_THREAD_PRIO, 0);
    if(err != RT_EOK)
    {
        LOG_E("Failed to create LVGL thread");
        return -1;
    }
    rt_thread_startup(&lvgl_thread);

    return 0;
}
INIT_ENV_EXPORT(lvgl_thread_init);

#endif /*__RTTHREAD__*/
