#ifndef _UI_H
#define _UI_H

#ifdef __cplusplus
extern "C" 
{
#endif

void ui_init(void);

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

void ui_set_path(const char *path);
void ui_dir_add_entity(ui_list_entity_t type, const char *name);
void ui_dir_clear();

extern ui_event_t ui_event;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UI_H */
