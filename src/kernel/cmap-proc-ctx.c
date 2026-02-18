
#include "cmap-proc-ctx.h"

#include "cmap-mem.h"
#include "cmap-env.h"
#include "cmap-refsstore.h"
#include "cmap-log.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_ENV * env;

  CMAP_REFSSTORE * refs;
  CMAP_MAP * definitions;

  int level;
  CMAP_PROC_CTX * prev;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static CMAP_ENV * env(CMAP_PROC_CTX * this)
{
  return ((INTERNAL *)(this + 1)) -> env;
}

static CMAP_PROTOTYPESTORE * prototypestore(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL(env_, prototypestore);
}

#define POOL_IMPL(NAME, name, type) \
static CMAP_POOL_##NAME * pool_##name(CMAP_PROC_CTX * this) \
{ \
  CMAP_ENV * env_ = env(this); \
  return CMAP_CALL(env_, pool_##name); \
}

CMAP_POOL_LOOP(POOL_IMPL)

static CMAP_MAP * global_env(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL(env_, global);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * local_definitions(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> definitions == NULL)
  {
    CMAP_POOL_MAP_GHOST * pool = pool_map_ghost(this);
    internal -> definitions = CMAP_CALL_ARGS(pool, take, this);
  }
  return internal -> definitions;
}

/*******************************************************************************
*******************************************************************************/

static void local_refs_add(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc,
  char created)
{
  CMAP_REFSSTORE * refs = ((INTERNAL *)(this + 1)) -> refs;
  CMAP_CALL_ARGS(refs, add, lc, created);
}

static void local_refs_rm(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc)
{
  CMAP_REFSSTORE * refs = ((INTERNAL *)(this + 1)) -> refs;
  CMAP_CALL_ARGS(refs, rm, lc);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROC_CTX * this_create(CMAP_PROC_CTX * this)
{
  return cmap_proc_ctx_create(env(this));
}

/*******************************************************************************
*******************************************************************************/

#define POOL_FN_SET(NAME, name, type) this -> pool_##name = pool_##name;

static CMAP_MAP * delete(CMAP_PROC_CTX * this, CMAP_MAP * ret)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  cmap_log_debug("[%p][proc-ctx][%d] deletion", this, internal -> level);

  if(internal -> definitions != NULL)
  {
    CMAP_POOL_MAP_GHOST * pool = pool_map_ghost(this);
    CMAP_CALL_ARGS(pool, release, internal -> definitions);
  }

  CMAP_CALL_ARGS(internal -> refs, delete, ret);

  CMAP_CALL_ARGS(internal -> env, set_proc_ctx, internal -> prev);
  cmap_mem_free(this);

  if(ret != NULL) CMAP_CALL((CMAP_LIFECYCLE *)ret, store);

  return ret;
}

CMAP_PROC_CTX * cmap_proc_ctx_create(CMAP_ENV * env_)
{
  CMAP_PROC_CTX * this = cmap_mem_alloc(
    sizeof(CMAP_PROC_CTX) + sizeof(INTERNAL)),
    * prev = CMAP_CALL(env_, proc_ctx);

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> env = env_;
  internal -> refs =
    cmap_refsstore_public.create(CMAP_CALL(env_, refswatcher));
  internal -> definitions = NULL;
  internal -> level =
    (prev == NULL) ? 1 : 1 + ((INTERNAL *)(prev + 1)) -> level;
  internal -> prev = prev;

  this -> delete = delete;
  this -> env = env;
  this -> prototypestore = prototypestore;
  CMAP_POOL_LOOP(POOL_FN_SET)
  this -> global_env = global_env;
  this -> local_definitions = local_definitions;
  this -> local_refs_add = local_refs_add;
  this -> local_refs_rm = local_refs_rm;
  this -> create = this_create;

  CMAP_CALL_ARGS(env_, set_proc_ctx, this);

  cmap_log_debug("[%p][proc-ctx][%d] created", this, internal -> level);

  return this;
}
