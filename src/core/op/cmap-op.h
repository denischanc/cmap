#ifndef __CMAP_OP_H__
#define __CMAP_OP_H__

#include "cmap-op-define.h"

typedef struct
{
  CMAP_OP_LOOP(CMAP_OP_DECL)

  CMAP_OP_SELF_LOOP(CMAP_OP_SELF_DECL)

  CMAP_OP_UNIQUE_LOOP(CMAP_OP_UNIQUE_DECL)
} CMAP_OP_PUBLIC;

extern const CMAP_OP_PUBLIC cmap_op_public;

#endif
