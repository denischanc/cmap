#ifndef __CMAP_REFSWATCHER_H__
#define __CMAP_REFSWATCHER_H__

#include "cmap-lifecycle-type.h"
#include "cmap-env-type.h"

typedef struct CMAP_REFSWATCHER CMAP_REFSWATCHER;

struct CMAP_REFSWATCHER
{
  void (*delete)(CMAP_REFSWATCHER * this);

  void (*add)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);
  void (*rm)(CMAP_REFSWATCHER * this, CMAP_LIFECYCLE * lc);

  void (*stop)(CMAP_REFSWATCHER * this);
};

CMAP_REFSWATCHER * cmap_refswatcher_create(CMAP_ENV * env);

#ifdef CONSUMED_TIME
void cmap_refswatcher_log_consumed_time(char lvl);
#endif

#endif
