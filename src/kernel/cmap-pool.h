#ifndef __CMAP_POOL_H__
#define __CMAP_POOL_H__

#include <cmap/proc-ctx.h>
#include "cmap-list.h"
#include "cmap-string.h"

#define CMAP_POOL(TYPE, type) \
typedef struct CMAP_POOL_##TYPE##_s CMAP_POOL_##TYPE; \
 \
struct CMAP_POOL_##TYPE##_s \
{ \
  void * internal; \
 \
  void (*delete)(CMAP_POOL_##TYPE * this); \
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

CMAP_POOL(LIST, list)
CMAP_POOL(STRING, string)

#endif
