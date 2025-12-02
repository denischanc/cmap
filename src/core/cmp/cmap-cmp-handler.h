#ifndef __CMAP_CMP_HANDLER_H__
#define __CMAP_CMP_HANDLER_H__

#include "cmap-cmp-handler-define.h"
#include <stdint.h>
#include "cmap-map-type.h"

typedef struct
{
  char (*cmp)(int64_t * ret, CMAP_MAP * map_l, CMAP_MAP * map_r);
} CMAP_CMP_HANDLER;

CMAP_CMP_HANDLER_LOOP(CMAP_CMP_HANDLER_DECL)

#endif
