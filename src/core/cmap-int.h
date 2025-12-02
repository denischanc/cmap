#ifndef __CMAP_INT_H__
#define __CMAP_INT_H__

#include "cmap-int-type.h"
#include "cmap-int-define.h"
#include "cmap-int-ext.h"
#include "cmap-map.h"
#include <stdint.h>
#include "cmap-proc-ctx-type.h"
#include "cmap-lifecycle.h"

struct CMAP_INT
{
  CMAP_MAP super;

  void * internal;

  int64_t (*get)(CMAP_INT * this);
  CMAP_INT * (*set)(CMAP_INT * this, int64_t val);
};

typedef struct
{
  CMAP_INT * (*create)(int64_t val, CMAP_PROC_CTX * proc_ctx);
  CMAP_INT * (*init)(CMAP_INT * int_, CMAP_INITARGS * initargs, int64_t val);
  void (*delete)(CMAP_LIFECYCLE * this);

  int64_t (*get)(CMAP_INT * this);
  CMAP_INT * (*set)(CMAP_INT * this, int64_t val);
} CMAP_INT_PUBLIC;

extern const CMAP_INT_PUBLIC cmap_int_public;

#endif
