#ifndef __CMAP_POOL_DEFINE_H__
#define __CMAP_POOL_DEFINE_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-pool-handler-define.h"

#define CMAP_POOL_LOOP(macro) CMAP_POOL_HANDLER_LOOP(macro)

#define CMAP_POOL_DECL(NAME, name, type) \
typedef struct CMAP_POOL_##NAME CMAP_POOL_##NAME; \
 \
type cmap_pool_##name##_take(CMAP_POOL_##NAME * pool, \
  CMAP_PROC_CTX * proc_ctx); \
void cmap_pool_##name##_release(CMAP_POOL_##NAME * pool, type e); \
 \
CMAP_POOL_##NAME * cmap_pool_##name##_create(int size, \
  CMAP_PROC_CTX * proc_ctx);

#endif
