#ifndef __CMAP_PROC_CTX_DEFINE_H__
#define __CMAP_PROC_CTX_DEFINE_H__

#include "cmap-pool.h"

#define CMAP_PROC_CTX_POOL_DECL(NAME, name, type) \
  CMAP_POOL_##NAME * cmap_proc_ctx_pool_##name(CMAP_PROC_CTX * proc_ctx);

#endif
