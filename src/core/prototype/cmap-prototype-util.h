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

typedef struct
{
  char (*args_to_map_fn)(CMAP_LIST * args, CMAP_PROTOTYPE_UTIL_MAP_FN * map_fn);
  CMAP_MAP * (*require_map)(CMAP_MAP ** proto, CMAP_PROC_CTX * proc_ctx);
  CMAP_MAP * (*instance)(CMAP_MAP ** proto, char * ok,
    CMAP_MAP * (*require)(CMAP_PROC_CTX *), void (*init)(CMAP_PROC_CTX *),
    CMAP_PROC_CTX * proc_ctx);
} CMAP_PROTOTYPE_UTIL_PUBLIC;

extern const CMAP_PROTOTYPE_UTIL_PUBLIC cmap_prototype_util_public;

#endif
