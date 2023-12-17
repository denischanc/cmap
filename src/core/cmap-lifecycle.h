#ifndef __CMAP_LIFECYCLE_H__
#define __CMAP_LIFECYCLE_H__

#include "cmap-lifecycle-type.h"
#include "cmap-lifecycle-define.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-stack.h"

struct CMAP_LIFECYCLE
{
  void * internal;

  void (*delete)(CMAP_LIFECYCLE * this);

  const char * (*nature)(CMAP_LIFECYCLE * this);

  void (*inc_refs)(CMAP_LIFECYCLE * this);
  int (*nb_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs_only)(CMAP_LIFECYCLE * this);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_STACK_lc_ptr ** stack);
};

typedef struct
{
  void (*init)(CMAP_LIFECYCLE * this, CMAP_PROC_CTX * proc_ctx);
  void (*delete)(CMAP_LIFECYCLE * this);

  void (*inc_refs)(CMAP_LIFECYCLE * this);
  int (*nb_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs_only)(CMAP_LIFECYCLE * this);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_STACK_lc_ptr ** stack);
} CMAP_LIFECYCLE_PUBLIC;

extern const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public;

#endif
