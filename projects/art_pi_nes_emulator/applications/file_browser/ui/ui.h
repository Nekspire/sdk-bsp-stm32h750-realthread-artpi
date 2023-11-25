#ifndef _UI_H
#define _UI_H

#ifdef __cplusplus
extern "C" {
#endif

void ui_init(void);

typedef enum
{
   UI_LIST_ENTITY_FILE,
   UI_LIST_ENTITY_DIR
} ui_list_entity_t;

void ui_set_path(const char *path);
void ui_dir_add_entity(ui_list_entity_t type, const char *name);
void ui_dir_clear();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UI_H */
