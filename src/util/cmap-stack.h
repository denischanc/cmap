#ifndef __CMAP_STACK_H__
#define __CMAP_STACK_H__

#include "cmap-stack-define.h"
#include "cmap-lifecycle-type.h"
#include "cmap-proc-ctx-type.h"

CMAP_STACK_DECL(LC, lc, CMAP_LIFECYCLE *)
CMAP_STACK_DECL(LC_PTR, lc_ptr, CMAP_LIFECYCLE **)
CMAP_STACK_DECL(PROC_CTX, proc_ctx, CMAP_PROC_CTX *)
CMAP_STACK_DECL(CHAR_PTR, char_ptr, char *)

#endif
