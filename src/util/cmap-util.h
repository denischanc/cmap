#ifndef __CMAP_UTIL_H__
#define __CMAP_UTIL_H__

#include <stdarg.h>
#include <uv.h>
#include <stdint.h>
#include "cmap-list-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-map-type.h"

typedef struct
{
  CMAP_LIST * (*fill_list)(CMAP_LIST * list, ...);
  CMAP_LIST * (*vfill_list)(CMAP_LIST * list, va_list maps);

  CMAP_LIST * (*to_list)(CMAP_PROC_CTX * proc_ctx, ...);
  CMAP_LIST * (*vto_list)(CMAP_PROC_CTX * proc_ctx, va_list maps);

  CMAP_MAP * (*to_map)(CMAP_PROC_CTX * proc_ctx, ...);
  CMAP_MAP * (*vto_map)(CMAP_PROC_CTX * proc_ctx, va_list key_maps);

  void (*uv_error)(int err);
  void (*uv_dummy)(uv_work_t * req);

  CMAP_MAP * (*copy)(CMAP_MAP * dst, CMAP_MAP * src);

  CMAP_LIST * (*dup_string)(CMAP_LIST * dst, CMAP_LIST * src,
    CMAP_PROC_CTX * proc_ctx);

  char * (*strdup)(const char * src);

  int (*is_val)(CMAP_LIST * list, CMAP_MAP * val);

  int64_t (*time_us)();
} CMAP_UTIL_PUBLIC;

extern const CMAP_UTIL_PUBLIC cmap_util_public;

#endif
