
#include "cmap-cli.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_LIST * args = CMAP_LIST(0, proc_ctx);
  int argc = cmap_config_public.argc();
  for(int i = 0; i < argc; i++)
  {
    const char * arg = cmap_config_public.argv()[i];
    CMAP_LIST_PUSH(args, CMAP_STRING(arg, 0, proc_ctx));
  }

  return cmap_to_map(proc_ctx, "args", args, NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CLI_PUBLIC cmap_cli_public = { create };
