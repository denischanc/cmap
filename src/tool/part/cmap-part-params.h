#ifndef __CMAP_PART_PARAMS_H__
#define __CMAP_PART_PARAMS_H__

#include "cmap-part-ctx.h"

typedef struct
{
  void (*add)(const char * map, CMAP_PART_CTX * ctx);
} CMAP_PART_PARAMS_PUBLIC;

extern const CMAP_PART_PARAMS_PUBLIC cmap_part_params_public;

#endif
