
#include "cmap-env.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-mem.h"
#include "cmap-global-env.h"
#include "cmap-proc-ctx.h"
#include "cmap-util.h"
#include "cmap-scheduler.h"
#include "cmap-loop-timer.h"
#include "cmap-module.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

#define POOL_VAR(NAME, name, type) CMAP_POOL_##NAME * pool_##name;

struct CMAP_ENV
{
  CMAP_ENV_MAIN main;

  int nb_jobs, nb_daemons;
  char state;

  CMAP_PROC_CTX * proc_ctx;

  CMAP_PROTOTYPESTORE * prototypestore;

  CMAP_POOL_LOOP(POOL_VAR)

  CMAP_MAP * global;

  CMAP_REFSWATCHER * refswatcher;

  CMAP_LOOP_TIMER timer;

  CMAP_ENV * prev, * next;
};

static CMAP_ENV * envs = NULL;

/*******************************************************************************
*******************************************************************************/

void cmap_env_set_main(CMAP_ENV * env, CMAP_ENV_MAIN main_)
{
  env -> main = main_;
}

/*******************************************************************************
*******************************************************************************/

static void delete_cb(CMAP_LOOP_TIMER * timer)
{
  cmap_env_delete(timer -> data);
}

static void check_jobs_n_daemons(CMAP_ENV * env)
{
  if((env -> nb_jobs <= 0) && (env -> nb_daemons <= 0))
  {
    CMAP_REFSWATCHER * refswatcher = env -> refswatcher;
    if(refswatcher != NULL) cmap_refswatcher_stop(refswatcher);

    CMAP_LOOP_TIMER * timer = &env -> timer;
    timer -> data = env;
    cmap_loop_timer_start(timer, delete_cb, 0, 0, NULL);
  }
}

void cmap_env_nb_jobs_add(CMAP_ENV * env, int nb)
{
  env -> nb_jobs += nb;
  if(env -> nb_jobs <= 0) env -> state = CMAP_ENV_S_CLOSING;

  check_jobs_n_daemons(env);
}

void cmap_env_nb_daemons_add(CMAP_ENV * env, int nb)
{
  env -> nb_daemons += nb;

  check_jobs_n_daemons(env);
}

char cmap_env_state(CMAP_ENV * env)
{
  return env -> state;
}

/*******************************************************************************
*******************************************************************************/

void cmap_env_set_proc_ctx(CMAP_ENV * env, CMAP_PROC_CTX * proc_ctx)
{
  env -> proc_ctx = proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

CMAP_PROC_CTX * cmap_env_proc_ctx(CMAP_ENV * env)
{
  return env -> proc_ctx;
}

/*******************************************************************************
*******************************************************************************/

CMAP_PROTOTYPESTORE * cmap_env_prototypestore(CMAP_ENV * env)
{
  if(env -> prototypestore == NULL)
    env -> prototypestore = cmap_prototypestore_create(env -> proc_ctx);
  return env -> prototypestore;
}

/*******************************************************************************
*******************************************************************************/

#define POOL_IMPL(NAME, name, type) \
CMAP_POOL_##NAME * cmap_env_pool_##name(CMAP_ENV * env) \
{ \
  if(env -> pool_##name == NULL) \
    env -> pool_##name = cmap_pool_##name##_create(0, env -> proc_ctx); \
  return env -> pool_##name; \
}

CMAP_POOL_LOOP(POOL_IMPL)

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_env_global(CMAP_ENV * env)
{
  if(env -> global == NULL)
  {
    env -> global = cmap_global_env_create(env -> proc_ctx);
    CMAP_INC_REFS(env -> global);
  }
  return env -> global;
}

/*******************************************************************************
*******************************************************************************/

CMAP_REFSWATCHER * cmap_env_refswatcher(CMAP_ENV * env)
{
  if(env -> refswatcher == NULL)
    env -> refswatcher = cmap_refswatcher_create(env);
  return env -> refswatcher;
}

/*******************************************************************************
*******************************************************************************/

static void bootstrap(CMAP_LOOP_TIMER * timer)
{
  CMAP_ENV * env = timer -> data;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(env);

  cmap_module_load_from_config(proc_ctx);

  CMAP_ENV_MAIN main_ = env -> main;
  if(main_ != NULL) main_(proc_ctx);

  cmap_proc_ctx_delete(proc_ctx, NULL);

  cmap_env_nb_jobs_add(env, 0);
}

static inline void loop_init(CMAP_ENV * env)
{
  CMAP_LOOP_TIMER * timer = &env -> timer;
  timer -> data = env;
  cmap_loop_timer_start(timer, bootstrap, 0, 0, NULL);
}

/*******************************************************************************
*******************************************************************************/

#define POOL_DELETE(NAME, name, type) \
  if(env -> pool_##name != NULL) cmap_pool_##name##_delete(env -> pool_##name);

#define POOL_SET(NAME, name, type) env -> pool_##name = NULL;

void cmap_env_delete(CMAP_ENV * env)
{
  CMAP_ENV * prev = env -> prev, * next = env -> next;
  if(next != NULL) next -> prev = prev;
  if(prev != NULL) prev -> next = next;
  else envs = next;

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx_create(env);

  if(env -> prototypestore != NULL)
    cmap_prototypestore_delete(env -> prototypestore);
  CMAP_POOL_LOOP(POOL_DELETE)
  if(env -> global != NULL) CMAP_DEC_REFS(env -> global);

  cmap_proc_ctx_delete(proc_ctx, NULL);

  if(env -> refswatcher != NULL) cmap_refswatcher_delete(env -> refswatcher);

  cmap_mem_free(env);
}

CMAP_ENV * cmap_env_create()
{
  CMAP_MEM_ALLOC_PTR(env, CMAP_ENV);
  env -> main = NULL;
  env -> nb_jobs = 0;
  env -> nb_daemons = 0;
  env -> state = CMAP_ENV_S_ALIVE;
  env -> proc_ctx = NULL;
  env -> prototypestore = NULL;
  CMAP_POOL_LOOP(POOL_SET)
  env -> global = NULL;
  env -> refswatcher = NULL;
  env -> prev = NULL;
  env -> next = envs;

  loop_init(env);

  if(envs != NULL) envs -> prev = env;
  envs = env;

  return env;
}

/*******************************************************************************
*******************************************************************************/

void cmap_env_delete_all()
{
  while(envs != NULL) cmap_env_delete(envs);
}
