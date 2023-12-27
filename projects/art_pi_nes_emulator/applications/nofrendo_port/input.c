#include <rtthread.h>

#include <lvgl.h>

#include <event.h>
#include <nes_controller.h>

volatile int event_joypad = event_none;

struct rt_i2c_bus_device *i2c_bus1;

void input_init()
{
   i2c_bus1 = (struct rt_i2c_bus_device *)rt_device_find("i2c4");
}

int input_joypad_get_event(void)
{
   nes_controller_button_t key = nes_match_button(i2c_bus1);

   switch (key)
   {
   case NES_CONTROLLER_BUTTON_A:
      event_joypad = event_joypad1_a;
      break;
   case NES_CONTROLLER_BUTTON_B:
      event_joypad = event_joypad1_b;
      break;
   case NES_CONTROLLER_BUTTON_DOWN:
      event_joypad = event_joypad1_down;
      break;
   case NES_CONTROLLER_BUTTON_UP:
      event_joypad = event_joypad1_up;
      break;
   case NES_CONTROLLER_BUTTON_LEFT:
      event_joypad = event_joypad1_left;
      break;
   case NES_CONTROLLER_BUTTON_RIGHT:
      event_joypad = event_joypad1_right;
      break;
   case NES_CONTROLLER_BUTTON_SELECT:
      event_joypad = event_joypad1_select;
      break;
   case NES_CONTROLLER_BUTTON_START:
      event_joypad = event_joypad1_start;
      break;
   default:
      event_joypad = event_none;
      break;
   }

   return event_joypad;
}