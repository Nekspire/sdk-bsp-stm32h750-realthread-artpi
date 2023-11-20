#ifndef NES_CONTROLLER_H
#define NES_CONTROLLER_H

#include "drivers/i2c.h"

typedef enum 
{
    NES_CONTROLLER_BUTTON_NULL,
    NES_CONTROLLER_BUTTON_LEFT,
    NES_CONTROLLER_BUTTON_RIGHT,
    NES_CONTROLLER_BUTTON_UP,
    NES_CONTROLLER_BUTTON_DOWN,
    NES_CONTROLLER_BUTTON_SELECT,
    NES_CONTROLLER_BUTTON_START,
    NES_CONTROLLER_BUTTON_B,
    NES_CONTROLLER_BUTTON_A,
} nes_controller_button_t;

typedef enum 
{
    NES_CONTROLLER_OK,
    NES_CONTROLLER_ERR,
} nes_controllet_status_t;

nes_controllet_status_t nes_controller_init(struct rt_i2c_bus_device *bus);
nes_controller_button_t nes_match_button(struct rt_i2c_bus_device *bus);

#endif /* NES_CONTROLLER_H */
