#ifndef __CMAP_FN_DEFINE_H__
#define __CMAP_FN_DEFINE_H__

#include "cmap.h"

#define CMAP_FN(process, proc_ctx, aisle) \
  cmap_fn_public.create(process, proc_ctx, aisle)

#define CMAP_FN_PROC(fn, proc_ctx, map, args) \
  CMAP_CALL_ARGS(fn, process, proc_ctx, (CMAP_MAP *)map, args)

#endif
