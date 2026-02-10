
#include "cmap-env.h"

#include <stdlib.h>
#include "cmap-mem.h"
#include "cmap-global-env.h"
#include "cmap-proc-ctx.h"
#include "cmap-util.h"
#include "cmap-scheduler.h"
#include "cmap-uv.h"

/*******************************************************************************
*******************************************************************************/

#define POOL_VAR(NAME, name, type) CMAP_POOL_##NAME * pool_##name;

typedef struct
{
  CMAP_ENV_MAIN main;

  CMAP_PROC_CTX * proc_ctx;

  CMAP_PROTOTYPESTORE * prototypestore;

  CMAP_POOL_LOOP(POOL_VAR)

  CMAP_MAP * global;

  CMAP_REFSWATCHER * refswatcher;

  uv_idle_t idle;

  CMAP_ENV * prev, * next;
} INTERNAL;

static CMAP_ENV * envs = NULL;

/*******************************************************************************
*******************************************************************************/

static void set_main(CMAP_ENV * this, CMAP_ENV_MAIN main_)
{
  ((INTERNAL *)(this + 1)) -> main = main_;
}

/*******************************************************************************
*******************************************************************************/

static void set_proc_ctx(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx)
{
  ((INTERNAL *)(this + 1)) -> proc_ctx = proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROC_CTX * proc_ctx(CMAP_ENV * this)
{
  return ((INTERNAL *)(this + 1)) -> proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PROTOTYPESTORE * prototypestore(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> prototypestore == NULL)
  {
    internal -> prototypestore =
      cmap_prototypestore_public.create(internal -> proc_ctx);
    CMAP_INC_REFS(internal -> prototypestore);
  }
  return internal -> prototypestore;
}

/*******************************************************************************
*******************************************************************************/

#define POOL_IMPL(NAME, name, type) \
static CMAP_POOL_##NAME * pool_##name(CMAP_ENV * this) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  if(internal -> pool_##name == NULL) \
  { \
    internal -> pool_##name = \
      cmap_pool_##name##_public.create(0, internal -> proc_ctx); \
    CMAP_INC_REFS(internal -> pool_##name); \
  } \
  return internal -> pool_##name; \
}

CMAP_POOL_LOOP(POOL_IMPL)

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * global(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> global == NULL)
  {
    internal -> global = cmap_global_env_public.create(internal -> proc_ctx);
    CMAP_INC_REFS(internal -> global);
  }
  return internal -> global;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_REFSWATCHER * refswatcher(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> refswatcher == NULL)
    internal -> refswatcher = cmap_refswatcher_public.create(this);
  return internal -> refswatcher;
}

/*******************************************************************************
*******************************************************************************/

static void bootstrap(uv_timer_t * timer)
{
  CMAP_ENV * env = timer -> data;
  INTERNAL * internal = (INTERNAL *)(env + 1);
  CMAP_ENV_MAIN main_ = internal -> main;
  if(main_ != NULL)
  {
    CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_public.create(env);
    main_(proc_ctx);
    CMAP_CALL_ARGS(proc_ctx, delete, NULL);
  }

  internal -> idle.data = env;
  cmap_uv_public.idle_start(&internal -> idle, cmap_scheduler_public.schedule);

  cmap_uv_public.timer_stop(timer, CMAP_T);
}

static inline void this_uv_init(CMAP_ENV * this)
{
  CMAP_MEM_INSTANCE_ALLOC_PTR(timer, uv_timer_t);
  timer -> data = this;
  cmap_uv_public.timer_start(timer, bootstrap, 0, 0);
}

static void scheduler_empty(CMAP_ENV * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  cmap_uv_public.idle_stop(&internal -> idle, CMAP_F);

  CMAP_REFSWATCHER * refswatcher = internal -> refswatcher;
  if(refswatcher != NULL) CMAP_CALL(refswatcher, stop);
}

/*******************************************************************************
*******************************************************************************/

#define POOL_DEC_REFS(NAME, name, type) \
  if(internal -> pool_##name != NULL) CMAP_DEC_REFS(internal -> pool_##name);

#define POOL_SET(NAME, name, type) internal -> pool_##name = NULL;

#define POOL_FN_SET(NAME, name, type) this -> pool_##name = pool_##name;

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

  if(internal -> refswatcher != NULL) CMAP_DELETE(internal -> refswatcher);

  CMAP_MEM_INSTANCE_FREE(this);
}

static CMAP_ENV * create()
{
  CMAP_ENV * this = (CMAP_ENV *)CMAP_MEM_INSTANCE -> alloc(
    sizeof(CMAP_ENV) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> main = NULL;
  internal -> proc_ctx = NULL;
  internal -> prototypestore = NULL;
  CMAP_POOL_LOOP(POOL_SET)
  internal -> global = NULL;
  internal -> refswatcher = NULL;
  internal -> prev = NULL;
  internal -> next = envs;

  this -> delete = delete;
  this -> set_main = set_main;
  this -> set_proc_ctx = set_proc_ctx;
  this -> proc_ctx = proc_ctx;
  this -> prototypestore = prototypestore;
  CMAP_POOL_LOOP(POOL_FN_SET)
  this -> global = global;
  this -> refswatcher = refswatcher;
  this -> scheduler_empty = scheduler_empty;

  this_uv_init(this);

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

const CMAP_ENV_PUBLIC cmap_env_public = {create, delete_all};
