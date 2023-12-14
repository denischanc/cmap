#ifndef __CMAP_LIFECYCLE_H__
#define __CMAP_LIFECYCLE_H__

#include "cmap-lifecycle-define.h"
#include "cmap-proc-ctx-type.h"

typedef struct CMAP_LIFECYCLE CMAP_LIFECYCLE;

struct CMAP_LIFECYCLE
{
  void * internal;

  void (*delete)(CMAP_LIFECYCLE * this);
  void (*deep_delete)(CMAP_LIFECYCLE * this);

  void (*inc_ref)(CMAP_LIFECYCLE * this);
  void (*dec_ref)(CMAP_LIFECYCLE * this);
  char (*dec_ref_or_deep_delete_last_ref)(CMAP_LIFECYCLE * this);
};

typedef struct
{
  void (*init)(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);
  void (*delete)(CMAP_LIFECYCLE * lc);
  void (*deep_delete)(CMAP_LIFECYCLE * lc);

  void (*inc_ref)(CMAP_LIFECYCLE * lc);
  void (*dec_ref)(CMAP_LIFECYCLE * lc);
  char (*dec_ref_or_deep_delete_last_ref)(CMAP_LIFECYCLE * lc);
} CMAP_LIFECYCLE_PUBLIC;

extern const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public;

#endif
