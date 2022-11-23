#ifndef __CMAP_INT_H__
#define __CMAP_INT_H__

#include <cmap/core.h>
#include "cmap-map.h"
#include <stdint.h>
#include <cmap/nature.h>
#include "cmap-proc-ctx.h"

struct CMAP_INT_s
{
  CMAP_MAP super;

  void * internal;

  int64_t (*get)(CMAP_INT * this);
  void (*set)(CMAP_INT * this, int64_t val);
};

typedef struct
{
  CMAP_INT * (*create)(int64_t val, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);
  void (*init)(CMAP_INT * int_, int64_t val);
  CMAP_MAP * (*delete)(CMAP_INT * int_);

  int64_t (*get)(CMAP_INT * this);
  void (*set)(CMAP_INT * this, int64_t val);
} CMAP_INT_PUBLIC;

extern const CMAP_INT_PUBLIC cmap_int_public;

#endif
