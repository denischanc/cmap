#ifndef __CMAP_REFSWATCHER_H__
#define __CMAP_REFSWATCHER_H__

#include "cmap-lifecycle-type.h"
#include "cmap-env-type.h"

typedef struct CMAP_REFSWATCHER CMAP_REFSWATCHER;

struct CMAP_REFSWATCHER
{
  void (*delete)(CMAP_REFSWATCHER * this);

  char (*add)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);
  void (*rm)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);

  void (*stop)(CMAP_REFSWATCHER * this);
};

typedef struct
{
  CMAP_REFSWATCHER * (*create)(CMAP_ENV * env);

#ifdef CONSUMED_TIME
  void (*log_consumed_time)(char lvl);
#endif
} CMAP_REFSWATCHER_PUBLIC;

extern const CMAP_REFSWATCHER_PUBLIC cmap_refswatcher_public;

#endif
