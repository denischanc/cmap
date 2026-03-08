#ifndef __CMAP_INT_H__
#define __CMAP_INT_H__

#include "cmap-int-define.h"
#include "cmap-int-ext.h"
#include "cmap-int-int.h"
#include "cmap-map.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-lifecycle.h"

struct CMAP_INT
{
  CMAP_MAP super;

  CMAP_INT_INTERNAL internal;
};

CMAP_INT * cmap_int_init(CMAP_INT * i, CMAP_INITARGS * initargs, int64_t val);
CMAP_INT * cmap_int_create(int64_t val, CMAP_PROC_CTX * proc_ctx);

#endif
