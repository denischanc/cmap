#ifndef __CMAP_PART_VAR_H__
#define __CMAP_PART_VAR_H__

#include "cmap-part-name2map.h"
#include "cmap-part-ctx.h"

typedef struct
{
  CMAP_PART_NAME2MAP_RET ret;
  char is_def;
} CMAP_PART_VAR_RET;

typedef struct
{
  char (*proc_is_local)(const char * name, CMAP_PART_CTX * ctx);

  CMAP_PART_VAR_RET (*put_loc)(const char * name, const char * next_name);
  CMAP_PART_VAR_RET (*put_no_loc)(const char * map, const char * name,
    const char * next_name);

  CMAP_PART_VAR_RET (*get)(const char * map, const char * name,
    char * next_name);
} CMAP_PART_VAR_PUBLIC;

extern const CMAP_PART_VAR_PUBLIC cmap_part_var_public;

#endif
