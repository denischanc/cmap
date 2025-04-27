#ifndef __CMAP_REFSWATCHER_H__
#define __CMAP_REFSWATCHER_H__

#include "cmap-lifecycle-type.h"

typedef struct CMAP_REFSWATCHER CMAP_REFSWATCHER;

struct CMAP_REFSWATCHER
{
  void (*delete)(CMAP_REFSWATCHER * this);

  void (*add)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);
  void (*upd)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);
  void (*rm)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);

  char (*is_zombie)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);
  void (*delete_if_zombie)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);

  void (*watch)(CMAP_REFSWATCHER * this);
};

typedef struct
{
  CMAP_REFSWATCHER * (*create)();
} CMAP_REFSWATCHER_PUBLIC;

extern const CMAP_REFSWATCHER_PUBLIC cmap_refswatcher_public;

#endif
