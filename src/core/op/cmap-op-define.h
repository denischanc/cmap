#ifndef __CMAP_OP_DEFINE_H__
#define __CMAP_OP_DEFINE_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_OP_LOOP(macro) \
  macro(add, +) \
  macro(sub, -) \
  macro(mul, *) \
  macro(div, /)

#define CMAP_OP_SELF_LOOP(macro) \
  macro(add_self, +=) \
  macro(sub_self, -=) \
  macro(mul_self, *=) \
  macro(div_self, /=)

#define CMAP_OP_UNIQUE_LOOP(macro) \
  macro(inc, ++) \
  macro(dec, --)

#define CMAP_OP_DECL(name, OP) \
  CMAP_MAP * (*name)(CMAP_MAP * map_l, CMAP_MAP * map_r, \
    CMAP_PROC_CTX * proc_ctx);

#define CMAP_OP_SELF_DECL(name, OP) \
  void (*name)(CMAP_MAP * map_dst, CMAP_MAP * map_src);

#define CMAP_OP_UNIQUE_DECL(name, OP) void (*name)(CMAP_MAP * map);

#endif
