
#include "cmap-global-env.h"

#include <stdlib.h>
#include "cmap-common.h"
#include "cmap-console.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  CMAP_MAP * cmap = CMAP_MAP(CMAP_AISLE_GLOBAL);
  CMAP_SET(cmap, "console", cmap_console_public.create());

  CMAP_MAP * global_env = cmap_map_public.create_root(CMAP_AISLE_KERNEL);
  CMAP_SET(global_env, "cmap", cmap);

  return global_env;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_GLOBAL_ENV_PUBLIC cmap_global_env_public =
{
  create
};
