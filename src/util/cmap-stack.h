#ifndef __CMAP_STACK_H__
#define __CMAP_STACK_H__

#include "cmap-stack-define.h"
#include "cmap-map-type.h"
#include "cmap-lifecycle-type.h"
#include "cmap-proc-ctx-type.h"

CMAP_STACK_DECL(map, CMAP_MAP *)
CMAP_STACK_DECL(lc, CMAP_LIFECYCLE *)
CMAP_STACK_DECL(lc_ptr, CMAP_LIFECYCLE **)
CMAP_STACK_DECL(proc_ctx, CMAP_PROC_CTX *)
CMAP_STACK_DECL(char_ptr, char *)

#endif
