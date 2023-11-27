
#include "cmap-cli.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-aisle.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create(CMAP_PROC_CTX * proc_ctx, int argc, char ** argv)
{
  CMAP_LIST * args = CMAP_LIST(0, proc_ctx, CMAP_AISLE_GLOBAL);
  for(int i = 0; i < argc; i++)
    CMAP_LIST_PUSH(args, CMAP_STRING(argv[i], 0, proc_ctx, CMAP_AISLE_GLOBAL));

  return cmap_to_map(proc_ctx, CMAP_AISLE_GLOBAL, "args", args, NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CLI_PUBLIC cmap_cli_public = { create };
