
#include "cmap-global-env.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fw.h"
#include "cmap-console.h"

/*******************************************************************************
*******************************************************************************/

void cmap_global_env_create()
{
  CMAP_MAP * global_env = cmap_root_map_create("TODO");
  cmap_kernel() -> global_env_ = global_env;

  CMAP_SET(global_env, "cmap", CMAP_MAP("TODO"));

  cmap_console_create();
}

/*******************************************************************************
*******************************************************************************/

void cmap_global_env_delete()
{
  CMAP_MAP * global_env = cmap_kernel() -> global_env_;
  if(global_env != NULL)
  {
    CMAP_MAP * cmap = CMAP_GET(global_env, "cmap");
    if(cmap != NULL)
    {
      cmap_console_delete();

      CMAP_CALL(cmap, delete);
    }

    CMAP_CALL(global_env, delete);
    cmap_kernel() -> global_env_ = NULL;
  }
}
