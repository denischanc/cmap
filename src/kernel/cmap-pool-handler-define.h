#ifndef __CMAP_POOL_HANDLER_DEFINE_H__
#define __CMAP_POOL_HANDLER_DEFINE_H__

#include "cmap-list-type.h"
#include "cmap-string-type.h"
#include "cmap-int-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_POOL_HANDLER_LOOP(macro) \
  macro(LIST, list) \
  macro(STRING, string) \
  macro(INT, int)

#define CMAP_POOL_HANDLER_DECL(TYPE, type) \
typedef struct \
{ \
  CMAP_##TYPE * (*create)(CMAP_PROC_CTX * proc_ctx); \
  void (*clean)(CMAP_##TYPE * this); \
} CMAP_POOL_HANDLER_##TYPE##_PUBLIC; \
 \
extern const CMAP_POOL_HANDLER_##TYPE##_PUBLIC \
  cmap_pool_handler_##type##_public;

#endif
