#ifndef __CMAP_LIFECYCLE_H__
#define __CMAP_LIFECYCLE_H__

#include "cmap-lifecycle-type.h"
#include "cmap-lifecycle-define.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-slist.h"
#include "cmap-initargs.h"

struct CMAP_LIFECYCLE
{
  void * internal;

  void (*delete)(CMAP_LIFECYCLE * this);

  const char * (*nature)(CMAP_LIFECYCLE * this);

  void (*inc_refs)(CMAP_LIFECYCLE * this);
  int (*nb_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs_only)(CMAP_LIFECYCLE * this);
  void (*dec_refs_only_nb)(CMAP_LIFECYCLE * this, int nb);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list);
};

typedef struct
{
  CMAP_LIFECYCLE * (*init)(CMAP_LIFECYCLE * this, CMAP_INITARGS * initargs);
  void (*delete)(CMAP_LIFECYCLE * this);

  void (*inc_refs)(CMAP_LIFECYCLE * this);
  int (*nb_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs)(CMAP_LIFECYCLE * this);
  void (*dec_refs_only)(CMAP_LIFECYCLE * this);
  void (*dec_refs_only_nb)(CMAP_LIFECYCLE * this, int nb);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list);
} CMAP_LIFECYCLE_PUBLIC;

extern const CMAP_LIFECYCLE_PUBLIC cmap_lifecycle_public;

#endif
