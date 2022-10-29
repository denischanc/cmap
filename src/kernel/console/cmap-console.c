
#include "cmap-console.h"

#include <stdlib.h>
#include <stdio.h>
#include "cmap-common.h"
#include "cmap-aisle.h"
#include "cmap-list.h"
#include "cmap-string.h"
#include "cmap-fn.h"

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
  CMAP_MAP * console = CMAP_MAP(CMAP_AISLE_GLOBAL);

  CMAP_SET(console, "info", CMAP_FN(info, CMAP_AISLE_GLOBAL));
  CMAP_SET(console, "error", CMAP_FN(error, CMAP_AISLE_GLOBAL));

  return console;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public =
{
  create
};
