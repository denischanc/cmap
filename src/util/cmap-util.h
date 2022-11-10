#ifndef __CMAP_UTIL_H__
#define __CMAP_UTIL_H__

#include "cmap-list.h"
#include <stdarg.h>

typedef struct
{
  void (*delete_list_vals)(CMAP_LIST * list);

  void (*release_pool_list_n_strings)(CMAP_LIST * list);

  CMAP_LIST * (*split_w_aisle)(const char * line, char sep,
    const char * aisle);
  CMAP_LIST * (*split_w_pool)(const char * line, char sep);

  CMAP_LIST * (*fill_list)(CMAP_LIST * list, ...);
  CMAP_LIST * (*vfill_list)(CMAP_LIST * list, va_list maps);

  CMAP_LIST * (*to_list)(const char * aisle, ...);
  CMAP_LIST * (*vto_list)(const char * aisle, va_list maps);

  CMAP_MAP * (*to_map)(const char * aisle, ...);
  CMAP_MAP * (*vto_map)(const char * aisle, va_list key_maps);

  void (*uv_error)(int err);
} CMAP_UTIL_PUBLIC;

extern const CMAP_UTIL_PUBLIC cmap_util_public;

#endif
