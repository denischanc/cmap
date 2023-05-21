#ifndef __CMAP_LIFECYCLE_H__
#define __CMAP_LIFECYCLE_H__

#include "cmap-lifecycle-define.h"
#include "cmap-proc-ctx-type.h"

typedef struct CMAP_LIFECYCLE CMAP_LIFECYCLE;

struct CMAP_LIFECYCLE
{
  void * internal;

  CMAP_LIFECYCLE * (*delete)(CMAP_LIFECYCLE * this);

  char (*is_ref)(CMAP_LIFECYCLE * this);
};

typedef struct
{
  void (*init)(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);
  CMAP_LIFECYCLE * (*delete)(CMAP_LIFECYCLE * lc);

  char (*is_ref)(CMAP_LIFECYCLE * lc);
} CMAP_LIFECYCLE_PUBLIC;

extern const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public;

#endif
