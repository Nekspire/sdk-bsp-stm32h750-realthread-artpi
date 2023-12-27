#include <rtthread.h>

#include <lvgl.h>

#include <event.h>
#include <nes_controller.h>

struct rt_i2c_bus_device *i2c_bus1;

enum
{
   INP_STATE_BREAK,
   INP_STATE_MAKE
};

void input_init()
{
   i2c_bus1 = (struct rt_i2c_bus_device *)rt_device_find("i2c4");
}

void input_joypad_get_event(void)
{
   uint8_t button;
   uint8_t b;
   event_t func_event;

   const int ev[] = 
   {
      event_joypad1_up, event_joypad1_left,
      event_joypad1_a, event_joypad1_b,
      event_joypad1_start, event_joypad1_select,
      event_joypad1_down, event_joypad1_right
   };

   button = nes_controller_get_buttons(i2c_bus1);

   if (button != NES_CONTROLLER_BUTTON_NONE)
   {
      for (size_t i = 0; i < NES_CONTROLLER_BUTTON_COUNT; i++)
      {
         b = button & (1u << i);

         if (b != 0)
         {
            func_event = event_get(ev[i]);
            if (func_event) func_event(INP_STATE_MAKE);
         }
         else
         {
            func_event = event_get(ev[i]);
            if (func_event) func_event(INP_STATE_BREAK);
         }
      }
   }
   else
   {
      for (size_t i = 0; i < NES_CONTROLLER_BUTTON_COUNT; i++)
      {
         func_event = event_get(ev[i]);
         if (func_event) func_event(INP_STATE_BREAK);
      }
   }
}