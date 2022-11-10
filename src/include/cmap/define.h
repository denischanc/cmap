#ifndef ___CMAP_DEFINE_H___
#define ___CMAP_DEFINE_H___

#include <stdlib.h>
#include <cmap/super-define.h>
#include <cmap/aisle.h>

#define CMAP_MAP(aisle) cmap_map(aisle)
#define CMAP_LOCAL_MAP() CMAP_MAP(CMAP_AISLE_LOCAL)

#define CMAP_LIST(size_inc, aisle) cmap_list(size_inc, aisle)
#define CMAP_LOCAL_LIST() CMAP_LIST(0, CMAP_AISLE_LOCAL)

#define CMAP_FN(process, aisle) cmap_fn(process, aisle)
#define CMAP_LOCAL_FN(process) CMAP_FN(process, CMAP_AISLE_LOCAL)

#define CMAP_STRING(val, size_inc, aisle) cmap_string(val, size_inc, aisle)
#define CMAP_LOCAL_STRING(val) CMAP_STRING(val, 0, CMAP_AISLE_LOCAL)

#define CMAP_INT(val, aisle) cmap_int(val, aisle)
#define CMAP_LOCAL_INT(val) CMAP_INT(val, CMAP_AISLE_LOCAL)

#define CMAP_DOUBLE(val, aisle) cmap_double(val, aisle)
#define CMAP_LOCAL_DOUBLE(val) CMAP_DOUBLE(val, CMAP_AISLE_LOCAL)

#define CMAP_PTR(struct, aisle) cmap_ptr(sizeof(struct), aisle)
#define CMAP_LOCAL_PTR(struct) CMAP_PTR(struct, CMAP_AISLE_LOCAL)

#define CMAP_NATURE(map) cmap_nature((CMAP_MAP *)map)
#define CMAP_DELETE(map) cmap_delete((CMAP_MAP *)map)
#define CMAP_SET(map, key, val) cmap_set( \
  (CMAP_MAP *)map, key, (CMAP_MAP *)val)
#define CMAP_GET(map, key) cmap_get((CMAP_MAP *)map, key)

#define CMAP_LIST_SET(list, i, val) cmap_list_set(list, i, (CMAP_MAP *)val)
#define CMAP_LIST_GET(list, i) cmap_list_get(list, i)

#define CMAP_INT_SET(i, v) cmap_int_set(i, v)
#define CMAP_INT_GET(i) cmap_int_get(i)

#define CMAP_DOUBLE_SET(d, v) cmap_double_set(d, v)
#define CMAP_DOUBLE_GET(d) cmap_double_get(d)

#define CMAP_PTR_GET(ptr) cmap_ptr_get(ptr)

#define CMAP_STRING_VAL(s) cmap_string_val(s)

#define CMAP_SET_SPLIT(map, keys, val) \
  cmap_set_split(map, keys, (CMAP_MAP *)val)
#define CMAP_GET_SPLIT(map, keys) cmap_get_split(map, keys)

#define CMAP_SET_GLOBAL(keys, val) CMAP_SET_SPLIT(NULL, keys, val)
#define CMAP_GET_GLOBAL(keys) CMAP_GET_SPLIT(NULL, keys)

#define CMAP_NEW(prototype, aisle) cmap_new(prototype, aisle, NULL)
#define CMAP_NEW_ARGS(prototype, aisle, args...) \
  cmap_new(prototype, aisle, args, NULL)
#define CMAP_LNEW(prototype, aisle, args) cmap_lnew(prototype, aisle, args)

#define CMAP_FN_PROC(fn, map) cmap_fn_proc(fn, (CMAP_MAP *)map, NULL)
#define CMAP_FN_PROC_ARGS(fn, map, args...) \
  cmap_fn_proc(fn, (CMAP_MAP *)map, args, NULL)

#define CMAP_PROC(map, key) cmap_proc((CMAP_MAP *)map, key, NULL)
#define CMAP_PROC_ARGS(map, key, args...) \
  cmap_proc((CMAP_MAP *)map, key, args, NULL)
#define CMAP_LPROC(map, key, args) cmap_lproc((CMAP_MAP *)map, key, args)

#define CMAP_PROC_SPLIT(map, keys) cmap_proc_split(map, keys, NULL)
#define CMAP_PROC_SPLIT_ARGS(map, keys, args...) \
  cmap_proc_split(map, keys, args, NULL)
#define CMAP_LPROC_SPLIT(map, keys, args) cmap_lproc_split(map, keys, args)

#define CMAP_PROC_GLOBAL(keys) CMAP_PROC_SPLIT(NULL, keys)
#define CMAP_PROC_GLOBAL_ARGS(keys, args...) \
  CMAP_PROC_SPLIT_ARGS(NULL, keys, args)

#define CMAP_PROC_CHAIN(map, args...) cmap_proc_chain(map, args, NULL)
#define CMAP_PROC_CHAIN_GLOBAL(args...) CMAP_PROC_CHAIN(NULL, args)

#define CMAP_TO_LIST(aisle, maps...) cmap_to_list(aisle, maps, NULL)
#define CMAP_TO_LOCAL_LIST(maps...) CMAP_TO_LIST(CMAP_AISLE_LOCAL, maps)

#define CMAP_TO_MAP(aisle, key_maps...) cmap_to_map(aisle, key_maps, NULL)
#define CMAP_TO_LOCAL_MAP(key_maps...) CMAP_TO_MAP(CMAP_AISLE_LOCAL, key_maps)

#define CMAP_DELETE_AISLE(aisle) cmap_delete_aisle(aisle)

#endif
