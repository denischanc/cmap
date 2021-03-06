#ifndef __CMAP_COMMON_LIST_H__
#define __CMAP_COMMON_LIST_H__

#include "cmap-list.h"

#define CMAP_ADD(list, i, val) \
  CMAP_CALL_ARGS(list, add, i, (CMAP_MAP *)val)
#define CMAP_RM(list, i) \
  CMAP_CALL_ARGS(list, rm, i)

#define CMAP_PUSH(list, val) \
  CMAP_CALL_ARGS(list, push, (CMAP_MAP *)val)
#define CMAP_POP(list) \
  CMAP_CALL(list, pop)

#define CMAP_SHIFT(list, val) \
  CMAP_CALL_ARGS(list, shift, (CMAP_MAP *)val)
#define CMAP_UNSHIFT(list) \
  CMAP_CALL(list, unshift)

#endif
