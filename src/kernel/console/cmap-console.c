
#include "cmap-console.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-kernel.h"
#include "cmap-map.h"
#include "cmap-list.h"
#include "cmap-fn.h"
#include "cmap-common.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * info(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  fprintf(stdout, "console info !!!\n");
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * error(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  fprintf(stderr, "console error !!!\n");
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

void cmap_console_create()
{
  CMAP_MAP * console = _M_();
  CMAP_CALL_ARGS(console, set, "info", (CMAP_MAP *)cmap_fn_create(info));
  CMAP_CALL_ARGS(console, set, "error", (CMAP_MAP *)cmap_fn_create(error));

  CMAP_MAP * cmap = CMAP_CALL_ARGS(cmap_kernel() -> global_env_, get, "cmap");
  CMAP_CALL_ARGS(cmap, set, "console", console);
}

/*******************************************************************************
*******************************************************************************/

void cmap_console_delete()
{
}
