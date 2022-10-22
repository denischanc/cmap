#ifndef __CMAP_UTIL_H__
#define __CMAP_UTIL_H__

#include "cmap-list.h"

typedef struct
{
  void (*delete_list_vals)(CMAP_LIST * list);
  void (*release_pool_list_n_strings)(CMAP_LIST * list);
  CMAP_LIST * (*split_w_aisle)(const char * line, char sep,
    const char * aisle);
  CMAP_LIST * (*split_w_pool)(const char * line, char sep);
} CMAP_UTIL_PUBLIC;

extern const CMAP_UTIL_PUBLIC cmap_util_public;

#endif
