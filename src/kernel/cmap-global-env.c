
#include "cmap-global-env.h"

#include "cmap-fw.h"
#include "cmap-console.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_global_env_create()
{
  CMAP_MAP * global_env = cmap_map_public.create_root(CMAP_AISLE_KERNEL);

  CMAP_MAP * cmap = CMAP_MAP(CMAP_AISLE_KERNEL);
  CMAP_SET(cmap, "console", cmap_console_create());

  CMAP_SET(global_env, "cmap", cmap);

  return global_env;
}
