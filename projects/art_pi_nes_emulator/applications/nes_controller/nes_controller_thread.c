#include <rtthread.h>
#include "nes_controller.h"

#ifndef LV_THREAD_STACK_SIZE
    #define LV_THREAD_STACK_SIZE 4096
#endif

#ifndef LV_THREAD_PRIO
    #define LV_THREAD_PRIO 21
#endif

static struct rt_thread nes_controller_thread;
static rt_uint8_t nes_controller_thread_stack[LV_THREAD_STACK_SIZE];

struct rt_i2c_bus_device *i2c_bus;      /* I2C bus device handle */

static void nes_controller_entry(void *parameter)
{
    nes_controller_button_t button;
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c4");

    nes_controllet_status_t stat = nes_controller_init(i2c_bus);

    while(1)
    {
        if (NES_CONTROLLER_OK == stat)
        {
            rt_thread_mdelay(100);
            button = nes_match_button(i2c_bus);

            switch (button)
            {
            case NES_CONTROLLER_BUTTON_A:
                rt_kprintf("A\n");
                break;
            case NES_CONTROLLER_BUTTON_B:
                rt_kprintf("B\n");
                break;
            case NES_CONTROLLER_BUTTON_DOWN:
                rt_kprintf("DOWN\n");
                break;
            case NES_CONTROLLER_BUTTON_UP:
                rt_kprintf("UP\n");
                break;
            case NES_CONTROLLER_BUTTON_LEFT:
                rt_kprintf("LEFT\n");
                break;
            case NES_CONTROLLER_BUTTON_RIGHT:
                rt_kprintf("RIGHT\n");
                break;
            case NES_CONTROLLER_BUTTON_SELECT:
                rt_kprintf("SELECT\n");
                break;
            case NES_CONTROLLER_BUTTON_START:
                rt_kprintf("START\n");
                break;
            default:
                break;
            }
        }
    }
}

static int init(void)
{
    rt_thread_init(&nes_controller_thread,
                   "NES_CONTROLLER",
                   nes_controller_entry,
                   RT_NULL,
                   &nes_controller_thread_stack[0],
                   sizeof(nes_controller_thread_stack),
                   LV_THREAD_PRIO,
                   10);
    rt_thread_startup(&nes_controller_thread);

    return 0;
}
INIT_ENV_EXPORT(init);