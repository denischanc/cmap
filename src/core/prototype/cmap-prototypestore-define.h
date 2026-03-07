#ifndef __CMAP_PROTOTYPESTORE_DEFINE_H__
#define __CMAP_PROTOTYPESTORE_DEFINE_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_PROTOTYPESTORE_LOOP(macro) \
  macro(map) \
  macro(list) \
  macro(fn) \
  macro(string) \
  macro(int) \
  macro(double) \
  macro(ptr)

#define CMAP_PROTOTYPESTORE_DECL(type) \
  CMAP_MAP * cmap_prototypestore_require_##type(CMAP_PROTOTYPESTORE * ps, \
    CMAP_PROC_CTX * proc_ctx); \
  CMAP_MAP * cmap_prototypestore_##type(CMAP_PROTOTYPESTORE * ps, \
    CMAP_PROC_CTX * proc_ctx);

#endif
