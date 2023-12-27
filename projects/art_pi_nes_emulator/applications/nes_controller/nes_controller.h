#ifndef NES_CONTROLLER_H
#define NES_CONTROLLER_H

#include "drivers/i2c.h"
#include "stdint.h"

#define NES_CONTROLLER_BUTTON_COUNT     8U

typedef enum 
{
    NES_CONTROLLER_BUTTON_NONE = 0x0000,
    NES_CONTROLLER_BUTTON_UP = 0x0001,
    NES_CONTROLLER_BUTTON_LEFT = 0x0002,
    NES_CONTROLLER_BUTTON_A = 0x0010,
    NES_CONTROLLER_BUTTON_B = 0x0040,
    NES_CONTROLLER_BUTTON_START = 0x0400,
    NES_CONTROLLER_BUTTON_SELECT = 0x1000,
    NES_CONTROLLER_BUTTON_DOWN = 0x4000,
    NES_CONTROLLER_BUTTON_RIGHT = 0x8000,
} nes_controller_button_t;

typedef enum 
{
    NES_CONTROLLER_OK,
    NES_CONTROLLER_ERR,
} nes_controllet_status_t;

nes_controllet_status_t nes_controller_init(struct rt_i2c_bus_device *bus);
nes_controller_button_t nes_match_button(struct rt_i2c_bus_device *bus);
uint8_t nes_controller_get_buttons(struct rt_i2c_bus_device *bus);

#endif /* NES_CONTROLLER_H */
