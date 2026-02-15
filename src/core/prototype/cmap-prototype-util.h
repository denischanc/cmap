#ifndef __CMAP_PROTOTYPE_UTIL__
#define __CMAP_PROTOTYPE_UTIL__

#include "cmap-prototype-util-define.h"
#include "cmap-map-type.h"
#include "cmap-fn-type.h"
#include "cmap-proc-ctx-type.h"

typedef struct
{
  CMAP_MAP * map;
  CMAP_FN * fn;
} CMAP_PROTOTYPE_UTIL_MAP_FN;

char cmap_prototype_util_args_to_map_fn(CMAP_LIST * args,
  CMAP_PROTOTYPE_UTIL_MAP_FN * map_fn);
void cmap_prototype_util_require_map(CMAP_MAP ** proto,
  CMAP_PROC_CTX * proc_ctx);

#endif
