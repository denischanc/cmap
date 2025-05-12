#ifndef __CMAP_PART_NAME2MAP_H__
#define __CMAP_PART_NAME2MAP_H__

#include "cmap-part-ctx.h"

typedef struct
{
  const char * map;
  char affected;
} CMAP_PART_NAME2MAP_RET;

typedef struct
{
  void (*put)(const char * map, const char * name, const char * map_name,
    CMAP_PART_CTX * ctx);

  CMAP_PART_NAME2MAP_RET (*get)(const char * map, const char * name,
    const char * next_name, CMAP_PART_CTX * ctx);
} CMAP_PART_NAME2MAP_PUBLIC;

extern const CMAP_PART_NAME2MAP_PUBLIC cmap_part_name2map_public;

#endif
