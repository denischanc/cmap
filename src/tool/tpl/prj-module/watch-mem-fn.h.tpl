#ifndef __WATCH_MEM_FN_H__
#define __WATCH_MEM_FN_H__

#include <cmap/cmap-ext.h>

CMAP_MAP * watch_mem_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args);
CMAP_MAP * watch_mem_stop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args);

#endif
