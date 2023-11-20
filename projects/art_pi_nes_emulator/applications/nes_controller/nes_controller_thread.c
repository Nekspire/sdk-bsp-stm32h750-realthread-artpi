#include <rtthread.h>
#include "nes_controller.h"

#ifndef LV_THREAD_STACK_SIZE
    #define LV_THREAD_STACK_SIZE 4096
#endif

#ifndef LV_THREAD_PRIO
    #define LV_THREAD_PRIO (RT_THREAD_PRIORITY_MAX * 2 / 3)
#endif

static struct rt_thread nes_controller_thread;
static rt_uint8_t nes_controller_thread_stack[LV_THREAD_STACK_SIZE];

#define NES_CONTROLLER_I2C_BUS_NAME "i2c3"  /* Sensor connected I2C bus device name */
struct rt_i2c_bus_device *i2c_bus;      /* I2C bus device handle */

static void nes_controller_entry(void *parameter)
{
    NES_Controller_Status stat = nes_controller_init(i2c_bus, "i2c3");

    while(1)
    {

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