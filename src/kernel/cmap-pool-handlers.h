#ifndef __CMAP_POOL_HANDLERS_H__
#define __CMAP_POOL_HANDLERS_H__

#include "cmap-list.h"
#include "cmap-string.h"

#define CMAP_POOL_HANDLER_DECL(name, lower_name, _struct, name_s) \
  typedef struct \
  { \
    _struct * (*create)(); \
    void (*delete)(_struct * name_s); \
    void (*clean)(_struct * name_s); \
  } CMAP_POOL_##name##_HANDLER; \
    \
  extern CMAP_POOL_##name##_HANDLER * cmap_pool_##lower_name##_handler;

CMAP_POOL_HANDLER_DECL(LIST, list, CMAP_LIST, list)
CMAP_POOL_HANDLER_DECL(STRING, string, CMAP_STRING, string)

#endif
