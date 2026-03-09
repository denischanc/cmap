#ifndef __CMAP_LIFECYCLE_H__
#define __CMAP_LIFECYCLE_H__

#include "cmap-lifecycle-type.h"
#include "cmap-lifecycle-define.h"
#include "cmap-lifecycle-int.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-slist.h"
#include "cmap-initargs.h"
#include "cmap-refswatcher.h"

struct CMAP_LIFECYCLE
{
  CMAP_LIFECYCLE_INTERNAL internal;

  void (*delete)(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);

  void (*nested)(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
    CMAP_PROC_CTX * proc_ctx);
};

const char * cmap_lifecycle_nature(CMAP_LIFECYCLE * lc);

void cmap_lifecycle_inc_refs(CMAP_LIFECYCLE * lc);
int cmap_lifecycle_nb_refs(CMAP_LIFECYCLE * lc);
void cmap_lifecycle_dec_refs(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);

void cmap_lifecycle_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx);

void cmap_lifecycle_watched(CMAP_LIFECYCLE * lc, char val);
char cmap_lifecycle_is_watched(CMAP_LIFECYCLE * lc);
uint64_t cmap_lifecycle_watch_time_us(CMAP_LIFECYCLE * lc);

void cmap_lifecycle_store(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);

char cmap_lifecycle_in_refs(CMAP_LIFECYCLE * lc);

void cmap_lifecycle_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);
CMAP_LIFECYCLE * cmap_lifecycle_init(CMAP_LIFECYCLE * lc,
  CMAP_INITARGS * initargs);

#endif
