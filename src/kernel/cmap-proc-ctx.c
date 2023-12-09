
#include "cmap-proc-ctx.h"

#include "cmap.h"
#include "cmap-util.h"
#include "cmap-list.h"
#include "cmap-kernel.h"
#include "cmap-env.h"
#include "cmap-ptr.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

static const char * AISLE_LOCAL_STACK = "cmap-internal-local-stack";
static const char * AISLE_DEFINITIONS = "cmap-internal-definitions";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_ENV * env;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static CMAP_ENV * env(CMAP_PROC_CTX * this)
{
  return ((INTERNAL *)this -> internal) -> env;
}

static CMAP_AISLESTORE * aislestore(CMAP_PROC_CTX * this)
{
  CMAP_ENV * env_ = env(this);
  return CMAP_CALL(env_, aislestore);
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

static CMAP_LIFECYCLE * delete_local_stack(CMAP_LIFECYCLE * lc)
{
  CMAP_LIST * local_stack = (CMAP_LIST *)lc;
  cmap_util_public.delete_list_vals(local_stack);
  return cmap_list_public.delete(local_stack);
}

static void push_local(CMAP_PROC_CTX * this)
{
  cmap_map_public.create_root(this, AISLE_DEFINITIONS);

  CMAP_LIST * local_stack = CMAP_LIST(0, this, AISLE_LOCAL_STACK);
  local_stack -> super.super.delete = delete_local_stack;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * local_definitions(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  return CMAP_GET(as, AISLE_DEFINITIONS);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIST * local_stack(CMAP_PROC_CTX * this)
{
  CMAP_AISLESTORE * as = aislestore(this);
  return (CMAP_LIST *)CMAP_GET(as, AISLE_LOCAL_STACK);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * pop_local(CMAP_PROC_CTX * this, CMAP_MAP * ret)
{
  CMAP_AISLESTORE * as = aislestore(this);

  CMAP_CALL_ARGS(as, delete_last, AISLE_DEFINITIONS);

  char ret_in_stack = CMAP_F;
  if(ret != NULL)
  {
    CMAP_LIST * old_stack = (CMAP_LIST *)CMAP_GET(as, AISLE_LOCAL_STACK);
    int i = cmap_util_public.is_val(old_stack, ret);
    if(i >= 0)
    {
      ret_in_stack = CMAP_T;
      CMAP_LIST_RM(old_stack, i);
    }
  }

  CMAP_LIST * new_stack =
    (CMAP_LIST *)CMAP_CALL_ARGS(as, delete_last, AISLE_LOCAL_STACK);

  if(ret_in_stack) CMAP_LIST_PUSH(new_stack, ret);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PROC_CTX * this)
{
  CMAP_KERNEL_FREE(this -> internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_PROC_CTX * create(CMAP_ENV * env_)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> env = env_;

  CMAP_KERNEL_ALLOC_PTR(proc_ctx, CMAP_PROC_CTX);
  proc_ctx -> internal = internal;
  proc_ctx -> delete = delete;
  proc_ctx -> env = env;
  proc_ctx -> aislestore = aislestore;
  proc_ctx -> prototypestore = prototypestore;
  proc_ctx -> pool_list = pool_list;
  proc_ctx -> pool_string = pool_string;
  proc_ctx -> pool_int = pool_int;
  proc_ctx -> global_env = global_env;
  proc_ctx -> push_local = push_local;
  proc_ctx -> local_definitions = local_definitions;
  proc_ctx -> local_stack = local_stack;
  proc_ctx -> pop_local = pop_local;

  return proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public =
{
  create
};
