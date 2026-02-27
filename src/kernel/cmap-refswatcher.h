#ifndef __CMAP_REFSWATCHER_H__
#define __CMAP_REFSWATCHER_H__

#include "cmap-lifecycle-type.h"
#include "cmap-env-type.h"

typedef struct CMAP_REFSWATCHER CMAP_REFSWATCHER;

void cmap_refswatcher_add(CMAP_REFSWATCHER * rw, CMAP_LIFECYCLE * lc);
void cmap_refswatcher_rm(CMAP_REFSWATCHER * rw, CMAP_LIFECYCLE * lc);

void cmap_refswatcher_stop(CMAP_REFSWATCHER * rw);

void cmap_refswatcher_delete(CMAP_REFSWATCHER * rw);
CMAP_REFSWATCHER * cmap_refswatcher_create(CMAP_ENV * env);

#ifdef CONSUMED_TIME
void cmap_refswatcher_log_consumed_time(char lvl);
#endif

#endif
