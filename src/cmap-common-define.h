#ifndef __CMAP_COMMON_DEFINE_H__
#define __CMAP_COMMON_DEFINE_H__

#include "cmap-common-super-define.h"
#include "cmap-aisle.h"

#define CMAP_MAP(proc_ctx, aisle) cmap_map_public.create(proc_ctx, aisle)
#define CMAP_KERNEL_MAP(proc_ctx) CMAP_MAP(proc_ctx, CMAP_AISLE_KERNEL)
#define CMAP_LIST(size_inc, proc_ctx, aisle) \
  cmap_list_public.create(size_inc, proc_ctx, aisle)
#define CMAP_FN(process, proc_ctx, aisle) \
  cmap_fn_public.create(process, proc_ctx, aisle)
#define CMAP_KERNEL_FN(process, proc_ctx) \
  CMAP_FN(process, proc_ctx, CMAP_AISLE_KERNEL)
#define CMAP_STRING(val, size_inc, proc_ctx, aisle) \
  cmap_string_public.create(val, size_inc, proc_ctx, aisle)
#define CMAP_INT(val, proc_ctx, aisle) \
  cmap_int_public.create(val, proc_ctx, aisle)
#define CMAP_DOUBLE(val, proc_ctx, aisle) \
  cmap_double_public.create(val, proc_ctx, aisle)

#define CMAP_NATURE(map) CMAP_CALL((CMAP_MAP *)map, nature)
#define CMAP_DELETE(map) CMAP_CALL((CMAP_MAP *)map, delete)
#define CMAP_SET(map, key, val) \
  CMAP_CALL_ARGS((CMAP_MAP *)map, set, key, (CMAP_MAP *)val)
#define CMAP_GET(map, key) CMAP_CALL_ARGS((CMAP_MAP *)map, get, key)

#define CMAP_LIST_SET(list, i, val) \
  CMAP_CALL_ARGS(list, set, i, (CMAP_MAP *)val)
#define CMAP_LIST_GET(list, i) CMAP_CALL_ARGS(list, get, i)

#define CMAP_FN_PROC(fn, proc_ctx, map, args) \
  CMAP_CALL_ARGS(fn, process, proc_ctx, (CMAP_MAP *)map, args)

#endif
