
#include "cmap-console.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-fw.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args)
{
  CMAP_STRING * line = CMAP_STRING("", 0, CMAP_AISLE_LOCAL);

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

  fprintf(f, "%s\n", CMAP_CALL(line, val));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * info(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  display(stdout, args);
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * error(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  display(stderr, args);
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

CMAP_MAP * cmap_console_create()
{
  CMAP_MAP * console = CMAP_MAP(CMAP_AISLE_KERNEL);

  CMAP_SET(console, "info", CMAP_FN(info, CMAP_AISLE_KERNEL));
  CMAP_SET(console, "error", CMAP_FN(error, CMAP_AISLE_KERNEL));

  return console;
}
