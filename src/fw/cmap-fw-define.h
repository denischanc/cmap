#ifndef __CMAP_FW_DEFINE_H__
#define __CMAP_FW_DEFINE_H__

#include <stdlib.h>
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-fn.h"
#include "cmap-string.h"
#include "cmap-int.h"

#define CMAP_MAP(aisle) cmap_map_create(aisle)
#define CMAP_LIST(size_inc, aisle) cmap_list_create(size_inc, aisle)
#define CMAP_FN(process, aisle) cmap_fn_create(process, aisle)
#define CMAP_STRING(val, size_inc, aisle) \
  cmap_string_create(val, size_inc, aisle)
#define CMAP_INT(aisle) cmap_int_create(aisle)

#define CMAP_SET(map, key, val) \
  CMAP_CALL_ARGS(map, set, key, (CMAP_MAP *)(val))
#define CMAP_GET(map, key) \
  CMAP_CALL_ARGS(map, get, key)

#define CMAP_SET_GLOBAL(keys, val) CMAP_SET_SPLIT(NULL, keys, val)
#define CMAP_GET_GLOBAL(keys) CMAP_GET_SPLIT(NULL, keys)

#define CMAP_SET_SPLIT(map, keys, val) \
  cmap_fw_set_split(map, keys, (CMAP_MAP *)(val))
#define CMAP_GET_SPLIT(map, keys) \
  cmap_fw_get_split(map, keys)

#define CMAP_PROC_ARGS(map, fn, args...) \
  cmap_fw_proc(map, fn, args, NULL)
#define CMAP_PROC(map, fn) \
  cmap_fw_proc(map, fn, NULL)
#define CMAP_PROC_GLOBAL_ARGS(fn, args...) \
  cmap_fw_proc_split(NULL, fn, args, NULL)
#define CMAP_PROC_GLOBAL(fn) \
  cmap_fw_proc_split(NULL, fn, NULL)

#endif
