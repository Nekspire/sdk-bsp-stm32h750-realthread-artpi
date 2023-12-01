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

static void event_key_handler_cb(lv_event_cb_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *event_button = lv_event_get_current_target(e);

    if (event_code == LV_EVENT_KEY && lv_event_get_key(e) == LV_KEY_DOWN)
    {
        file_browser_dir_next(rootp);
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key(e) == LV_KEY_UP)
    {
        file_browser_dir_prev(rootp);
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key(e) == LV_KEY_ENTER)
    {
        file_browser_dir_open(rootp);
    }

    if (event_code == LV_EVENT_KEY && lv_event_get_key(e) == LV_KEY_ESC)
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
    lv_user_gui_init();

    lv_port_indev_t lv_port_indev = lv_port_indev_init();
    rootp = file_browser_init(lv_port_indev.indevp, lv_port_indev.indev_drv.type, event_key_handler_cb);

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
