
#include "cmap-proc-ctx.h"

#include "cmap-list.h"
#include "cmap-kernel.h"
#include "cmap-env.h"
#include "cmap-lifecycle.h"
#include "cmap-stack-define.h"
#include "cmap-set-define.h"

/*******************************************************************************
*******************************************************************************/

static int CMAP_TREE_EVALFN_NAME(lifecycle)(CMAP_TREE_RUNNER * this,
  void * node, void * data);

CMAP_SET_DEF(lifecycle, CMAP_LIFECYCLE *)
CMAP_STACK_DEF(set_lifecycle, CMAP_SET_lifecycle *)

CMAP_STACK_DEF(map, CMAP_MAP *)

typedef struct
{
  CMAP_ENV * env;

  CMAP_STACK_set_lifecycle * local_stack;
  CMAP_STACK_map * definitions;
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

static int CMAP_TREE_EVALFN_NAME(lifecycle)(CMAP_TREE_RUNNER * this,
  void * node, void * data)
{
  CMAP_LIFECYCLE * lc_l = ((CMAP_SET_lifecycle *)node) -> v,
    * lc_r = ((CMAP_SET_lifecycle *)data) -> v;

  if(lc_l > lc_r) return 1;
  else if(lc_l < lc_r) return -1;
  else return 0;
}

/*******************************************************************************
*******************************************************************************/

static void push_local(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  cmap_stack_set_lifecycle_push(&internal -> local_stack, NULL);

  cmap_stack_map_push(&internal -> definitions,
    cmap_map_public.create_root(this));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * local_definitions(CMAP_PROC_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_STACK_map * definitions = internal -> definitions;
  return definitions -> v;
}

/*******************************************************************************
*******************************************************************************/

static void local_stack_add(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_STACK_set_lifecycle * local_stack = internal -> local_stack;
  if(cmap_set_lifecycle_add(&local_stack -> v, lc)) CMAP_INC_REF(lc);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * pop_local(CMAP_PROC_CTX * this, CMAP_MAP * ret)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;

  cmap_stack_map_pop(&internal -> definitions);

  CMAP_SET_lifecycle ** local_stack = &internal -> local_stack -> v;
  char fnd_ret = CMAP_F;
  while(*local_stack != NULL)
  {
    CMAP_LIFECYCLE * lc = cmap_set_lifecycle_rm(local_stack);
    if(lc == (CMAP_LIFECYCLE *)ret) fnd_ret = CMAP_T;
    else CMAP_CALL(lc, dec_ref_or_deep_delete_last_ref);
  }

  cmap_stack_set_lifecycle_pop(&internal -> local_stack);

  if(ret != NULL)
  {
    if(fnd_ret) CMAP_DEC_REF(ret);
    local_stack_add(this, (CMAP_LIFECYCLE *)ret);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PROC_CTX * this)
{
  pop_local(this, NULL);

  CMAP_ENV * env_ = env(this);
  CMAP_CALL(env_, pop_proc_ctx);

  CMAP_KERNEL_FREE(this -> internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_PROC_CTX * create(CMAP_ENV * env_)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> env = env_;
  internal -> local_stack = NULL;
  internal -> definitions = NULL;

  CMAP_KERNEL_ALLOC_PTR(proc_ctx, CMAP_PROC_CTX);
  proc_ctx -> internal = internal;
  proc_ctx -> delete = delete;
  proc_ctx -> env = env;
  proc_ctx -> prototypestore = prototypestore;
  proc_ctx -> pool_list = pool_list;
  proc_ctx -> pool_string = pool_string;
  proc_ctx -> pool_int = pool_int;
  proc_ctx -> global_env = global_env;
  proc_ctx -> push_local = push_local;
  proc_ctx -> local_definitions = local_definitions;
  proc_ctx -> local_stack_add = local_stack_add;
  proc_ctx -> pop_local = pop_local;

  CMAP_CALL_ARGS(env_, push_proc_ctx, proc_ctx);

  push_local(proc_ctx);

  return proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public =
{
  create
};
