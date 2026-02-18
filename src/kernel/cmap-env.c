
#include "cmap-env.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-global-env.h"
#include "cmap-proc-ctx.h"
#include "cmap-util.h"
#include "cmap-scheduler.h"
#include "cmap-loop-timer.h"

/*******************************************************************************
*******************************************************************************/

#define POOL_VAR(NAME, name, type) CMAP_POOL_##NAME * pool_##name;

typedef struct
{
  CMAP_ENV_MAIN main;

  int nb_jobs;

  CMAP_PROC_CTX * proc_ctx;

  CMAP_PROTOTYPESTORE * prototypestore;

  CMAP_POOL_LOOP(POOL_VAR)

  CMAP_MAP * global;

  CMAP_REFSWATCHER * refswatcher;

  CMAP_LOOP_TIMER timer;

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

static void delete_cb(CMAP_LOOP_TIMER * timer)
{
  CMAP_ENV * env = timer -> data;
  CMAP_DELETE(env);
}

static void nb_jobs_add(CMAP_ENV * this, int nb)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);

  int * nb_jobs = &internal -> nb_jobs;
  *nb_jobs += nb;

  if(*nb_jobs <= 0)
  {
    CMAP_REFSWATCHER * refswatcher = internal -> refswatcher;
    if(refswatcher != NULL) CMAP_CALL(refswatcher, stop);

    CMAP_LOOP_TIMER * timer = &internal -> timer;
    timer -> data = this;
    cmap_loop_timer_start(timer, delete_cb, 0, 0);
  }
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
      cmap_prototypestore_create(internal -> proc_ctx);
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
    internal -> refswatcher = cmap_refswatcher_create(this);
  return internal -> refswatcher;
}

/*******************************************************************************
*******************************************************************************/

static void bootstrap(CMAP_LOOP_TIMER * timer)
{
  CMAP_ENV * env = timer -> data;
  INTERNAL * internal = (INTERNAL *)(env + 1);
  CMAP_ENV_MAIN main_ = internal -> main;
  if(main_ != NULL)
  {
    CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(env);
    main_(proc_ctx);
    CMAP_CALL_ARGS(proc_ctx, delete, NULL);
  }

  nb_jobs_add(env, 0);
}

static inline void loop_init(CMAP_ENV * this)
{
  CMAP_LOOP_TIMER * timer = &((INTERNAL *)(this + 1)) -> timer;
  timer -> data = this;
  cmap_loop_timer_start(timer, bootstrap, 0, 0);
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

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(this);

  if(internal -> prototypestore != NULL)
    CMAP_DEC_REFS(internal -> prototypestore);
  CMAP_POOL_LOOP(POOL_DEC_REFS)
  if(internal -> global != NULL) CMAP_DEC_REFS(internal -> global);

  CMAP_CALL_ARGS(proc_ctx, delete, NULL);

  if(internal -> refswatcher != NULL) CMAP_DELETE(internal -> refswatcher);

  CMAP_MEM_INSTANCE_FREE(this);
}

CMAP_ENV * cmap_env_create()
{
  CMAP_ENV * this = (CMAP_ENV *)CMAP_MEM_INSTANCE -> alloc(
    sizeof(CMAP_ENV) + sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> main = NULL;
  internal -> nb_jobs = 0;
  internal -> proc_ctx = NULL;
  internal -> prototypestore = NULL;
  CMAP_POOL_LOOP(POOL_SET)
  internal -> global = NULL;
  internal -> refswatcher = NULL;
  internal -> prev = NULL;
  internal -> next = envs;

  this -> delete = delete;
  this -> set_main = set_main;
  this -> nb_jobs_add = nb_jobs_add;
  this -> set_proc_ctx = set_proc_ctx;
  this -> proc_ctx = proc_ctx;
  this -> prototypestore = prototypestore;
  CMAP_POOL_LOOP(POOL_FN_SET)
  this -> global = global;
  this -> refswatcher = refswatcher;

  loop_init(this);

  if(envs != NULL) ((INTERNAL *)(envs + 1)) -> prev = this;
  envs = this;

  return this;
}

/*******************************************************************************
*******************************************************************************/

void cmap_env_delete_all()
{
  while(envs != NULL) delete(envs);
}
