#ifndef __CMAP_POOL_HANDLERS_DEFINE_H__
#define __CMAP_POOL_HANDLERS_DEFINE_H__

#include "cmap-proc-ctx-type.h"

#define CMAP_POOL_HANDLER(TYPE, type) \
typedef struct \
{ \
  CMAP_##TYPE * (*create)(CMAP_PROC_CTX * proc_ctx); \
  void (*clean)(CMAP_##TYPE * e); \
} CMAP_POOL_HANDLER_##TYPE##_PUBLIC; \
 \
extern const CMAP_POOL_HANDLER_##TYPE##_PUBLIC \
  cmap_pool_handler_##type##_public;

#endif
