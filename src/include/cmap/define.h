#ifndef __CMAP_DEFINE_H__
#define __CMAP_DEFINE_H__

#include <stdlib.h>

#define CMAP_T (1 == 1)
#define CMAP_F (1 == 0)

#define CMAP_MAP(aisle) cmap_map(aisle)
#define CMAP_LIST(size_inc, aisle) cmap_list(size_inc, aisle)
#define CMAP_FN(process, aisle) cmap_fn(process, aisle)
#define CMAP_STRING(val, size_inc, aisle) cmap_string(val, size_inc, aisle)
#define CMAP_INT(aisle) cmap_int(aisle)

#define CMAP_NATURE(map) cmap_nature((CMAP_MAP *)(map))
#define CMAP_DELETE(map) cmap_delete((CMAP_MAP *)(map))
#define CMAP_SET(map, key, val) cmap_set( \
  (CMAP_MAP *)(map), key, (CMAP_MAP *)(val))
#define CMAP_GET(map, key) cmap_get((CMAP_MAP *)(map), key)

#define CMAP_LIST_SET(list, i, val) cmap_list_set(list, i, (CMAP_MAP *)(val))
#define CMAP_LIST_GET(list, i) cmap_list_get(list, i)

#define CMAP_SET_GLOBAL(keys, val) CMAP_SET_SPLIT(NULL, keys, val)
#define CMAP_GET_GLOBAL(keys) CMAP_GET_SPLIT(NULL, keys)

#define CMAP_SET_SPLIT(map, keys, val) \
  cmap_set_split(map, keys, (CMAP_MAP *)(val))
#define CMAP_GET_SPLIT(map, keys) cmap_get_split(map, keys)

#define CMAP_PROC_ARGS(map, fn, args...) cmap_proc(map, fn, args, NULL)
#define CMAP_PROC(map, fn) cmap_proc(map, fn, NULL)
#define CMAP_PROC_GLOBAL_ARGS(fn, args...) \
  cmap_proc_split(NULL, fn, args, NULL)
#define CMAP_PROC_GLOBAL(fn) cmap_proc_split(NULL, fn, NULL)

#endif
