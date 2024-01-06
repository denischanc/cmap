#ifndef __CMAP_POOL_DEFINE_H__
#define __CMAP_POOL_DEFINE_H__

#include "cmap-lifecycle.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-pool-handler-define.h"

#define CMAP_POOL_LOOP(macro) CMAP_POOL_HANDLER_LOOP(macro)

#define CMAP_POOL_DECL(TYPE, type) \
typedef struct CMAP_POOL_##TYPE CMAP_POOL_##TYPE; \
 \
struct CMAP_POOL_##TYPE \
{ \
  CMAP_LIFECYCLE super; \
 \
  CMAP_##TYPE * (*take)(CMAP_POOL_##TYPE * this, CMAP_PROC_CTX * proc_ctx); \
  void (*release)(CMAP_POOL_##TYPE * this, CMAP_##TYPE * e); \
}; \
 \
typedef struct \
{ \
  CMAP_POOL_##TYPE * (*create)(int size, CMAP_PROC_CTX * proc_ctx); \
} CMAP_POOL_##TYPE##_PUBLIC; \
 \
extern const CMAP_POOL_##TYPE##_PUBLIC cmap_pool_##type##_public;

#endif
