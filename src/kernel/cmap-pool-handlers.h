#ifndef __CMAP_POOL_HANDLERS_H__
#define __CMAP_POOL_HANDLERS_H__

#include "cmap-list-type.h"
#include "cmap-string-type.h"
#include "cmap-int-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_POOL_HANDLER(TYPE, type) \
typedef struct \
{ \
  CMAP_##TYPE * (*create)(CMAP_PROC_CTX * proc_ctx); \
  void (*delete)(CMAP_##TYPE * e); \
  void (*clean)(CMAP_##TYPE * e); \
} CMAP_POOL_HANDLER_##TYPE##_PUBLIC; \
 \
extern const CMAP_POOL_HANDLER_##TYPE##_PUBLIC \
  cmap_pool_handler_##type##_public;

CMAP_POOL_HANDLER(LIST, list)
CMAP_POOL_HANDLER(STRING, string)
CMAP_POOL_HANDLER(INT, int)

#endif
