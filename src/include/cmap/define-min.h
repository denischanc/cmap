#ifndef ___CMAP_DEFINE_MIN_H___
#define ___CMAP_DEFINE_MIN_H___

#define $MAP(aisle) CMAP_MAP(aisle)
#define $MAP_L() CMAP_LOCAL_MAP()

#define $LIST(size_inc, aisle) CMAP_LIST(size_inc, aisle)
#define $LIST_L() CMAP_LOCAL_LIST()

#define $FN(process, aisle) CMAP_FN(process, aisle)
#define $FN_L(process) CMAP_LOCAL_FN(process)

#define $STR(val, size_inc, aisle) CMAP_STRING(val, size_inc, aisle)
#define $STR_L(val) CMAP_LOCAL_STRING(val)

#define $INT(val, aisle) CMAP_INT(val, aisle)
#define $INT_L(val) CMAP_LOCAL_INT(val)

#define $DOUBLE(val, aisle) CMAP_DOUBLE(val, aisle)
#define $DOUBLE_L(val) CMAP_LOCAL_DOUBLE(val)

#define $SET(map, key, val) CMAP_SET(map, key, val)
#define $GET(map, key) CMAP_GET(map, key)

#define $SETL(list, i, val) CMAP_LIST_SET(list, i, val)
#define $GETL(list, i) CMAP_LIST_GET(list, i)

#define $SETI(i, v) CMAP_INT_SET(i, v)
#define $GETI(i) CMAP_INT_GET(i)

#define $SETD(d, v) CMAP_DOUBLE_SET(d, v)
#define $GETD(d) CMAP_DOUBLE_GET(d)

#define $STRV(s) CMAP_STRING_VAL(s)

#define $K_SET(map, keys, val) CMAP_SET_SPLIT(map, keys, val)
#define $K_GET(map, keys) CMAP_GET_SPLIT(map, keys)

#define $K_SET_G(keys, val) CMAP_SET_GLOBAL(keys, val)
#define $K_GET_G(keys) CMAP_GET_GLOBAL(keys)

#define $PROC_FN(fn, map) CMAP_FN_PROC(fn, map)
#define $PROC_FN_A(fn, map, args...) CMAP_FN_PROC_ARGS(fn, map, args)

#define $$(map, key) CMAP_PROC(map, key)
#define $$_A(map, key, args...) CMAP_PROC_ARGS(map, key, args)
#define $$_L(map, key, args) CMAP_LPROC(map, key, args)

#define $K_$(map, keys) CMAP_PROC_SPLIT(map, keys)
#define $K_$_A(map, keys, args...) CMAP_PROC_SPLIT_ARGS(map, keys, args)
#define $K_$_L(map, keys, args) CMAP_LPROC_SPLIT(map, keys, args)

#define $G_$(keys) CMAP_PROC_GLOBAL(keys)
#define $G_$_A(keys, args...) CMAP_PROC_GLOBAL_ARGS(keys, args)

#define $$$(map, args...) CMAP_PROC_CHAIN(map, args)
#define $G_$$(args...) CMAP_PROC_CHAIN_GLOBAL(args)

#define $$LIST(aisle, maps...) CMAP_TO_LIST(aisle, maps)
#define $$LIST_L(maps...) CMAP_TO_LOCAL_LIST(maps)

#define $$MAP(aisle, key_maps...) CMAP_TO_MAP(aisle, key_maps)
#define $$MAP_L(key_maps...) CMAP_TO_LOCAL_MAP(key_maps)

#endif
