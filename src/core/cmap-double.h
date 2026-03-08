#ifndef __CMAP_DOUBLE_H__
#define __CMAP_DOUBLE_H__

#include "cmap-double-define.h"
#include "cmap-double-ext.h"
#include "cmap-double-int.h"
#include "cmap-map.h"

struct CMAP_DOUBLE
{
  CMAP_MAP super;

  CMAP_DOUBLE_INTERNAL internal;
};

CMAP_DOUBLE * cmap_double_init(CMAP_DOUBLE * d, CMAP_INITARGS * initargs,
  double val);
CMAP_DOUBLE * cmap_double_create(double val, CMAP_PROC_CTX * proc_ctx);

#endif
