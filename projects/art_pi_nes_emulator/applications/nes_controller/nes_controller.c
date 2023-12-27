#include "nes_controller.h"
#include "drv_common.h"

#define DBG_TAG               "nes.controller"
#define DBG_LVL               DBG_INFO
#include <rtdbg.h>

#define NES_I2C_ADDRESS         (0x52 << 1)

nes_controllet_status_t nes_controller_init(struct rt_i2c_bus_device *bus)
{
    const uint8_t DATA[] = {0xF0, 0x55};
    const uint8_t ID_ADDR = 0xFA;
    const uint8_t NES_CONTROLLER_ID[] = {0x01, 0x00, 0xA4, 0x20, 0x01, 0x01};

    uint8_t id[6] = {0};

    if (RT_NULL != bus)
    {
        rt_i2c_master_send(bus, NES_I2C_ADDRESS, RT_I2C_WR, DATA, 2);
        rt_i2c_master_send(bus, NES_I2C_ADDRESS, RT_I2C_WR, &ID_ADDR, 1);
        rt_i2c_master_recv(bus, NES_I2C_ADDRESS, RT_I2C_RD, id, 6);
        rt_thread_mdelay(10);

        if (memcmp(id, NES_CONTROLLER_ID, sizeof(NES_CONTROLLER_ID)) == 0)
        {
            LOG_I("ID: %u %u %u %u %u %u correct, init success", id[0], id[1], id[2], id[3], id[4], id[5]);
            return NES_CONTROLLER_OK;
        }
        else
        {
            LOG_E("ID: %u %u %u %u %u %u not correct, init error", id[0], id[1], id[2], id[3], id[4], id[5]);
            return NES_CONTROLLER_ERR;
        }
    }
    else
    {
        return NES_CONTROLLER_ERR;
    }
}

nes_controller_button_t nes_controller_get_button(struct rt_i2c_bus_device *bus) 
{
    nes_controller_button_t button = NES_CONTROLLER_BUTTON_NONE;
    const uint8_t ADDR = 0x00;
    uint8_t controller_bytes[6];
    uint16_t state = 0;

    rt_i2c_master_send(bus, NES_I2C_ADDRESS, RT_I2C_WR, &ADDR, 1);
    rt_i2c_master_recv(bus, NES_I2C_ADDRESS, RT_I2C_RD, controller_bytes, 6);

    controller_bytes[4] ^= 0xFF;
    controller_bytes[5] ^= 0xFF;

    state = (controller_bytes[4] << 8) | controller_bytes[5];

    if (state != NES_CONTROLLER_BUTTON_NONE)
    {
        button = state;
    }

    return button;
}

uint8_t nes_controller_get_buttons(struct rt_i2c_bus_device *bus)
{
    const uint8_t ADDR = 0x00;
    uint8_t controller_bytes[6];
    uint16_t st = NES_CONTROLLER_BUTTON_NONE;
    uint8_t ret = 0;

    for (size_t i = 0; i < NES_CONTROLLER_BUTTON_COUNT; i++)
    {
        rt_i2c_master_send(bus, NES_I2C_ADDRESS, RT_I2C_WR, &ADDR, 1);
        rt_i2c_master_recv(bus, NES_I2C_ADDRESS, RT_I2C_RD, controller_bytes, 6);

        controller_bytes[4] ^= 0xFF;
        controller_bytes[5] ^= 0xFF;

        st = (controller_bytes[4] << 8) | controller_bytes[5];

        if (st & NES_CONTROLLER_BUTTON_UP)
            ret |= 1u << 0;
        if (st & NES_CONTROLLER_BUTTON_LEFT)
            ret |= 1u << 1;
        if (st & NES_CONTROLLER_BUTTON_A)
            ret |= 1u << 2;
        if (st & NES_CONTROLLER_BUTTON_B)
            ret |= 1u << 3;
        if (st & NES_CONTROLLER_BUTTON_START)
            ret |= 1u << 4;
        if (st & NES_CONTROLLER_BUTTON_SELECT)
            ret |= 1u << 5;
        if (st & NES_CONTROLLER_BUTTON_DOWN)
            ret |= 1u << 6;
        if (st & NES_CONTROLLER_BUTTON_RIGHT)
            ret |= 1u << 7;
    }
    
    return ret;
}