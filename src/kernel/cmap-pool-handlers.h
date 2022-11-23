#ifndef __CMAP_POOL_HANDLERS_H__
#define __CMAP_POOL_HANDLERS_H__

#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-proc-ctx.h"

#define CMAP_POOL_HANDLER(TYPE, type) \
typedef struct \
{ \
  CMAP_##TYPE * (*create)(CMAP_PROC_CTX * proc_ctx); \
  void (*delete)(CMAP_##TYPE * e); \
  void (*clean)(CMAP_##TYPE * s); \
} CMAP_POOL_HANDLER_##TYPE##_PUBLIC; \
 \
extern const CMAP_POOL_HANDLER_##TYPE##_PUBLIC \
  cmap_pool_handler_##type##_public;

CMAP_POOL_HANDLER(LIST, list)
CMAP_POOL_HANDLER(STRING, string)

#endif
