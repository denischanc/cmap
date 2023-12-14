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

  CMAP_INT_OP_LOOP(CMAP_INT_OP_DECL)

  CMAP_INT_STEP_LOOP(CMAP_INT_STEP_DECL)
};

typedef struct
{
  CMAP_INT * (*create)(int64_t val, CMAP_PROC_CTX * proc_ctx);
  void (*init)(CMAP_INT * int_, int64_t val);
  void (*delete)(CMAP_INT * int_);

  int64_t (*get)(CMAP_INT * this);

  CMAP_INT_OP_LOOP(CMAP_INT_OP_DECL)

  CMAP_INT_STEP_LOOP(CMAP_INT_STEP_DECL)
} CMAP_INT_PUBLIC;

extern const CMAP_INT_PUBLIC cmap_int_public;

#endif
