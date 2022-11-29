#ifndef ___CMAP_DEFINE_H___
#define ___CMAP_DEFINE_H___

#include <stdlib.h>
#include <cmap/super-define.h>
#include <cmap/aisle.h>

#define CMAP_MAP(proc_ctx, aisle) cmap_map(proc_ctx, aisle)
#define CMAP_LOCAL_MAP(proc_ctx) CMAP_MAP(proc_ctx, CMAP_AISLE_LOCAL)

#define CMAP_LIST(size_inc, proc_ctx, aisle) \
  cmap_list(size_inc, proc_ctx, aisle)
#define CMAP_LOCAL_LIST(proc_ctx) CMAP_LIST(0, proc_ctx, CMAP_AISLE_LOCAL)

#define CMAP_FN(process, proc_ctx, aisle) cmap_fn(process, proc_ctx, aisle)
#define CMAP_LOCAL_FN(process, proc_ctx) \
  CMAP_FN(process, proc_ctx, CMAP_AISLE_LOCAL)

#define CMAP_STRING(val, size_inc, proc_ctx, aisle) \
  cmap_string(val, size_inc, proc_ctx, aisle)
#define CMAP_LOCAL_STRING(val, proc_ctx) \
  CMAP_STRING(val, 0, proc_ctx, CMAP_AISLE_LOCAL)

#define CMAP_INT(val, proc_ctx, aisle) cmap_int(val, proc_ctx, aisle)
#define CMAP_LOCAL_INT(val, proc_ctx) CMAP_INT(val, proc_ctx, CMAP_AISLE_LOCAL)

#define CMAP_DOUBLE(val, proc_ctx, aisle) cmap_double(val, proc_ctx, aisle)
#define CMAP_LOCAL_DOUBLE(val, proc_ctx) \
  CMAP_DOUBLE(val, proc_ctx, CMAP_AISLE_LOCAL)

#define CMAP_PTR(struct, proc_ctx, aisle) \
  cmap_ptr(sizeof(struct), proc_ctx, aisle)
#define CMAP_LOCAL_PTR(struct, proc_ctx) \
  CMAP_PTR(struct, proc_ctx, CMAP_AISLE_LOCAL)

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

#define CMAP_NEW(prototype, proc_ctx, aisle) \
  cmap_new(prototype, proc_ctx, aisle, NULL)
#define CMAP_NEW_ARGS(prototype, proc_ctx, aisle, args...) \
  cmap_new(prototype, proc_ctx, aisle, args, NULL)
#define CMAP_LNEW(prototype, proc_ctx, aisle, args) \
  cmap_lnew(prototype, proc_ctx, aisle, args)

#define CMAP_FN_PROC(fn, proc_ctx, map) \
  cmap_fn_proc(fn, proc_ctx, (CMAP_MAP *)map, NULL)
#define CMAP_FN_PROC_ARGS(fn, proc_ctx, map, args...) \
  cmap_fn_proc(fn, proc_ctx, (CMAP_MAP *)map, args, NULL)

#define CMAP_PROC(map, key, proc_ctx) \
  cmap_proc((CMAP_MAP *)map, key, proc_ctx, NULL)
#define CMAP_PROC_ARGS(map, key, proc_ctx, args...) \
  cmap_proc((CMAP_MAP *)map, key, proc_ctx, args, NULL)
#define CMAP_LPROC(map, key, proc_ctx, args) \
  cmap_lproc((CMAP_MAP *)map, key, proc_ctx, args)

#define CMAP_TO_LIST(proc_ctx, aisle, maps...) \
  cmap_to_list(proc_ctx, aisle, maps, NULL)
#define CMAP_TO_LOCAL_LIST(proc_ctx, maps...) \
  CMAP_TO_LIST(proc_ctx, CMAP_AISLE_LOCAL, maps)

#define CMAP_TO_MAP(proc_ctx, aisle, key_maps...) \
  cmap_to_map(proc_ctx, aisle, key_maps, NULL)
#define CMAP_TO_LOCAL_MAP(proc_ctx, key_maps...) \
  CMAP_TO_MAP(proc_ctx, CMAP_AISLE_LOCAL, key_maps)

#define CMAP_DELETE_AISLE(proc_ctx, aisle) cmap_delete_aisle(proc_ctx, aisle)

#endif
