
#include "cmap-console.h"

#include <stdio.h>
#include <cmap/cmap.h>
#include <cmap/define-min.h>

/*******************************************************************************
*******************************************************************************/

static void display(FILE * f, CMAP_LIST * args)
{
  CMAP_STRING * line = $STR_L("");
  $$_L(line, "append", args);
  fprintf(f, "%s\n", $STRV(line));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * info(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  display(stdout, args);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * error(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  display(stderr, args);
  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * create()
{
  CMAP_MAP * console = $MAP(CMAP_AISLE_GLOBAL);
  CMAP_SET(console, "info", $FN(info, CMAP_AISLE_GLOBAL));
  CMAP_SET(console, "error", $FN(error, CMAP_AISLE_GLOBAL));
  return console;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSOLE_PUBLIC cmap_console_public =
{
  create
};
