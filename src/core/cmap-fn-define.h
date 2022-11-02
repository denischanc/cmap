#ifndef __CMAP_FN_DEFINE_H__
#define __CMAP_FN_DEFINE_H__

#include "cmap-common.h"

#define CMAP_FN_PROCESS(fn, map, args) \
  CMAP_CALL_ARGS(fn, process, (CMAP_MAP *)map, args)

#endif
