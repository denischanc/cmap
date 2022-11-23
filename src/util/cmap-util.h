#ifndef __CMAP_UTIL_H__
#define __CMAP_UTIL_H__

#include <cmap/core.h>
#include <stdarg.h>
#include <uv.h>
#include "cmap-proc-ctx.h"

typedef struct
{
  void (*delete_list_vals)(CMAP_LIST * list);

  void (*release_pool_list_n_strings)(CMAP_LIST * list,
    CMAP_PROC_CTX * proc_ctx);

  CMAP_LIST * (*split_w_aisle)(const char * line, char sep,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
  CMAP_LIST * (*split_w_pool)(const char * line, char sep,
    CMAP_PROC_CTX * proc_ctx);

  CMAP_LIST * (*fill_list)(CMAP_LIST * list, ...);
  CMAP_LIST * (*vfill_list)(CMAP_LIST * list, va_list maps);

  CMAP_LIST * (*to_list)(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...);
  CMAP_LIST * (*vto_list)(CMAP_PROC_CTX * proc_ctx, const char * aisle,
    va_list maps);

  CMAP_MAP * (*to_map)(CMAP_PROC_CTX * proc_ctx, const char * aisle, ...);
  CMAP_MAP * (*vto_map)(CMAP_PROC_CTX * proc_ctx, const char * aisle,
    va_list key_maps);

  void (*uv_error)(int err);
  void (*uv_dummy)(uv_work_t * req);
} CMAP_UTIL_PUBLIC;

extern const CMAP_UTIL_PUBLIC cmap_util_public;

#endif
