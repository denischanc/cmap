#ifndef __CMAP_PART_VAR_H__
#define __CMAP_PART_VAR_H__

#include "cmap-part-name2map.h"
#include "cmap-part-ctx.h"

typedef struct
{
  CMAP_PART_NAME2MAP_RET ret;
  char is_def;
} CMAP_PART_VAR_RET;

char cmap_part_var_is_loc(const char * name, CMAP_PART_CTX * ctx);
char cmap_part_var_is_proc_ctx_def(const char * name, CMAP_PART_CTX * ctx);

void cmap_part_var_put_loc(const char * name, const char * next_name);
char cmap_part_var_put_no_loc(const char * map, const char * name,
    const char * next_name);

CMAP_PART_VAR_RET cmap_part_var_get(const char * map, const char * name,
    char * next_name);

#endif
