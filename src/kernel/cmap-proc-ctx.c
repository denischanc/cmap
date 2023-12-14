
#include "cmap-proc-ctx.h"

#include "cmap-kernel.h"
#include "cmap-env.h"
#include "cmap-lifecycle.h"
#include "cmap-stack-define.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_DEF(lifecycle, CMAP_LIFECYCLE *)

typedef struct
{
  CMAP_ENV * env;

  CMAP_STACK_lifecycle * local_stack;
  CMAP_MAP * definitions;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static CMAP_ENV * env(CMAP_PROC_CTX * this)
{
  return ((INTERNAL *)this -> internal) -> env;
}

static CMAP_PROTOTYPESTORE * prototypestore(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, prototypestore, this);
}

static CMAP_POOL_LIST * pool_list(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, pool_list, this);
}

static CMAP_POOL_STRING * pool_string(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, pool_string, this);
}

static CMAP_POOL_INT * pool_int(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, pool_int, this);
}

static CMAP_MAP * global_env(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL_ARGS(env_, global, this);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * local_definitions(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> definitions;
}

/*******************************************************************************
*******************************************************************************/

static void local_stack_add(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  cmap_stack_lifecycle_push(&internal -> local_stack, lc);
  CMAP_INC_REF(lc);
}

/*******************************************************************************
*******************************************************************************/

static int delete_local_stack(CMAP_STACK_lifecycle ** local_stack,
  CMAP_LIFECYCLE * ret)
{
  int nb_ret = 0;
  while(*local_stack != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_stack_lifecycle_pop(local_stack);
    if(lc == ret) nb_ret++;
    else CMAP_CALL(lc, dec_ref_or_deep_delete_last_ref);
  }
  return nb_ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * delete(CMAP_PROC_CTX * this, CMAP_MAP * ret)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_ENV * env = internal -> env;

  CMAP_LIFECYCLE * lc_ret = &ret -> super;
  int nb_ret = delete_local_stack(&internal -> local_stack, lc_ret);

  CMAP_CALL(env, pop_proc_ctx);

  CMAP_KERNEL_FREE(internal);
  CMAP_KERNEL_FREE(this);

  if(ret != NULL)
  {
    for(int i = 0; i < nb_ret; i++) CMAP_DEC_REF(ret);
    local_stack_add(CMAP_CALL(env, proc_ctx), lc_ret);
  }

  return ret;
}

static CMAP_PROC_CTX * create(CMAP_ENV * env_)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> env = env_;
  internal -> local_stack = NULL;

  CMAP_KERNEL_ALLOC_PTR(proc_ctx, CMAP_PROC_CTX);
  proc_ctx -> internal = internal;
  proc_ctx -> delete = delete;
  proc_ctx -> env = env;
  proc_ctx -> prototypestore = prototypestore;
  proc_ctx -> pool_list = pool_list;
  proc_ctx -> pool_string = pool_string;
  proc_ctx -> pool_int = pool_int;
  proc_ctx -> global_env = global_env;
  proc_ctx -> local_definitions = local_definitions;
  proc_ctx -> local_stack_add = local_stack_add;

  internal -> definitions = cmap_map_public.create_root(proc_ctx);

  CMAP_CALL_ARGS(env_, push_proc_ctx, proc_ctx);

  return proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public =
{
  create
};
