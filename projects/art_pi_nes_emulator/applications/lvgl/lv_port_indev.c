#include <lvgl.h>
#include <rtdevice.h>
#include <lv_port_indev.h>
#include "nes_controller.h"

struct rt_i2c_bus_device *i2c_bus;

static void nes_controller_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    nes_controller_button_t button;

    button = nes_controller_get_button(i2c_bus);

    switch (button)
    {
    case NES_CONTROLLER_BUTTON_A:
        data->key = LV_KEY_ENTER;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_B:
        data->key = LV_KEY_ESC;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_DOWN:
        data->key = LV_KEY_DOWN;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_UP:
        data->key = LV_KEY_UP;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_LEFT:
        data->key = LV_KEY_LEFT;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_RIGHT:
        data->key = LV_KEY_RIGHT;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_SELECT:
        data->key = LV_KEY_BACKSPACE;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    case NES_CONTROLLER_BUTTON_START:
        data->key = LV_KEY_HOME;
        data->state = LV_INDEV_STATE_PRESSED;
        break;
    default:
        data->state = LV_INDEV_STATE_RELEASED;
        break;
    }
}

rt_err_t rt_hw_nes_controller_register(void)
{
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c4");

    nes_controllet_status_t stat = nes_controller_init(i2c_bus);

    if (stat != NES_CONTROLLER_OK)
    {
        rt_kprintf("Failed to initialize nes controller");
        return -RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}

lv_port_indev_t lv_port_indev_init(void)
{
    static lv_port_indev_t lv_port;

    lv_indev_drv_init(&lv_port.indev_drv);

    lv_port.indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    lv_port.indev_drv.read_cb = nes_controller_read_cb;

    lv_port.indevp = lv_indev_drv_register(&lv_port.indev_drv);
    rt_hw_nes_controller_register();

    return lv_port;
}
