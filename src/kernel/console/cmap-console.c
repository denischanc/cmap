
#include "cmap-console.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-common-define.h"
#include "cmap.h"
#include "cmap-aisle.h"
#include "cmap-list.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args)
{
  CMAP_STRING * line = CMAP_STRING("", 0, cmap_aisle_public.local);

  if(args != NULL)
  {
    int i;
    for(i = 0; i < CMAP_CALL(args, size); i++)
    {
      CMAP_MAP * tmp = CMAP_CALL_ARGS(args, get, i);
      if(CMAP_NATURE(tmp) == cmap_string_public.nature)
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

static CMAP_MAP * create()
{
  CMAP_MAP * console = CMAP_MAP(cmap_aisle_public.kernel);

  CMAP_SET(console, "info", CMAP_FN(info, cmap_aisle_public.kernel));
  CMAP_SET(console, "error", CMAP_FN(error, cmap_aisle_public.kernel));

  return console;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public =
{
  create
};
