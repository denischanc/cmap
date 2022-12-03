
#include "cmap-env.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-global-env.h"
#include "cmap-proc-ctx.h"
#include "cmap-parser-util.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_AISLESTORE * aislestore;

  CMAP_POOL_LIST * pool_list;
  CMAP_POOL_STRING * pool_string;

  CMAP_MAP * global;

  CMAP_ENV * prev, * next;
} INTERNAL;

static CMAP_ENV * envs = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * main_(CMAP_ENV * this, int argc, char * argv[],
  CMAP_MAP * definitions, const char * impl)
{
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(this);
  CMAP_CALL(proc_ctx, push_local_stack);

  CMAP_MAP * ret =
    cmap_parser_util_public.proc_impl(proc_ctx, definitions, impl);

  CMAP_CALL(proc_ctx, pop_local_stack);
  CMAP_CALL(proc_ctx, delete);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_AISLESTORE * aislestore(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> aislestore == NULL)
  {
    internal -> aislestore = cmap_aislestore_public.create();
  }
  return internal -> aislestore;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_POOL_LIST * pool_list(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> pool_list == NULL)
  {
    internal -> pool_list = cmap_pool_list_public.create(20, proc_ctx);
  }
  return internal -> pool_list;
}

static CMAP_POOL_STRING * pool_string(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> pool_string == NULL)
  {
    internal -> pool_string = cmap_pool_string_public.create(20, proc_ctx);
  }
  return internal -> pool_string;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * global(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> global == NULL)
  {
    internal -> global = cmap_global_env_public.create(proc_ctx);
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

  if(internal -> aislestore != NULL) CMAP_DELETE(internal -> aislestore);
  if(internal -> pool_list != NULL) CMAP_CALL(internal -> pool_list, delete);
  if(internal -> pool_string != NULL)
    CMAP_CALL(internal -> pool_string, delete);

  CMAP_KERNEL_FREE(internal);
  CMAP_KERNEL_FREE(this);
}

static CMAP_ENV * create()
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> aislestore = NULL;
  internal -> pool_list = NULL;
  internal -> pool_string = NULL;
  internal -> global = NULL;
  internal -> prev = NULL;
  internal -> next = envs;

  CMAP_KERNEL_ALLOC_PTR(env, CMAP_ENV);
  env -> internal = internal;
  env -> delete = delete;
  env -> main = main_;
  env -> aislestore = aislestore;
  env -> pool_list = pool_list;
  env -> pool_string = pool_string;
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
