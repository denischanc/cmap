
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
  CMAP_STRING * line = CMAP_STRING("", 0);

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
  CMAP_MAP * console = CMAP_MAP();
  CMAP_SET(console, "info", CMAP_FN(info));
  CMAP_SET(console, "error", CMAP_FN(error));

  CMAP_SET_GLOBAL("cmap.console", console);
}

/*******************************************************************************
*******************************************************************************/

void cmap_console_delete()
{
}
