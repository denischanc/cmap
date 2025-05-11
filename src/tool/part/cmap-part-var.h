#ifndef __CMAP_PART_VAR_H__
#define __CMAP_PART_VAR_H__

#include "cmap-part-ctx.h"

typedef struct
{
  const char * map;
  char dirty, is_def;
} CMAP_PART_VAR_RET;

typedef struct
{
  void (*put)(const char * name, const char * map, CMAP_PART_CTX * ctx);
  void (*put_loc)(const char * name, const char * map, CMAP_PART_CTX * ctx);
  char (*put_no_loc)(const char * name, const char * map, CMAP_PART_CTX * ctx);

  CMAP_PART_VAR_RET (*get)(const char * name, CMAP_PART_CTX * ctx);

  CMAP_STRINGS * (*defs)(CMAP_PART_CTX * ctx);
} CMAP_PART_VAR_PUBLIC;

extern const CMAP_PART_VAR_PUBLIC cmap_part_var_public;

#endif
