#ifndef ___CMAP_DEFINE_MIN_H___
#define ___CMAP_DEFINE_MIN_H___

#define $MAP(proc_ctx, aisle) CMAP_MAP(proc_ctx, aisle)
#define $MAP_L(proc_ctx) CMAP_LOCAL_MAP(proc_ctx)

#define $LIST(size_inc, proc_ctx, aisle) CMAP_LIST(size_inc, proc_ctx, aisle)
#define $LIST_L(proc_ctx) CMAP_LOCAL_LIST(proc_ctx)

#define $FN(process, proc_ctx, aisle) CMAP_FN(process, proc_ctx, aisle)
#define $FN_L(process, proc_ctx) CMAP_LOCAL_FN(process, proc_ctx)

#define $STR(val, size_inc, proc_ctx, aisle) \
  CMAP_STRING(val, size_inc, proc_ctx, aisle)
#define $STR_L(val, proc_ctx) CMAP_LOCAL_STRING(val, proc_ctx)

#define $INT(val, proc_ctx, aisle) CMAP_INT(val, proc_ctx, aisle)
#define $INT_L(val, proc_ctx) CMAP_LOCAL_INT(val, proc_ctx)

#define $DOUBLE(val, proc_ctx, aisle) CMAP_DOUBLE(val, proc_ctx, aisle)
#define $DOUBLE_L(val, proc_ctx) CMAP_LOCAL_DOUBLE(val, proc_ctx)

#define $PTR(struct, proc_ctx, aisle) CMAP_PTR(struct, proc_ctx, aisle)
#define $PTR_L(struct, proc_ctx) CMAP_LOCAL_PTR(struct, proc_ctx)

#define $SET(map, key, val) CMAP_SET(map, key, val)
#define $GET(map, key) CMAP_GET(map, key)

#define $SETL(list, i, val) CMAP_LIST_SET(list, i, val)
#define $GETL(list, i) CMAP_LIST_GET(list, i)

#define $SETI(i, v) CMAP_INT_SET(i, v)
#define $GETI(i) CMAP_INT_GET(i)

#define $SETD(d, v) CMAP_DOUBLE_SET(d, v)
#define $GETD(d) CMAP_DOUBLE_GET(d)

#define $GETP(ptr) CMAP_PTR_GET(ptr)

#define $STRV(s) CMAP_STRING_VAL(s)

#define $NEW(prototype, proc_ctx, aisle) CMAP_NEW(prototype, proc_ctx, aisle)
#define $NEW_A(prototype, proc_ctx, aisle, args...) \
  CMAP_NEW_ARGS(prototype, proc_ctx, aisle, args)
#define $NEW_L(prototype, proc_ctx, aisle, args) \
  CMAP_LNEW(prototype, proc_ctx, aisle, args)

#define $PROC_FN(fn, proc_ctx, map) CMAP_FN_PROC(fn, proc_ctx, map)
#define $PROC_FN_A(fn, proc_ctx, map, args...) \
  CMAP_FN_PROC_ARGS(fn, proc_ctx, map, args)

#define $$(map, key, proc_ctx) CMAP_PROC(map, key, proc_ctx)
#define $$_A(map, key, proc_ctx, args...) \
  CMAP_PROC_ARGS(map, key, proc_ctx, args)
#define $$_L(map, key, proc_ctx, args) CMAP_LPROC(map, key, proc_ctx, args)

#define $$LIST(proc_ctx, aisle, maps...) CMAP_TO_LIST(proc_ctx, aisle, maps)
#define $$LIST_L(proc_ctx, maps...) CMAP_TO_LOCAL_LIST(proc_ctx, maps)

#define $$MAP(proc_ctx, aisle, key_maps...) \
  CMAP_TO_MAP(proc_ctx, aisle, key_maps)
#define $$MAP_L(proc_ctx, key_maps...) CMAP_TO_LOCAL_MAP(proc_ctx, key_maps)

#endif
