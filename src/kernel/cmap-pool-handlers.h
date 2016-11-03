#ifndef __CMAP_POOL_HANDLERS_H__
#define __CMAP_POOL_HANDLERS_H__

#include "cmap-list.h"

typedef struct
{
  CMAP_LIST * (*create)();
  void (*delete)(CMAP_LIST * list);
  void (*clean)(CMAP_LIST * list);
} CMAP_POOL_LIST_HANDLER;

extern CMAP_POOL_LIST_HANDLER * cmap_pool_list_handler;

#endif
