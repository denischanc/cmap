#ifndef __CMAP_DOUBLE_H__
#define __CMAP_DOUBLE_H__

#include "cmap-double-type.h"
#include "cmap-double-define.h"
#include "cmap-double-ext.h"
#include "cmap-map.h"

struct CMAP_DOUBLE
{
  CMAP_MAP super;

  void * internal;

  double (*get)(CMAP_DOUBLE * this);
  void (*set)(CMAP_DOUBLE * this, double val);
};

typedef struct
{
  CMAP_DOUBLE * (*create)(double val, CMAP_PROC_CTX * proc_ctx);
  void (*init)(CMAP_DOUBLE * double_, double val);
  void (*delete)(CMAP_LIFECYCLE * this);

  double (*get)(CMAP_DOUBLE * this);
  void (*set)(CMAP_DOUBLE * this, double val);
} CMAP_DOUBLE_PUBLIC;

extern const CMAP_DOUBLE_PUBLIC cmap_double_public;

#endif
