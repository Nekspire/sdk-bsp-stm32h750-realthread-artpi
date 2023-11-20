#include "nes_controller.h"
#include "drv_common.h"

#define DBG_TAG               "NES_CONTROLLER"
#define DBG_LVL               DBG_INFO
#include <rtdbg.h>

#define NES_I2C_ADDRESS         (0x52 << 1)

NES_Controller_Status nes_controller_init(struct rt_i2c_bus_device *bus, char *bus_name)
{
    const rt_uint8_t DATA[] = {0xF0, 0x55};
    const rt_uint8_t ID_ADDR = 0xFA;
    const uint8_t NES_CONTROLLER_ID[] = {0x01, 0x00, 0xA4, 0x20, 0x01, 0x01};

    rt_uint8_t id[6] = {0,0,0,0,0,0};

    bus = (struct rt_i2c_bus_device *)rt_device_find(bus_name);

    if (RT_NULL != bus)
    {
        rt_i2c_master_send(bus, NES_I2C_ADDRESS, RT_I2C_WR, DATA, 2);
        rt_i2c_master_send(bus, NES_I2C_ADDRESS, RT_I2C_WR, &ID_ADDR, 1);
        rt_i2c_master_recv(bus, NES_I2C_ADDRESS, RT_I2C_RD, id, 6);
        rt_thread_mdelay(10);
    
        LOG_I("%u %u %u %u %u %u", id[0], id[1], id[2], id[3], id[4], id[5]);

        if (memcmp(id, NES_CONTROLLER_ID, sizeof(NES_CONTROLLER_ID)) == 0)
        {
            return NES_CONTROLLER_OK;
        }
        else
        {
            return NES_CONTROLLER_ERR;
        }
    }
    else
    {
        return NES_CONTROLLER_ERR;
    }
}

// uint16_t nes_controller_read_code(I2C_HandleTypeDef *i2c) {
//     uint8_t zero_addr = 0x00;
//     uint8_t controller_bytes[6];
//     uint16_t state = 0;

//     HAL_I2C_Master_Transmit(i2c, NES_I2C_ADDRESS_WRITE, &zero_addr, 1, 100);

//     HAL_I2C_Master_Receive(i2c, NES_I2C_ADDRESS_READ, controller_bytes, 6, 100);

//     controller_bytes[4] ^= 0xFF;
//     controller_bytes[5] ^= 0xFF;

//     state = (controller_bytes[4] << 8) | controller_bytes[5];

//     return state;
// }

// bool nes_match_button(uint16_t code, NES_Controller_Buttons *button) {
//     bool state = false;

//     if (code & NES_BUTTON_LEFT) {
//         *button = LEFT;
//         state = true;
//     } else if (code & NES_BUTTON_RIGHT) {
//         *button = RIGHT;
//         state = true;
//     } else if (code & NES_BUTTON_UP) {
//         *button = UP;
//         state = true;
//     } else if (code & NES_BUTTON_DOWN) {
//         *button = DOWN;
//         state = true;
//     } else if (code & NES_BUTTON_SELECT) {
//         *button = SELECT;
//         state = true;
//     } else if (code & NES_BUTTON_START) {
//         *button = START;
//         state = true;
//     } else if (code & NES_BUTTON_B) {
//         *button = B;
//         state = true;
//     } else if (code & NES_BUTTON_A) {
//         *button = A;
//         state = true;
//     }

//     return state;
// }