#include <rtthread.h>

#include <lvgl.h>

#include <event.h>

int event_joypad = event_none;

static void lv_event_key_handler_cb(lv_event_cb_t *e)
{
   lv_event_code_t event_code = lv_event_get_code((lv_event_t *)e);
   lv_obj_t *event_button = lv_event_get_current_target((lv_event_t *)e);

   uint32_t key = lv_event_get_key((lv_event_t *)e);

   if (event_code == LV_EVENT_KEY)
   {
      switch (key)
      {
      case LV_KEY_ENTER:
         event_joypad = event_joypad1_a;
         break;
      case LV_KEY_ESC:
         event_joypad = event_joypad1_b;
         break;
      case LV_KEY_DOWN:
         event_joypad = event_joypad1_down;
         break;
      case LV_KEY_UP:
         event_joypad = event_joypad1_up;
         break;        
      case LV_KEY_LEFT:
         event_joypad = event_joypad1_left;
         break;        
      case LV_KEY_RIGHT:
         event_joypad = event_joypad1_right;
         break;  
      case LV_KEY_BACKSPACE:
         event_joypad = event_joypad1_select;
         break; 
      case LV_KEY_HOME:
         event_joypad = event_joypad1_start;
         break;
      default:
         break;
      }
   }
}

int input_joypad_get_event(void)
{
   return event_joypad;
}