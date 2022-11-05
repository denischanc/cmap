#ifndef ___CMAP_DEFINE_MIN_H___
#define ___CMAP_DEFINE_MIN_H___

#define $M(aisle) CMAP_MAP(aisle)
#define $ML() CMAP_LOCAL_MAP()

#define $L(size_inc, aisle) CMAP_LIST(size_inc, aisle)
#define $LL() CMAP_LOCAL_LIST()

#define $F(process, aisle) CMAP_FN(process, aisle)
#define $FL(process) CMAP_LOCAL_FN(process)

#define $S(val, size_inc, aisle) CMAP_STRING(val, size_inc, aisle)
#define $SL(val) CMAP_LOCAL_STRING(val)

#define $I(aisle) CMAP_INT(aisle)
#define $IL() CMAP_LOCAL_INT()

#define $_S(map, key, val) CMAP_SET(map, key, val)
#define $_G(map, key) CMAP_GET(map, key)

#define $_SI(list, i, val) CMAP_LIST_SET(list, i, val)
#define $_GI(list, i) CMAP_LIST_GET(list, i)

#define $_S_(map, keys, val) CMAP_SET_SPLIT(map, keys, val)
#define $_G_(map, keys) CMAP_GET_SPLIT(map, keys)

#define $_SG(keys, val) CMAP_SET_GLOBAL(keys, val)
#define $_GG(keys) CMAP_GET_GLOBAL(keys)

#define $$F(fn, map) CMAP_FN_PROC(fn, map)
#define $$FA(fn, map, args...) CMAP_FN_PROC_ARGS(fn, map, args)

#define $$(map, key) CMAP_PROC(map, key)
#define $$A(map, key, args...) CMAP_PROC_ARGS(map, key, args)
#define $$L(map, key, args) CMAP_LPROC(map, key, args)

#define $$_(map, keys) CMAP_PROC_SPLIT(map, keys)
#define $$_A(map, keys, args...) CMAP_PROC_SPLIT_ARGS(map, keys, args)
#define $$_L(map, keys, args) CMAP_LPROC_SPLIT(map, keys, args)

#define $$G(keys) CMAP_PROC_GLOBAL(keys)
#define $$GA(keys, args...) CMAP_PROC_GLOBAL_ARGS(keys, args)

#define $$$(map, args...) CMAP_PROC_CHAIN(map, args)
#define $$$G(args...) CMAP_PROC_CHAIN_GLOBAL(args)

#define L$(maps...) CMAP_TO_LIST(maps)

#endif
