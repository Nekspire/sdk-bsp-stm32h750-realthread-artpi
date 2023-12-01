#ifndef _UI_H
#define _UI_H

#include "lv_hal_indev.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum
{
   UI_LIST_ENTITY_FILE,
   UI_LIST_ENTITY_DIR
} ui_list_entity_t;

typedef enum
{
   UI_EVENT_NONE,
   UI_EVENT_DOWN,
   UI_EVENT_UP,
   UI_EVENT_OPEN,
   UI_EVENT_CLOSE
} ui_event_t;


uint8_t ui_init(lv_indev_t *indevp, lv_indev_type_t type);
void ui_set_path(const char *path);
void ui_dir_add_entity(ui_list_entity_t type, const char *name);
void ui_dir_clear();
void ui_empty_dir_add_entity(void);
void ui_dir_add_count(const char *txt);

extern ui_event_t ui_event;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UI_H */
