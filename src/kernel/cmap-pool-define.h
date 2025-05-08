#ifndef __CMAP_POOL_DEFINE_H__
#define __CMAP_POOL_DEFINE_H__

#include "cmap-lifecycle.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-pool-handler-define.h"

#define CMAP_POOL_LOOP(macro) CMAP_POOL_HANDLER_LOOP(macro)

#define CMAP_POOL_DECL(NAME, name, type) \
typedef struct CMAP_POOL_##NAME CMAP_POOL_##NAME; \
 \
struct CMAP_POOL_##NAME \
{ \
  CMAP_LIFECYCLE super; \
 \
  type (*take)(CMAP_POOL_##NAME * this, CMAP_PROC_CTX * proc_ctx); \
  void (*release)(CMAP_POOL_##NAME * this, type e); \
}; \
 \
typedef struct \
{ \
  CMAP_POOL_##NAME * (*create)(int size, CMAP_PROC_CTX * proc_ctx); \
} CMAP_POOL_##NAME##_PUBLIC; \
 \
extern const CMAP_POOL_##NAME##_PUBLIC cmap_pool_##name##_public;

#endif
