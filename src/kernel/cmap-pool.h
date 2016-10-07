#ifndef __CMAP_POOL_H__
#define __CMAP_POOL_H__

#include "cmap-list.h"

#define CMAP_POOL_DECL(name, lower_name, _struct, name_s) \
  typedef struct CMAP_POOL_##name##_s CMAP_POOL_##name; \
   \
  struct CMAP_POOL_##name##_s \
  { \
    void * internal_; \
   \
    void (*delete)(CMAP_POOL_##name * this); \
   \
    _struct * (*take)(CMAP_POOL_##name * this); \
    void (*release)(CMAP_POOL_##name * this, _struct * name_s); \
  }; \
   \
  CMAP_POOL_##name * cmap_pool_##lower_name##_create(int size);

CMAP_POOL_DECL(LIST, list, CMAP_LIST, list)

#endif
