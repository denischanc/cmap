
#include "cmap-global-env.h"

#include "cmap-fw.h"
#include "cmap-console.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_global_env_create()
{
  CMAP_MAP * global_env = cmap_root_map_create(CMAP_AISLE_KERNEL);

  CMAP_SET(global_env, "cmap", CMAP_MAP(CMAP_AISLE_KERNEL));

  cmap_console_create();

  return global_env;
}
