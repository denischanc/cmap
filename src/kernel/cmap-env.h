#ifndef __CMAP_ENV_H__
#define __CMAP_ENV_H__

#include "cmap-env-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-pool.h"
#include "cmap-prototypestore.h"
#include "cmap-refswatcher.h"

#define CMAP_ENV_POOL_DECL(NAME, name, type) \
  CMAP_POOL_##NAME * cmap_env_pool_##name(CMAP_ENV * env);

void cmap_env_set_main(CMAP_ENV * env, CMAP_ENV_MAIN main_);

void cmap_env_nb_jobs_add(CMAP_ENV * env, int nb);

void cmap_env_set_proc_ctx(CMAP_ENV * env, CMAP_PROC_CTX * proc_ctx);
CMAP_PROC_CTX * cmap_env_proc_ctx(CMAP_ENV * env);

CMAP_PROTOTYPESTORE * cmap_env_prototypestore(CMAP_ENV * env);

CMAP_POOL_LOOP(CMAP_ENV_POOL_DECL)

CMAP_MAP * cmap_env_global(CMAP_ENV * env);

CMAP_REFSWATCHER * cmap_env_refswatcher(CMAP_ENV * env);

void cmap_env_delete(CMAP_ENV * env);
CMAP_ENV * cmap_env_create();

void cmap_env_delete_all();

#endif
