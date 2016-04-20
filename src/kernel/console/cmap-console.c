
#include "cmap-console.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * info(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_STRING * line = _S_("", 0);

  if(args != NULL)
  {
    int i;
    for(i = 0; i < CMAP_CALL(args, size); i++)
    {
      CMAP_MAP * tmp = CMAP_CALL_ARGS(args, get, i);
      if(CMAP_CALL(tmp, nature) == CMAP_STRING_NATURE)
      {
        CMAP_CALL_ARGS(line, append, CMAP_CALL((CMAP_STRING *)tmp, val));
      }
    }
  }

  fprintf(stdout, "%s\n", CMAP_CALL(line, val));

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
  CMAP_CALL_ARGS(console, set, "info", (CMAP_MAP *)_F_(info));
  CMAP_CALL_ARGS(console, set, "error", (CMAP_MAP *)_F_(error));

  CMAP_MAP * cmap = CMAP_CALL_ARGS(cmap_kernel() -> global_env_, get, "cmap");
  CMAP_CALL_ARGS(cmap, set, "console", console);
}

/*******************************************************************************
*******************************************************************************/

void cmap_console_delete()
{
}
