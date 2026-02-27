#ifndef __CMAP_REFSSTORE_H__
#define __CMAP_REFSSTORE_H__

#include "cmap-lifecycle-type.h"
#include "cmap-map-type.h"
#include "cmap-refswatcher.h"

typedef struct CMAP_REFSSTORE CMAP_REFSSTORE;

void cmap_refsstore_add(CMAP_REFSSTORE * rs, CMAP_LIFECYCLE * lc, char created);
void cmap_refsstore_rm(CMAP_REFSSTORE * rs, CMAP_LIFECYCLE * lc);

void cmap_refsstore_delete(CMAP_REFSSTORE * rs, CMAP_MAP * ret);
CMAP_REFSSTORE * cmap_refsstore_create(CMAP_REFSWATCHER * refswatcher);

#ifdef CONSUMED_TIME
void cmap_refsstore_log_consumed_time(char lvl);
#endif

#endif
