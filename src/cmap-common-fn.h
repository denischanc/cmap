#ifndef __CMAP_COMMON_FN_H__
#define __CMAP_COMMON_FN_H__

#include "cmap-fn.h"

#define CMAP_DO_PROCESS(fn, map, args) \
  CMAP_CALL_ARGS(fn, process, map, args)

#endif
