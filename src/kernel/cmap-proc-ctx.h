#ifndef __CMAP_PROC_CTX_H__
#define __CMAP_PROC_CTX_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-proc-ctx-define.h"
#include "cmap-env-type.h"
#include "cmap-map-type.h"
#include "cmap-prototypestore.h"

CMAP_ENV * cmap_proc_ctx_env(CMAP_PROC_CTX * proc_ctx);
CMAP_PROTOTYPESTORE * cmap_proc_ctx_prototypestore(CMAP_PROC_CTX * proc_ctx);
CMAP_POOL_LOOP(CMAP_PROC_CTX_POOL_DECL)
CMAP_MAP * cmap_proc_ctx_global_env(CMAP_PROC_CTX * proc_ctx);

CMAP_MAP * cmap_proc_ctx_local_definitions(CMAP_PROC_CTX * proc_ctx);

void cmap_proc_ctx_local_refs_add(CMAP_PROC_CTX * proc_ctx, CMAP_LIFECYCLE * lc,
  char created);
void cmap_proc_ctx_local_refs_rm(CMAP_PROC_CTX * proc_ctx, CMAP_LIFECYCLE * lc);

CMAP_PROC_CTX * cmap_proc_ctx_new(CMAP_PROC_CTX * proc_ctx);

CMAP_MAP * cmap_proc_ctx_delete(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * ret);
CMAP_PROC_CTX * cmap_proc_ctx_create(CMAP_ENV * env);

#endif
