
#include "cmap-global-env.h"

#include "cmap.h"
#include "cmap-console.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_global_env_create()
{
  CMAP_MAP * global_env = cmap_map_public.create_root(cmap_aisle_public.kernel);

  CMAP_MAP * cmap = CMAP_MAP(cmap_aisle_public.kernel);
  CMAP_SET(cmap, "console", cmap_console_create());

  CMAP_SET(global_env, "cmap", cmap);

  return global_env;
}
