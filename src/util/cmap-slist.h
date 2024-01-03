#ifndef __CMAP_SLIST_H__
#define __CMAP_SLIST_H__

#include "cmap-slist-define.h"
#include "cmap-lifecycle-type.h"
#include "cmap-proc-ctx-type.h"

CMAP_SLIST_DECL(LC, lc, CMAP_LIFECYCLE *)
CMAP_SLIST_DECL(LC_PTR, lc_ptr, CMAP_LIFECYCLE **)
CMAP_SLIST_DECL(PROC_CTX, proc_ctx, CMAP_PROC_CTX *)
CMAP_SLIST_DECL(CHAR_PTR, char_ptr, char *)

#endif
