
#include "cmap-env.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-global-env.h"
#include "cmap-proc-ctx.h"
#include "cmap-util.h"
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_DEF(proc_ctx, CMAP_PROC_CTX *)

typedef struct
{
  CMAP_STACK_proc_ctx * proc_ctx;

  CMAP_PROTOTYPESTORE * prototypestore;

  CMAP_POOL_LIST * pool_list;
  CMAP_POOL_STRING * pool_string;
  CMAP_POOL_INT * pool_int;

  CMAP_MAP * global;

  CMAP_ENV * prev, * next;
} INTERNAL;

static CMAP_ENV * envs = NULL;

/*******************************************************************************
*******************************************************************************/

static void push_proc_ctx(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  cmap_stack_proc_ctx_push(&internal -> proc_ctx, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROC_CTX * proc_ctx(CMAP_ENV * this)
{
  return ((INTERNAL *)this -> internal) -> proc_ctx -> v;
}

/*******************************************************************************
*******************************************************************************/

static void pop_proc_ctx(CMAP_ENV * this)
{
  cmap_stack_proc_ctx_pop(&((INTERNAL *)this -> internal) -> proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROTOTYPESTORE * prototypestore(CMAP_ENV * this,
  CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> prototypestore == NULL)
  {
    internal -> prototypestore = cmap_prototypestore_public.create(proc_ctx);
    CMAP_INC_REF(internal -> prototypestore);
  }
  return internal -> prototypestore;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_POOL_LIST * pool_list(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> pool_list == NULL)
  {
    internal -> pool_list = cmap_pool_list_public.create(20, proc_ctx);
    CMAP_INC_REF(internal -> pool_list);
  }
  return internal -> pool_list;
}

static CMAP_POOL_STRING * pool_string(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> pool_string == NULL)
  {
    internal -> pool_string = cmap_pool_string_public.create(20, proc_ctx);
    CMAP_INC_REF(internal -> pool_string);
  }
  return internal -> pool_string;
}

static CMAP_POOL_INT * pool_int(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> pool_int == NULL)
  {
    internal -> pool_int = cmap_pool_int_public.create(20, proc_ctx);
    CMAP_INC_REF(internal -> pool_int);
  }
  return internal -> pool_int;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * global(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> global == NULL)
  {
    internal -> global =
      cmap_global_env_public.create(proc_ctx, this -> argc, this -> argv);
    CMAP_INC_REF(internal -> global);
  }
  return internal -> global;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  CMAP_ENV * prev = internal -> prev, * next = internal -> next;
  if(next != NULL) ((INTERNAL *)next -> internal) -> prev = prev;
  if(prev != NULL) ((INTERNAL *)prev -> internal) -> next = next;
  else envs = next;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(this);

  if(internal -> prototypestore != NULL)
    CMAP_DEC_REF(internal -> prototypestore);
  if(internal -> pool_list != NULL) CMAP_DEC_REF(internal -> pool_list);
  if(internal -> pool_string != NULL) CMAP_DEC_REF(internal -> pool_string);
  if(internal -> pool_int != NULL) CMAP_DEC_REF(internal -> pool_int);
  if(internal -> global != NULL) CMAP_DEC_REF(internal -> global);

  CMAP_CALL(proc_ctx, delete);

  CMAP_KERNEL_FREE(internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_ENV * create(int argc, char ** argv)
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> proc_ctx = NULL;
  internal -> prototypestore = NULL;
  internal -> pool_list = NULL;
  internal -> pool_string = NULL;
  internal -> pool_int = NULL;
  internal -> global = NULL;
  internal -> prev = NULL;
  internal -> next = envs;

  CMAP_KERNEL_ALLOC_PTR(env, CMAP_ENV);
  env -> internal = internal;
  env -> argc = argc;
  env -> argv = argv;
  env -> delete = delete;
  env -> push_proc_ctx = push_proc_ctx;
  env -> proc_ctx = proc_ctx;
  env -> pop_proc_ctx = pop_proc_ctx;
  env -> prototypestore = prototypestore;
  env -> pool_list = pool_list;
  env -> pool_string = pool_string;
  env -> pool_int = pool_int;
  env -> global = global;

  if(envs != NULL) ((INTERNAL *)envs -> internal) -> prev = env;
  envs = env;

  return env;
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  while(envs != NULL) delete(envs);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_ENV_PUBLIC cmap_env_public =
{
  create,
  delete_all
};
