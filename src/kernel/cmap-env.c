
#include "cmap-env.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-global-env.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

#define POOL_VAR(TYPE, type) CMAP_POOL_##TYPE * pool_##type;

typedef struct
{
  CMAP_SLIST_PROC_CTX * proc_ctx;

  CMAP_PROTOTYPESTORE * prototypestore;

  CMAP_POOL_LOOP(POOL_VAR)

  CMAP_MAP * global;

  CMAP_ENV * prev, * next;
} INTERNAL;

static CMAP_ENV * envs = NULL;

/*******************************************************************************
*******************************************************************************/

static void push_proc_ctx(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_CALL_ARGS(internal -> proc_ctx, push, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROC_CTX * proc_ctx(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_PROC_CTX ** proc_ctx = CMAP_CALL(internal -> proc_ctx, last);
  return (proc_ctx == NULL) ? NULL : *proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

static void pop_proc_ctx(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_CALL(internal -> proc_ctx, pop);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROTOTYPESTORE * prototypestore(CMAP_ENV * this,
  CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> prototypestore == NULL)
  {
    internal -> prototypestore = cmap_prototypestore_public.create(proc_ctx);
    CMAP_INC_REFS(internal -> prototypestore);
  }
  return internal -> prototypestore;
}

/*******************************************************************************
*******************************************************************************/

#define POOL_IMPL(TYPE, type) \
static CMAP_POOL_##TYPE * pool_##type(CMAP_ENV * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  if(internal -> pool_##type == NULL) \
  { \
    internal -> pool_##type = cmap_pool_##type##_public.create( \
      1 << 10, proc_ctx); \
    CMAP_INC_REFS(internal -> pool_##type); \
  } \
  return internal -> pool_##type; \
}

CMAP_POOL_LOOP(POOL_IMPL)

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * global(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> global == NULL)
  {
    internal -> global =
      cmap_global_env_public.create(proc_ctx, this -> argc, this -> argv);
    CMAP_INC_REFS(internal -> global);
  }
  return internal -> global;
}

/*******************************************************************************
*******************************************************************************/

#define POOL_DEC_REFS(TYPE, type) \
  if(internal -> pool_##type != NULL) CMAP_DEC_REFS(internal -> pool_##type);

#define POOL_SET(TYPE, type) internal -> pool_##type = NULL;

#define POOL_FN_SET(TYPE, type) this -> pool_##type = pool_##type;

static void delete(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_ENV * prev = internal -> prev, * next = internal -> next;
  if(next != NULL) ((INTERNAL *)(next + 1)) -> prev = prev;
  if(prev != NULL) ((INTERNAL *)(prev + 1)) -> next = next;
  else envs = next;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(this);

  if(internal -> prototypestore != NULL)
    CMAP_DEC_REFS(internal -> prototypestore);
  CMAP_POOL_LOOP(POOL_DEC_REFS)
  if(internal -> global != NULL) CMAP_DEC_REFS(internal -> global);

  CMAP_CALL_ARGS(proc_ctx, delete, NULL);

  CMAP_CALL(internal -> proc_ctx, delete);

  CMAP_KERNEL_FREE(this);
}

static CMAP_ENV * create(int argc, char ** argv)
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_ENV * this = (CMAP_ENV *)mem -> alloc(
    sizeof(CMAP_ENV) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> proc_ctx = cmap_slist_proc_ctx_public.create(0);
  internal -> prototypestore = NULL;
  CMAP_POOL_LOOP(POOL_SET)
  internal -> global = NULL;
  internal -> prev = NULL;
  internal -> next = envs;

  this -> argc = argc;
  this -> argv = argv;
  this -> delete = delete;
  this -> push_proc_ctx = push_proc_ctx;
  this -> proc_ctx = proc_ctx;
  this -> pop_proc_ctx = pop_proc_ctx;
  this -> prototypestore = prototypestore;
  CMAP_POOL_LOOP(POOL_FN_SET)
  this -> global = global;

  if(envs != NULL) ((INTERNAL *)(envs + 1)) -> prev = this;
  envs = this;

  return this;
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  while(envs != NULL) delete(envs);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_ENV_PUBLIC cmap_env_public = { create, delete_all };
