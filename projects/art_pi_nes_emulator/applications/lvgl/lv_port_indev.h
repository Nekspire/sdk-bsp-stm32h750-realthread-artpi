#ifndef LV_PORT_INDEV_H
#define LV_PORT_INDEV_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <lvgl.h>

typedef struct
{
   lv_indev_drv_t indev_drv;
   lv_indev_t *indevp;
} lv_port_indev_t;

lv_port_indev_t lv_port_indev_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LV_PORT_INDEV_H */
