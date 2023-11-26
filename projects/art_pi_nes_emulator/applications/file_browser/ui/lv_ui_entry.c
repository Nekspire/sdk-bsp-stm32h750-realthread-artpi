/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author        Notes
 * 2022-05-13     Meco Man      First version
 */

#ifdef __RTTHREAD__

void lv_user_gui_init(void)
{
    extern void lv_file_browser_init(void);
    lv_file_browser_init();
}

#endif /* __RTTHREAD__ */
