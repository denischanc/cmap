
#include "cmap-proc-ctx.h"

#include "cmap-kernel.h"
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
  return CMAP_CALL_ARGS(env_, prototypestore, this);
}

#define POOL_IMPL(TYPE, type) \
static CMAP_POOL_##TYPE * pool_##type(CMAP_PROC_CTX * this) \
{ \
  CMAP_ENV * env_ = env(this); \
  return CMAP_CALL_ARGS(env_, pool_##type, this); \
}

CMAP_POOL_LOOP(POOL_IMPL)

static CMAP_MAP * global_env(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, global, this);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * local_definitions(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> definitions == NULL)
    internal -> definitions = cmap_map_public.create_root(this);
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

/*******************************************************************************
*******************************************************************************/

#define POOL_FN_SET(TYPE, type) this -> pool_##type = pool_##type;

static CMAP_MAP * delete(CMAP_PROC_CTX * this, CMAP_MAP * ret)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  cmap_log_public.debug("[%p][proc-ctx][%d] deletion", this, internal -> level);

  CMAP_CALL_ARGS(internal -> refs, delete, ret);

  CMAP_CALL(internal -> env, pop_proc_ctx);
  CMAP_PROC_CTX * proc_ctx_cur = CMAP_CALL(internal -> env, proc_ctx);

  CMAP_KERNEL_FREE(this);

  if(ret != NULL) local_refs_add(proc_ctx_cur, (CMAP_LIFECYCLE *)ret, CMAP_F);

  return ret;
}

static CMAP_PROC_CTX * create(CMAP_ENV * env_)
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_PROC_CTX * this = (CMAP_PROC_CTX *)mem -> alloc(
    sizeof(CMAP_PROC_CTX) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> env = env_;
  internal -> refs = cmap_refsstore_public.create();
  internal -> definitions = NULL;
  internal -> level = 1;

  CMAP_PROC_CTX * proc_ctx_parent = CMAP_CALL(env_, proc_ctx);
  if(proc_ctx_parent != NULL)
    internal -> level += ((INTERNAL *)(proc_ctx_parent + 1)) -> level;

  this -> delete = delete;
  this -> env = env;
  this -> prototypestore = prototypestore;
  CMAP_POOL_LOOP(POOL_FN_SET)
  this -> global_env = global_env;
  this -> local_definitions = local_definitions;
  this -> local_refs_add = local_refs_add;

  CMAP_CALL_ARGS(env_, push_proc_ctx, this);

  cmap_log_public.debug("[%p][proc-ctx][%d] created", this, internal -> level);

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public = { create };
