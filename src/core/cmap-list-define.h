#ifndef __CMAP_LIST_DEFINE_H__
#define __CMAP_LIST_DEFINE_H__

#include "cmap-define.h"

#define CMAP_LIST_ADD(list, i, val) \
  CMAP_CALL_ARGS(list, add, i, (CMAP_MAP *)(val))
#define CMAP_LIST_RM(list, i) CMAP_CALL_ARGS(list, rm, i)

#define CMAP_LIST_PUSH(list, val) \
  CMAP_CALL_ARGS(list, push, (CMAP_MAP *)(val))
#define CMAP_LIST_POP(list) CMAP_CALL(list, pop)

#define CMAP_LIST_SHIFT(list, val) \
  CMAP_CALL_ARGS(list, shift, (CMAP_MAP *)(val))
#define CMAP_LIST_UNSHIFT(list) CMAP_CALL(list, unshift)

#endif
