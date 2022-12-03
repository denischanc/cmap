#ifndef __CMAP_LIST_DEFINE_H__
#define __CMAP_LIST_DEFINE_H__

#include "cmap.h"

#define CMAP_LIST(size_inc, proc_ctx, aisle) \
  cmap_list_public.create(size_inc, proc_ctx, aisle)

#define CMAP_LIST_SET(list, i, val) \
  CMAP_CALL_ARGS(list, set, i, (CMAP_MAP *)val)
#define CMAP_LIST_GET(list, i) CMAP_CALL_ARGS(list, get, i)

#define CMAP_LIST_ADD(list, i, val) \
  CMAP_CALL_ARGS(list, add, i, (CMAP_MAP *)val)
#define CMAP_LIST_RM(list, i) CMAP_CALL_ARGS(list, rm, i)

#define CMAP_LIST_PUSH(list, val) \
  CMAP_CALL_ARGS(list, push, (CMAP_MAP *)val)
#define CMAP_LIST_POP(list) CMAP_CALL(list, pop)

#define CMAP_LIST_UNSHIFT(list, val) \
  CMAP_CALL_ARGS(list, unshift, (CMAP_MAP *)val)
#define CMAP_LIST_SHIFT(list) CMAP_CALL(list, shift)

#endif
