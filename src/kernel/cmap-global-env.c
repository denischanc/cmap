
#include "cmap-global-env.h"

#include "cmap-common.h"
#include "cmap-aisle.h"
#include "cmap-cmap.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  CMAP_MAP * global_env = cmap_map_public.create_root(CMAP_AISLE_KERNEL);
  CMAP_SET(global_env, "cmap", cmap_cmap_public.create());
  return global_env;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_GLOBAL_ENV_PUBLIC cmap_global_env_public =
{
  create
};
