
#include "cmap-proc-ctx.h"

#include "cmap-mem.h"
#include "cmap-env.h"
#include "cmap-refsstore.h"
#include "cmap-log.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

struct CMAP_PROC_CTX
{
  CMAP_ENV * env;

  CMAP_REFSSTORE * refs;
  CMAP_MAP * definitions;

  int level;
  CMAP_PROC_CTX * prev;
};

/*******************************************************************************
*******************************************************************************/

CMAP_ENV * cmap_proc_ctx_env(CMAP_PROC_CTX * proc_ctx)
{
  return proc_ctx -> env;
}

CMAP_PROTOTYPESTORE * cmap_proc_ctx_prototypestore(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_env_prototypestore(cmap_proc_ctx_env(proc_ctx));
}

#define POOL_IMPL(NAME, name, type) \
CMAP_POOL_##NAME * cmap_proc_ctx_pool_##name(CMAP_PROC_CTX * proc_ctx) \
{ \
  return cmap_env_pool_##name(cmap_proc_ctx_env(proc_ctx)); \
}

CMAP_POOL_LOOP(POOL_IMPL)

CMAP_MAP * cmap_proc_ctx_global_env(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_env_global(cmap_proc_ctx_env(proc_ctx));
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_proc_ctx_local_definitions(CMAP_PROC_CTX * proc_ctx)
{
  if(proc_ctx -> definitions == NULL)
  {
    CMAP_POOL_MAP_GHOST * pool = cmap_proc_ctx_pool_map_ghost(proc_ctx);
    proc_ctx -> definitions = cmap_pool_map_ghost_take(pool, proc_ctx);
  }
  return proc_ctx -> definitions;
}

/*******************************************************************************
*******************************************************************************/

void cmap_proc_ctx_local_refs_add(CMAP_PROC_CTX * proc_ctx, CMAP_LIFECYCLE * lc,
  char created)
{
  cmap_refsstore_add(proc_ctx -> refs, lc, created);
}

void cmap_proc_ctx_local_refs_rm(CMAP_PROC_CTX * proc_ctx, CMAP_LIFECYCLE * lc)
{
  cmap_refsstore_rm(proc_ctx -> refs, lc);
}

/*******************************************************************************
*******************************************************************************/

CMAP_PROC_CTX * cmap_proc_ctx_new(CMAP_PROC_CTX * proc_ctx)
{
  return cmap_proc_ctx_create(cmap_proc_ctx_env(proc_ctx));
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_proc_ctx_delete(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * ret)
{
  cmap_log_debug("[%p][proc-ctx][%d] deletion", proc_ctx, proc_ctx -> level);

  if(proc_ctx -> definitions != NULL)
  {
    CMAP_POOL_MAP_GHOST * pool = cmap_proc_ctx_pool_map_ghost(proc_ctx);
    cmap_pool_map_ghost_release(pool, proc_ctx -> definitions);
  }

  cmap_refsstore_delete(proc_ctx -> refs, ret);

  cmap_env_set_proc_ctx(proc_ctx -> env, proc_ctx -> prev);
  cmap_mem_free(proc_ctx);

  if(ret != NULL) CMAP_CALL((CMAP_LIFECYCLE *)ret, store);

  return ret;
}

CMAP_PROC_CTX * cmap_proc_ctx_create(CMAP_ENV * env)
{
  CMAP_MEM_ALLOC_PTR(proc_ctx, CMAP_PROC_CTX);
  CMAP_PROC_CTX * prev = cmap_env_proc_ctx(env);

  proc_ctx -> env = env;
  proc_ctx -> refs = cmap_refsstore_create(cmap_env_refswatcher(env));
  proc_ctx -> definitions = NULL;
  proc_ctx -> level = (prev == NULL) ? 1 : 1 + prev -> level;
  proc_ctx -> prev = prev;

  cmap_env_set_proc_ctx(env, proc_ctx);

  cmap_log_debug("[%p][proc-ctx][%d] created", proc_ctx, proc_ctx -> level);

  return proc_ctx;
}
