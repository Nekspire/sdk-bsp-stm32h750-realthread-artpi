#ifndef NES_CONTROLLER_H
#define NES_CONTROLLER_H

#include "drivers/i2c.h"

#define NES_BUTTON_UP           0x0001
#define NES_BUTTON_RIGHT        0x8000
#define NES_BUTTON_DOWN         0x4000
#define NES_BUTTON_LEFT         0x0002
#define NES_BUTTON_A            0x0010
#define NES_BUTTON_B            0x0040
#define NES_BUTTON_START        0x0400
#define NES_BUTTON_SELECT       0x1000

typedef enum 
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SELECT,
    START,
    B,
    A,
} NES_Controller_Buttons;

typedef enum 
{
    NES_CONTROLLER_OK,
    NES_CONTROLLER_ERR,
} NES_Controller_Status;

NES_Controller_Status nes_controller_init(struct rt_i2c_bus_device *bus, char *bus_name);
// uint16_t nes_controller_read_code(I2C_HandleTypeDef *i2c);
// bool nes_match_button(uint16_t code, NES_Controller_Buttons *button);

#endif /* NES_CONTROLLER_H */
