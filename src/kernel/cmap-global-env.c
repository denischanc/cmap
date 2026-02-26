
#include "cmap-global-env.h"

#include "cmap-map.h"
#include "cmap-cmap.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * global_env = cmap_map_public.create_root(proc_ctx);
  CMAP_SET(global_env, "cmap", cmap_cmap_create(proc_ctx));
  return global_env;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_GLOBAL_ENV_PUBLIC cmap_global_env_public = { create };
