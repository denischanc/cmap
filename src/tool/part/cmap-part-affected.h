#ifndef __CMAP_PART_AFFECTED_H__
#define __CMAP_PART_AFFECTED_H__

#include "cmap-part-ctx.h"

typedef struct
{
  void (*add)(const char * map, const char * name, CMAP_PART_CTX * ctx);

  char (*contains_n_add)(const char * map, const char * name);
} CMAP_PART_AFFECTED_PUBLIC;

extern const CMAP_PART_AFFECTED_PUBLIC cmap_part_affected_public;

#endif
