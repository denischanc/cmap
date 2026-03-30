#ifndef __CMAP_CORE_H__
#define __CMAP_CORE_H__

#include "cmap-core-define.h"
#include "cmap-core-ext.h"
#include "cmap-lifecycle-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-slist.h"

CMAP_CORE_LOOP(CMAP_CORE_NATURE_CHAR_DECL)

void cmap_core_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx);
void cmap_core_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);

const char * cmap_core_nature_char(CMAP_LIFECYCLE * lc);

void cmap_core_log_sizeof(char lvl);

#endif
